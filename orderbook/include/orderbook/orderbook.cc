#include "orderbook.hpp"
// #include "snapStore.hpp"
#include "../matchingengine/matchingEngineConstants.hpp"
#include "../utils/unorderedDelete.hpp"
#include <chrono>
#include <cstddef>
#include <expected>
#include <memory>
#include <string>
namespace MCORE = Market::core;
namespace MCORE_T = Market::core::Trading;
namespace MCORE_O = Market::core::Order;

using namespace MCORE_T;

MCORE::system_time_t Orderbook::getTimestamp() const noexcept {
  return m_timestamp;
}

double Orderbook::getBestAsk() const noexcept {
  if (m_ask.empty())
    return 0.0;
  return m_ask.begin()->first; // O(1) operation
}
/***
    @brief Match bids and asks at the market price.
           Bids are matched to the best asks and user portfolios are updated
    @return matchstatus - contains descriptive information on the status of the
   trades
*/
MatchStatus Orderbook::matchMarketOrder(std::shared_ptr<MCORE_O::Order> order,
                                        std::shared_ptr<Users::User> user) {
  MatchStatus status;
  auto ordersym = order->getSymbol();
  auto ordertype = order->getOrderType();

  if (ordersym != m_symbol || ordertype != Order::OrderType::MARKET)
      [[unlikely]] {
    status.match = false;
    status.statusbits = ORDER_ERROR | SYMBOL_OR_MARKET_ERROR;
    return status;
  }
  uint8_t invalidOrderState =
      static_cast<uint8_t>(Order::OrderState::INVALID) |
      static_cast<uint8_t>(Order::OrderState::REJECTED) |
      static_cast<uint8_t>(Order::OrderState::FILLED) |
      static_cast<uint8_t>(Order::OrderState::CANCELED);

  auto userside = order->getOrderSide();
  auto userquantity = order->getOriginalAmount();
  auto userprice = order->getPrice();

  auto bidCost = userquantity * userprice;
  auto userAmount = user->getAccountAmount();

  /// TODO: reserve memory for the vector below - prevents memory allocations

  if (userside == Order::OrderSide::BID) {
    if (bidCost > userAmount) {
      status.match = false;
      status.statusbits = INVALID_USER_AMOUNT;
      return status;
    }
    if (m_ask.empty()) [[unlikely]] {
      status.statusbits = ORDER_FILLED;
      status.match = false;
      return status;
    }
    std::size_t tradeNumber = 0;
    for (auto it = m_ask.begin(); it != m_ask.end(); ++it) {
      // have a function that returns top
      // orders like 5 of them
      // have a 0(1) locking system to the top first orders
      auto askprice = it->first;
      if (userprice < askprice) [[unlikely]] {
        status.match = false;
        status.statusbits = BID_LOWER;
        return status; // trade did not execute bid is lower than the ask
      }

      auto bidfill = 0.0;
      auto askmap = it->second;

      for (auto askOrder : askmap) {
        uint8_t _orderstate = static_cast<uint8_t>(askOrder->getOrderState());
        if ((_orderstate & invalidOrderState) != 0)
          continue;
        auto askquantity = askOrder->getOriginalAmount();
        auto askprice = askOrder->getPrice();
        userquantity -= askquantity;

        if (userquantity > 0) {
          auto userPayment = user->withdrawAmount(askprice * askquantity);
          if (!userPayment) {
            status.statusbits =
                INVALID_USER_AMOUNT |
                PARTIALLY_FILLED_ORDER; /// User could not complete the orders
            return status;
          }
          /// TODO: check performance implication of to_string
          Trade finalizedTrade{askprice,
                               askquantity,
                               std::chrono::steady_clock::now(),
                               m_symbol,
                               user->getId(),
                               std::to_string(++tradeNumber),
                               order->getId(),
                               askOrder->getId()};
          /// Order is now filled
          askOrder->updateOrderState(Order::OrderState::FILLED);
          /// erase the bid order from the system
          unorderedDelete(askmap, askOrder);

          /// Record Trade for the system
          m_trades.emplace_back(finalizedTrade);
          status.numberOfTrades++;
        }

        // best askprice
        if (userquantity <= 0) {
          auto userPayment = user->withdrawAmount(askprice * askquantity);
          if (!userPayment) {
            status.statusbits =
                PARTIALLY_FILLED_ORDER; /// User could not complete the trades
            return status;
          }
          auto remainingaskamount = askquantity - userquantity;
          bool updateState = askOrder->updateOrderAmount(remainingaskamount);
          Trade finalizedTrade{askprice,
                               askquantity,
                               std::chrono::steady_clock::now(),
                               m_symbol,
                               user->getId(),
                               std::to_string(++tradeNumber),
                               order->getId(),
                               askOrder->getId()};
          /// Record Trade for the system
          m_trades.emplace_back(finalizedTrade);
          if (remainingaskamount == 0) {
            askOrder->updateOrderState(Order::OrderState::FILLED);
            /// erase the bid order from the system
            unorderedDelete(askmap, askOrder);
          }
          // Update user portfolio
          user->updatePosition(
              m_symbol, Users::position{bidCost, userquantity, m_symbol});
          order->updateOrderState(Order::OrderState::PARTIALLY_FILLED);

          // create object trade
          // all bids have been filled
          status.numberOfTrades++;
          status.match = true;
          status.statusbits = ORDER_FILLED;
          return status;
        }
      }
    }
    order->updateOrderState(Order::OrderState::PARTIALLY_FILLED);
  } else if (userside == Order::OrderSide::ASK) {
    /// User wants to sell
    /// They should sell
    if (m_bid.empty()) [[unlikely]] {
      status.match = false;
      return status;
    }
    std::size_t tradeNumber = 0;
    for (auto it = m_bid.begin(); it != m_bid.end(); ++it) {

      // have a function that returns top
      // orders like 5 of them
      // have a 0(1) locking system to the top first orders
      auto bidprice = it->first;
      auto bidmap = it->second;
      /// if the users selling price is
      /// Higher then the a tade cannot be made
      if (userprice > bidprice) [[unlikely]] {
        status.match = false;
        status.statusbits = BID_LOWER;
        return status; // trade did not execute bid is lower than the ask
      }

      auto bidfill = 0.0;

      for (auto bidOrder : bidmap) {
        /// Safety guard but ideally all FILLED Orders have been removed
        uint8_t _orderstate = static_cast<uint8_t>(bidOrder->getOrderState());
        if ((_orderstate & invalidOrderState) != 0) [[unlikely]]
          continue;
        auto bidquantity = bidOrder->getOriginalAmount();
        auto bidprice = bidOrder->getPrice();
        userquantity -= bidquantity;

        if (userquantity > 0) {
          /// TODO: check performance implication of to_string
          Trade finalizedTrade{bidprice,
                               bidquantity,
                               std::chrono::steady_clock::now(),
                               m_symbol,
                               user->getId(),
                               std::to_string(++tradeNumber),
                               bidOrder->getId(),
                               order->getId()};
          /// Order is now filled
          bidOrder->updateOrderState(Order::OrderState::FILLED);
          /// erase the bid order from the system
          unorderedDelete(bidmap, bidOrder);
          /// Record Trade for the system
          m_trades.emplace_back(finalizedTrade);
          status.numberOfTrades++;
        }

        // best askprice
        if (userquantity <= 0) {
          auto remainingaskamount = bidquantity - userquantity;
          bool updateState = bidOrder->updateOrderAmount(remainingaskamount);
          Trade finalizedTrade{bidprice,
                               bidquantity,
                               std::chrono::steady_clock::now(),
                               m_symbol,
                               user->getId(),
                               std::to_string(++tradeNumber),
                               bidOrder->getId(),
                               order->getId()};

          /// Record Trade for the system
          m_trades.emplace_back(finalizedTrade);
          auto transactionAmount =
              order->getOriginalAmount() * order->getPrice();
          /// Profits and loses user made while trading
          user->depositAmount(transactionAmount);
          // Update user portfolio
          user->updatePosition(m_symbol,
                               Users::position{bidCost, bidquantity, m_symbol});
          if (remainingaskamount == 0) {
            bidOrder->updateOrderState(Order::OrderState::FILLED);
            /// erase the bid order from the system
            unorderedDelete(bidmap, bidOrder);
          }
          order->updateOrderState(Order::OrderState::FILLED);
          bidOrder->updateOrderState(Order::OrderState::PARTIALLY_FILLED);

          // create object trade
          // all bids have been filled
          status.numberOfTrades++;
          status.match = true;
          status.statusbits = ORDER_FILLED;
          return status;
        }
      }
    }
    order->updateOrderState(Order::OrderState::PARTIALLY_FILLED);
  }

  return status;
}

