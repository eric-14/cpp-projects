#ifndef __ORDERBOOK__
#define __ORDERBOOK__

#include <expected>
#include <flat_map>
#include <string>
#include <vector>

#include "../logger/applogger/asyncLogger.hpp"
#include "../utils/validationEngine/validationEngine.hpp"

namespace Market::core::Snap {
class snapStore;
}
namespace Market::core {
namespace Trading {

/// Forward declaration of the snapstore

/// Add allocator for maps to consume a preallocated memory chunk
class Orderbook {
public:
  Orderbook(std::string Id, system_time_t timestamp)
      : m_Id{Id}, m_timestamp{timestamp} {}

  ~Orderbook();

  Orderbook(Orderbook &) = delete;
  Orderbook operator=(Orderbook &rhs) = delete;

  Orderbook(Orderbook &&) = default;
  Orderbook &operator=(Orderbook &&rhs) = default;

  std::expected<std::shared_ptr<Orderbook>, SystemError::OrderFailedToBuild>
  buildEngineFromSnap(std::shared_ptr<Snap::snapStore> snap);

  [[nodiscard]]
  std::expected<bool, SystemError::OrderEntryError>
  addOrder(std::shared_ptr<Order::Order> Order) noexcept;

  [[nodiscard]]
  std::expected<bool, SystemError::OrderEntryError>
  removeOrder(std::shared_ptr<Order::Order> Order) noexcept;

  double getBestAsk() const noexcept;
  double getBestBid() const noexcept;
  double getSpread() const noexcept;

  constexpr std::string_view getId() const noexcept { return m_Id; };

  system_time_t getTimestamp() const noexcept;

  std::string_view getOrderBookId() const noexcept;

  const std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>> &
  getAsk() const noexcept;

  const std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>,
                      std::greater<double>> &
  getBid() const noexcept;

private:
  // order book Id
  std::string m_Id;
  // time of creation of orderbook
  system_time_t m_timestamp;

  std::shared_ptr<Logger::asyncLogger> m_asyncLogger;
  /// A snapshot of the Order engine
  /// Taken on the
  std::shared_ptr<Snap::snapStore> m_snap;
  std::shared_ptr<Validation::validationEngine> m_validationEngine;
  // C++23 flatmap implementation
  // cache friendly
  /// Trade off is in insertion speed.
  /// Add memory preallocation
  std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>>

      m_ask; // initial sell order has the lowest price
  std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>,
                std::greater<double>>
      m_bid;
};

} // namespace Trading
} // namespace Market::core

#endif