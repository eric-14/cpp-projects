#ifndef __ORDERBOOK__
#define __ORDERBOOK__

#include <expected>
#include <flat_map>
#include <memory>
#include <string>
#include <vector>

#include "../logger/applogger/asyncLogger.hpp"
#include "../users/users.hpp"
#include "../utils/validationEngine/validationEngine.hpp"

namespace Market::core::Snap {
class snapStore;
}
namespace Market::core {
namespace Trading {

typedef struct {
  bool match = false;
  double numberOfTrades = 0.0;
  double volume = 0.0; /// Number of units bought
  uint16_t statusbits = 0;

  /**
     0 - default
     1 - bid is lower than ask
     2. - order has been completely matched
     3 - user does not have enough amount to place the bid
     4 - users ASK is higher that the highest bid prices
  */

} MatchStatus;
/// Forward declaration of the snapstore

/// Add allocator for maps to consume a preallocated memory chunk
class Orderbook {
public:
  explicit Orderbook(std::string Id, std::string symbol,
                     system_time_t timestamp)
      : m_Id{Id}, m_symbol{symbol}, m_timestamp{timestamp}, m_bid{}, m_ask{} {}

  ~Orderbook() = default;

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
  std::expected<std::shared_ptr<Order::Order>, SystemError::UndefinedState>
  findOrder(std::shared_ptr<Order::Order> Order) noexcept;

  [[nodiscard]]
  std::expected<bool, SystemError::OrderEntryError>
  removeOrder(std::shared_ptr<Order::Order> Order) noexcept;

  std::expected<MatchStatus, SystemError::UndefinedState>
  matchOrder(std::shared_ptr<Order::Order> order,
             std::shared_ptr<Users::User> user);

  double getBestAsk() const noexcept;
  double getBestBid() const noexcept;
  double getSpread() const noexcept;

  std::string_view getId() const noexcept { return m_Id; };
  std::string_view getSymbol() const noexcept { return m_symbol; };

  system_time_t getTimestamp() const noexcept;

  std::string_view getOrderBookId() const noexcept;

  const std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>> *
  getAsk() const noexcept;

  const std::flat_map<double, std::vector<std::shared_ptr<Order::Order>>,
                      std::greater<double>> *
  getBid() const noexcept;

private:
  // order book Id
  std::string m_Id;
  std::string m_symbol;
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