/***
    @brief Match limit orders. The maximum and minimum bid price are defined in
   the order. OrderTime Frame is also confirmed
    @return returns the status of the trade
*/
MatchStatus
Orderbook::matchLimitOrder(std::shared_ptr<MCORE_O::Order> order,
                           std::shared_ptr<Market::core::Users::User> user) {
  MatchStatus status;
  auto limitsym = order->getSymbol();
  auto ordertype = order->getOrderType();

  if (m_symbol != limitsym || ordertype != Order::OrderType::LIMIT)
      [[unlikely]] {
    status.match = false;
    status.statusbits = ORDER_ERROR;
    return status;
  }

  auto limitprice = order->getLimitPrice();
  auto limitState = order->getOrderState();
  auto limitSide = order->getOrderSide();
  /// Check first of the order state is in invalid state
  /// This is validated by the validation engine
  /// Further Validation is required limit orders
  if (limitState == Order::OrderState::CANCELED ||
      limitState == Order::OrderState::INVALID ||
      limitState == Order::OrderState::REJECTED) [[unlikely]] {
    status.match = false;
    status.statusbits = ORDER_ERROR;
    return status;
  }
  ///
  auto t_status = matchMarketOrder(order, user);
  return t_status;
}
bool matchBid(std::shared_ptr<Market::core::Order::Order> order);

