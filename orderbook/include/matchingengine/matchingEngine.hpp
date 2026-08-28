#ifndef __MATCHINGENGINE__
#define __MATCHINENGINE__
#include "../order/order.hpp"
#include "../orderbook/orderbook.hpp"
#include "../trade/trade.hpp"
#include "../users/users.hpp"

#include <cstddef>
#include <flat_map>
#include <map>
#include <string>
#include <vector>

namespace Market::core {
namespace Trading {

class matchingEngine {
public:
  constexpr std::size_t getNumberOfTrades() const noexcept {
    return numberOfTrades;
  }
  constexpr std::size_t getMarketVolume() const noexcept {
    return marketVolume;
  }
  constexpr std::string_view getId() const noexcept { return m_Id; }
  constexpr system_time_t getTimeStamp() const noexcept { return m_timestamp; }

  bool addOrderBook(Orderbook &ordrbook) noexcept;
  bool removeOrderBook(Orderbook &orderbook) noexcept;

  bool matchMarketOrder(Order::Order &order, Orderbook &orderbook) noexcept;
  bool matchLimitOrder(Order::Order &order, Orderbook &orderbook) noexcept;

  bool updateUserPosition(std::string_view userID) noexcept;
  bool replayEngine(); // to be implemented in V2

private:
  std::size_t numberOfTrades;
  std::size_t marketVolume;
  system_time_t m_timestamp;
  std::string m_Id;
  std::vector<Trading::Trade> m_trades;
  std::flat_map<std::string, Orderbook> m_orderbooks;
  std::map<std::string, Users::User> m_registry;
};

} // namespace Trading
} // namespace Market::core

#endif