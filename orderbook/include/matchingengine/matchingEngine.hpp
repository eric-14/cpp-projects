#ifndef __MATCHINGENGINE__
#define __MATCHINENGINE__
#include "../order/order.hpp"
#include "../orderbook/orderbook.hpp"
#include "../trade/trade.hpp"
#include "../utils/unorderedDelete.hpp"
#include <atomic>
#include <cstddef>
#include <flat_map>
#include <map>
#include <memory>
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
    return m_numberOfTrades;
  }
  constexpr std::size_t getMarketVolume() const noexcept {
    return m_marketVolume;
  }
  constexpr std::string_view getId() const noexcept { return m_Id; }
  constexpr system_time_t getTimeStamp() const noexcept { return m_timestamp; }

  void addOrderBook(std::shared_ptr<Orderbook> orderbook) noexcept;
  bool removeOrderBook(std::shared_ptr<Orderbook> orderbook) noexcept;

  std::shared_ptr<Orderbook> findOrderBook(std::string_view symbol) noexcept;

  EngineState startEngine() noexcept {
    m_engineState.store(EngineState::OPEN);
    return m_engineState.load(std::memory_order_acquire);
  }
  EngineState stopEngine() noexcept {
    m_engineState.store(EngineState::CLOSED);
    return m_engineState.load(std::memory_order_acquire);
  }
  constexpr EngineState getState() noexcept {
    return m_engineState.load(std::memory_order_relaxed);
  }
  constexpr std::size_t numberOfOrderbooks() noexcept {
    return m_orderbooks.size();
  }
  bool matchOrder(std::shared_ptr<Order::Order> order,
                  std::shared_ptr<Users::User> user,
                  std::shared_ptr<Orderbook> orderbook) noexcept;

  bool updateUserPosition(std::string_view userID) noexcept;
  std::expected<bool, SystemError::UndefinedState>
  replayEngine(); // to be implemented in V2

  std::expected<bool, SystemError::UndefinedState>
  addUser(std::shared_ptr<Users::User> user);

  std::expected<Users::User, SystemError::UndefinedState>
  findUser(std::shared_ptr<Users::User> user);

private:
  std::atomic<EngineState> m_engineState{EngineState::CLOSED};
  std::size_t m_numberOfTrades;
  std::size_t m_marketVolume;
  system_time_t m_timestamp;
  std::string m_Id;

  std::vector<Trading::Trade> m_trades;

  std::flat_map<std::string_view, std::shared_ptr<Orderbook>> m_orderbooks;
  std::map<std::string_view, Users::User> m_registry;

  bool matchMarketOrder(std::shared_ptr<Order::Order> order,
                        std::shared_ptr<Users::User> user,
                        std::shared_ptr<Orderbook> orderbook) noexcept;

  bool matchLimitOrder(std::shared_ptr<Order::Order> order,
                       std::shared_ptr<Orderbook> orderbook) noexcept;
};

} // namespace Trading
} // namespace Market::core

#endif