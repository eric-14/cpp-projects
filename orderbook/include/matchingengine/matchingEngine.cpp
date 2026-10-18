#include "matchingEngine.hpp"
#include "../trade/trade.hpp"
#include "matchingEngineConstants.hpp"
#include <cstddef>
#include <expected>
#include <optional>

namespace Market::core::Trading {
/***
    @brief The matching engine is the centralised location that holds all the
    orderbooks. Therefore, all orderbooks are converted to rvalues then assigned
    to the orderbook map. The result is the orderbooks are all stored in
    contiguous memory, increasing probability of cache hit when matching on the
    same symbol
    @TODO: validate the result of the function
*/
void matchingEngine::addOrderBook(Orderbook &&orderbook) noexcept {
  auto symbol = orderbook.getSymbol();
  if (m_orderbooks.find(symbol) == m_orderbooks.end()) {
    m_orderbooks.insert_or_assign(symbol, std::move(orderbook));
  }
}
bool removeOrderBook(std::shared_ptr<Orderbook> orderbook) noexcept;

/***
  @brief Functions returns a pointer to an orderbook inside the ME


  @note There is a risk of pointer invalidation when new orderbooks are inserted
  in the system Validate data before the using the pointer.
*/
std::optional<const Orderbook *>
matchingEngine::findOrderBook(std::string_view symbol) noexcept {
  if (m_orderbooks.find(symbol) != m_orderbooks.end()) [[likely]] {
    auto it = m_orderbooks.find(symbol);
    if (it != m_orderbooks.end()) {
      return &(it->second); // Return pointer to the found Orderbook
    }
    // return nullptr;
    return &m_orderbooks[symbol];
  }
  return std::nullopt;
}

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