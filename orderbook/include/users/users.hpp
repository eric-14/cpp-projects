#include "../order/order.hpp"
#include "../trade/trade.hpp"
#include <string>

namespace Market::core {
namespace Users {
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
  bool depositAmount(double amount) noexcept;
  bool withdrawAmount(double amount) noexcept;

private:
  double account_amount;
  double realized_pnl;
  std::string m_Id; // user Id
  std::string m_username;
  system_time_t m_timestamp;
  std::vector<Trading::Trade> trades; // trades done by the user
};
} // namespace Users
} // namespace Market::core