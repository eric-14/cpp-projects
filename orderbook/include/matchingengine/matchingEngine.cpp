#include "matchingEngine.hpp"
#include "../trade/trade.hpp"
#include "matchingEngineConstants.hpp"
#include <expected>

namespace Market::core::Trading {

std::expected<bool, SystemError::UndefinedState>
matchingEngine::matchMarketOrder(std::shared_ptr<Order::Order> order,
                                 std::shared_ptr<Users::User> user,
                                 std::shared_ptr<Orderbook> orderbook) noexcept

{
  auto orderprice = order->getPrice();
  auto orderquantity = order->getOriginalAmount();
  auto ordersym = order->getSymbol();
  auto booksym = orderbook->getSymbol();
  if (ordersym != booksym) [[unlikely]] {
    return std::unexpected(SystemError::UndefinedState(
        "[Matching Engine] Orders and Orderbook do not match"));
  }
  /// User Order is matched in the symbol orderbook
  MatchStatus status = orderbook->matchMarketOrder(order, user);
  if (!status.match) {
    // switch (status.statusbits) {
    //   case 1:

    //   default:
    //     std::string
    // }
    /// Log why the trade could not be completed
    /// Matchstatus
    return false; // could not complete trade
  }
  if (status.statusbits == ORDER_FILLED ||
      status.statusbits == PARTIALLY_FILLED_ORDER) {
    matchingEngine::m_numberOfTrades += status.numberOfTrades;
    matchingEngine::m_volume += status.volume;
    return true;
  }

  /// Publish the information over the wire
  /// Add information to the statistics engine
  /// Number of trades for this symbol
  /// volume traded for the symbol
  /// Best ask for the day
  /// Best bid for the day
  /// Best spread for the day

  std::unexpected(SystemError::UndefinedState(
      "[Matching Engine]Could not processed Order"));
}

} // namespace Market::core::Trading