#ifndef __USERS__
#define __USERS__

#include "../order/order.hpp"
#include "../trade/trade.hpp"
#include <map>
#include <string>

namespace Market::core {
namespace Users {

/// Number of positions held by the user
struct position {
  double marketPrice;
  double units; // amount of units in that position
  std::string symbol;
  // Default unique identifier for different orders placed in the system
  system_time_t timestamp;
};

class User {
public:
  User(std::string Id, std::string name, system_time_t timestamp)
      : m_timestamp{timestamp}, m_Id{Id} {}

  constexpr system_time_t getTimestamp() const noexcept { return m_timestamp; };
  constexpr std::string_view getId() const noexcept { return m_Id; };
  constexpr std::string_view getUserName() const noexcept {
    return m_username;
  };
  constexpr double getAccountAmount() const noexcept { return account_amount; };
  constexpr double getRealizedPnl() const noexcept { return realized_pnl; };
  constexpr std::vector<Trading::Trade> &getTrades() noexcept { return trades; }
  void depositAmount(double amount) noexcept;
  bool withdrawAmount(double amount) noexcept;

  bool executeOrder(Order::Order &order) noexcept;

  void updatePosition(std::string symbol, position pos) noexcept;
  std::optional<std::vector<position>> findPosition(std::string symbol);

private:
  double account_amount;
  double realized_pnl;
  std::string m_Id; // user Id
  std::string m_username;
  system_time_t m_timestamp;
  std::vector<Trading::Trade> trades; // trades done by the user
  std::map<std::string, std::vector<position>>
      m_position; // track live positions in the market
};
} // namespace Users
} // namespace Market::core

#endif