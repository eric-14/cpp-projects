#include "matchingEngine.hpp"
#include "../trade/trade.hpp"
#include <cstddef>
#include <cstdint>
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
void matchingEngine::addOrderBook(
    std::shared_ptr<Orderbook> orderbook) noexcept {
  auto symbol = orderbook->getSymbol();
  if (m_orderbooks.find(symbol) == m_orderbooks.end()) {
    m_orderbooks.insert_or_assign(symbol, orderbook);
  }
}
bool removeOrderBook(std::shared_ptr<Orderbook> orderbook) noexcept;

/***
  @brief Functions returns a pointer to an orderbook inside the ME


  @note There is a risk of pointer invalidation when new orderbooks are inserted
  in the system Validate data before the using the pointer.
*/
std::shared_ptr<Orderbook>
matchingEngine::findOrderBook(std::string_view symbol) noexcept {
  if (m_orderbooks.find(symbol) != m_orderbooks.end()) [[likely]] {
    // return nullptr;
    return m_orderbooks[symbol];
  }
}

bool matchingEngine::matchMarketOrder(
    std::shared_ptr<Order::Order> order, std::shared_ptr<Users::User> user,
    std::shared_ptr<Orderbook> orderbook) noexcept

{

  auto orderprice = order->getPrice();
  auto orderquantity = order->getOriginalAmount();
  auto ordersym = order->getSymbol();
  auto booksym = orderbook->getSymbol();

  if (ordersym != booksym) [[unlikely]] {
    return false;
  }
  /// User Order is matched in the symbol orderbook
  MatchStatus status = orderbook->matchMarketOrder(order, user);

  if (!status.match) {
    /***
      @TODO: Implement a better notification system.
             The information below should be transmitted to system logs
    */
    switch (status.statusbits) {

    case STATEBITS::NC:
      std::println("Not used");
    case STATEBITS::BID_LOWER:
      std::println("Bid lower than ask ");
    case STATEBITS::ORDER_FILLED:
      std::println("BID/ASK completely filled");
    case STATEBITS::INVALID_USER_AMOUNT:
      std::println("User does not have enough amount to place bid");
    case STATEBITS::INVALID_ASK_PRICE:
      std::println("Ask is higher than the best bid");
    case STATEBITS::ORDER_ERROR:
      std::println("General Order Error");
    case STATEBITS::PARTIALLY_FILLED_ORDER:
      std::println(
          "Early termination of trade. User may have run out of funds");
    case STATEBITS::SYMBOL_OR_MARKET_ERROR:
      std::println("market or symbol error");
    default:
      std::println("Out of range error status bit. Bit is {}",
                   static_cast<uint16_t>(status.statusbits));
    }
    /// Log why the trade could not be completed
    /// Matchstatus
    return false; // could not complete trade
  }
  if (status.statusbits == STATEBITS::ORDER_FILLED ||
      status.statusbits == STATEBITS::PARTIALLY_FILLED_ORDER) {
    matchingEngine::m_numberOfTrades += status.numberOfTrades;
    matchingEngine::m_marketVolume += status.volume;
    return true;
  }

  /// Publish the information over the wire
  /// Add information to the statistics engine
  /// Number of trades for this symbol
  /// volume traded for the symbol
  /// Best ask for the day
  /// Best bid for the day
  /// Best spread for the day

  // std::unexpected(SystemError::UndefinedState(
  //     "[Matching Engine]Could not processed Order"));
  return true;
}

bool matchingEngine::matchOrder(std::shared_ptr<Order::Order> order,
                                std::shared_ptr<Users::User> user,
                                std::shared_ptr<Orderbook> orderbook) noexcept {
  return matchingEngine::matchMarketOrder(order, user, orderbook);
}

} // namespace Market::core::Trading