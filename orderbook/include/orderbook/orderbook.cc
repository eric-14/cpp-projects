#include "orderbook.hpp"
#include "snapStore.hpp"
#include <expected>

namespace MCORE = Market::core;
namespace MCORE_T = Market::core::Trading;
namespace MCORE_O = Market::core::Order;

MCORE::system_time_t MCORE_T::Orderbook::getTimestamp() const noexcept {
  return m_timestamp;
}

double MCORE_T::Orderbook::getBestAsk() const noexcept {
  if (m_ask.empty())
    return 0.0;
  return m_ask.begin()->first; // O(1) operation
}

double MCORE_T::Orderbook::getBestBid() const noexcept {
  if (m_ask.empty())
    return 0.0;
  return m_bid.begin()->first; // O(1) operation
}

double MCORE_T::Orderbook::getSpread() const noexcept {
  if (m_ask.empty() || m_bid.empty())
    return 0.0;
  return m_ask.begin()->first - m_bid.begin()->first; // O(1) operation
}

std::string_view MCORE_T::Orderbook::getOrderBookId() const noexcept {
  return m_Id;
}

const std::flat_map<double, std::vector<std::shared_ptr<MCORE_O::Order>>> &
MCORE_T::Orderbook::getAsk() const noexcept {
  return m_ask;
}
const std::flat_map<double, std::vector<std::shared_ptr<MCORE_O::Order>>,
                    std::greater<double>> &
MCORE_T::Orderbook::getBid() const noexcept {
  return m_bid;
}

[[nodiscard]]
std::expected<bool, SystemError::OrderEntryError>
MCORE_T::Orderbook::addOrder(std::shared_ptr<MCORE_O::Order> Order) noexcept {
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