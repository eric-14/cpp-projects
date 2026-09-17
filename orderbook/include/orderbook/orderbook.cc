#include "orderbook.hpp"
// #include "snapStore.hpp"
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
std::expected<MatchStatus, SystemError::UndefinedState>
Orderbook::matchOrder(std::shared_ptr<MCORE_O::Order> order,
                      std::shared_ptr<Users::User> user) {
  MatchStatus status;
  auto ordersym = order->getSymbol();

  if (ordersym != m_symbol) [[unlikely]] {
    return std::unexpected(
        SystemError::UndefinedState("Order and Orderbook do not match"));
  }

  auto userside = order->getOrderSide();
  auto userquantity = order->getOriginalAmount();
  auto userprice = order->getPrice();

  auto bidCost = userquantity * userprice;
  auto userAmount = user->getAccountAmount();

  /// TODO: reserve memory for the vector below - prevents memory allocations
  std::vector<Trade> m_trades;
  if (userside == Order::OrderSide::BID) {
    if (bidCost > userAmount) {
      status.match = false;
      status.statusbits = 3;
      return status;
    }
    if (m_ask.empty()) [[unlikely]] {
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
        status.statusbits = 1;
        return status; // trade did not execute bid is lower than the ask
      }

      auto bidfill = 0.0;

      for (auto askOrder : it->second) {

        auto askquantity = askOrder->getOriginalAmount();
        auto askprice = askOrder->getPrice();
        userquantity -= askquantity;

        if (userquantity > 0) {
          /// TODO: check performance implication of to_string
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
          status.numberOfTrades++;
        }

        // best askprice
        if (userquantity <= 0) {
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

          user->withdrawAmount(bidCost);
          // Update user portfolio
          user->updatePosition(
              m_symbol, Users::position{bidCost, userquantity, m_symbol});
          order->updateOrderState(Order::OrderState::FILLED);

          // create object trade
          // all bids have been filled
          status.numberOfTrades++;
          status.match = true;
          status.statusbits = 2;
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
      /// if the users selling price is
      /// Higher then the a tade cannot be made
      if (userprice > bidprice) [[unlikely]] {
        status.match = false;
        status.statusbits = 1;
        return status; // trade did not execute bid is lower than the ask
      }

      auto bidfill = 0.0;

      for (auto bidOrder : it->second) {

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
          order->updateOrderState(Order::OrderState::FILLED);

          // create object trade
          // all bids have been filled
          status.numberOfTrades++;
          status.match = true;
          status.statusbits = 2;
          return status;
        }
      }
    }
    order->updateOrderState(Order::OrderState::PARTIALLY_FILLED);
  }

  std::unexpected(SystemError::UndefinedState("Could not processed the order"));
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

std::expected<bool, SystemError::OrderEntryError>
MCORE_T::Orderbook::removeOrder(
    std::shared_ptr<MCORE_O::Order> Order) noexcept {
  m_validationEngine->removeOrder(Order);
  /**
      One approach would be to iterate over the map and find the Order to
     remove it, which is extremely expensive in the hotpath The alternative
     which is used here is to have a validation engine that will mark the
     order INVALID in memory
  */
}

[[nodiscard]]
std::expected<std::shared_ptr<MCORE_O::Order>, SystemError::UndefinedState>
MCORE_T::Orderbook::findOrder(std::shared_ptr<MCORE_O::Order> Order) noexcept {
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
// MCORE_T::Orderbook::~Orderbook() {
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