double Orderbook::getBestBid() const noexcept {
  if (m_ask.empty())
    return 0.0;
  return m_bid.begin()->first; // O(1) operation
}

double Orderbook::getSpread() const noexcept {
  if (m_ask.empty() || m_bid.empty())
    return 0.0;
  return m_ask.begin()->first - m_bid.begin()->first; // O(1) operation
}

std::string_view Orderbook::getOrderBookId() const noexcept { return m_Id; }

const std::flat_map<double, std::vector<std::shared_ptr<MCORE_O::Order>>> *
Orderbook::getAsk() const noexcept {
  return &m_ask;
}
const std::flat_map<double, std::vector<std::shared_ptr<MCORE_O::Order>>,
                    std::greater<double>> *
Orderbook::getBid() const noexcept {
  return &m_bid;
}

[[nodiscard]]
std::expected<bool, SystemError::OrderEntryError>
Orderbook::addOrder(std::shared_ptr<MCORE_O::Order> Order) noexcept {
  /// Add new order
  /// This should be strictly construction to a preallocated memory
  if (Order->getSymbol() == m_symbol) [[likely]] {
    auto orderDir = Order->getOrderSide();
    double price = Order->getPrice();
    if (orderDir == MCORE_O::OrderSide::BID) {
      m_bid[price].emplace_back(Order);
      return true;
    } else if (orderDir == MCORE_O::OrderSide::ASK) {
      m_ask[price].emplace_back(Order);
      return true;
    }
  } else [[unlikely]] {
    return std::unexpected(
        SystemError::OrderEntryError("Failed to add order to Orderbook"));
  }
}
/**
    @brief RemoveOrder fucntion to remove traded orders from the maps
            When order becomes it should be removed.


    @return returns bool

*/
std::expected<bool, SystemError::OrderEntryError>
Orderbook::removeOrder(std::shared_ptr<MCORE_O::Order> Order) noexcept {}

[[nodiscard]]
std::expected<std::shared_ptr<MCORE_O::Order>, SystemError::UndefinedState>
Orderbook::findOrder(std::shared_ptr<MCORE_O::Order> Order) noexcept {
  auto orderside = Order->getOrderSide();
  auto orderId = Order->getId();
  auto orderprice = Order->getPrice();
  if (orderside == MCORE_O::OrderSide::BID) {
    if (m_bid.empty()) {
      return std::unexpected(SystemError::UndefinedState("Order not found"));
    }
    if (m_bid.find(orderprice) != m_bid.end()) [[likely]] {
      for (const auto &order : m_bid[orderprice]) {
        if (orderId == order->getId()) {
          return order;
        }
      }
    }
  } else if (orderside == MCORE_O::OrderSide::ASK) {
    if (m_ask.empty()) {
      return std::unexpected(SystemError::UndefinedState("Order not found"));
    }
    if (m_ask.find(orderprice) != m_ask.end()) [[likely]] {
      for (const auto &order : m_ask[orderprice]) {
        if (orderId == order->getId()) {
          return order;
        }
      }
    }
  } else [[unlikely]] {
    return std::unexpected(SystemError::UndefinedState("Order not found"));
  }
}
// Orderbook::~Orderbook() {
//   Snap::snapObject tmp = this;
//   /// wait for all async snapshot of the orderbook before destruction
//   bool state = m_snap->addSystem(tmp);
//   if (state) {
//     auto state = m_asyncLogger->addLog("[tm {}] Saved Orderbook");
//   } else {
//     auto sm = m_asyncLogger->addError(SystemError::SnapFailedToSave(
//         "[] Failed to save order book on Shutdown/deletion"));
//   }
//   ///
//   system_time_t d_timestamp;
//   auto s1 = m_asyncLogger->addLog(
//       std::format("Shutting down Order book Id {} at time {}", m_Id,
//                   d_timestamp.time_since_epoch()));
// }