#ifndef __MATCHINGENGINE__
#define __MATCHINENGINE__
#include "../order/order.hpp"
#include "../orderbook/orderbook.hpp"
#include "../trade/trade.hpp"

#include <cstddef>
#include <flat_map>
#include <map>
#include <string>
#include <vector>

namespace Market::core {
namespace Trading {

/**
  @brief the state of the matching engine
        OPEN - the engine is up and running and matching trades
        CLOSED - The engine is not matching trades
        STALLED - Processing a trade
        SLAVE - a matching engine that does executions to track master activity
        RECORDER - Similar to slave. Just records the trades done the master

*/
enum class EngineState : uint8_t {
  OPEN,
  CLOSED,
  STALLED,
  MASTER,
  SLAVE,
  RECORDER
};

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

  bool startEngine() noexcept;
  bool stopEngine() noexcept;

  bool matchOrder(Order::Order &order, Orderbook &orderbook) noexcept;

  bool updateUserPosition(std::string_view userID) noexcept;
  std::expected<bool, SystemError::UndefinedState>
  replayEngine(); // to be implemented in V2

  std::expected<bool, SystemError::UndefinedState>
  addUser(std::shared_ptr<Users::User> user);

  std::expected<Users::User, SystemError::UndefinedState>
  findUser(std::shared_ptr<Users::User> user);

private:
  EngineState m_engineState;
  std::size_t numberOfTrades;
  std::size_t marketVolume;
  system_time_t m_timestamp;
  std::string m_Id;

  long long m_numberOfTrades;
  long long m_volume;

  std::vector<Trading::Trade> m_trades;
  std::flat_map<std::string, Orderbook> m_orderbooks;
  std::map<std::string, Users::User> m_registry;

  std::expected<bool, SystemError::UndefinedState>
  matchMarketOrder(std::shared_ptr<Order::Order> order,
                   std::shared_ptr<Users::User> user,
                   std::shared_ptr<Orderbook> orderbook) noexcept;

  bool matchLimitOrder(std::shared_ptr<Order::Order> order,
                       std::shared_ptr<Orderbook> orderbook) noexcept;
};

} // namespace Trading
} // namespace Market::core

#endif