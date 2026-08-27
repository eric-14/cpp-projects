#ifndef __ASYNC__
#define __ASYNC__

#include "../order/order.hpp"
#include <expected>
#include <string>
namespace Market::core {
namespace Trading {
class Trade {
public:
  explicit Trade(double price, double quantity, system_time_t timestamp,
                 std::string symbol, std::string userID, std::string tradeID,
                 std::string buyOrderID, std::string sellOrderID)
      : m_price{price}, m_quantity{quantity}, m_timestamp{timestamp},
        m_symbol{symbol}, m_userID{userID}, m_tradeID{tradeID},
        m_buyOrderID{buyOrderID}, m_sellOrderID{sellOrderID} {}

  constexpr system_time_t getTimestamp() const noexcept { return m_timestamp; };
  constexpr std::string_view getId() const noexcept { return m_tradeID; };
  constexpr std::string_view getUserName() const noexcept { return m_userID; };
  constexpr double getPrice() const noexcept { return m_price; };
  constexpr double getQuantity() const noexcept { return m_quantity; };

private:
  double m_price;
  double m_quantity; // units traded
  system_time_t m_timestamp;
  std::string m_symbol;
  std::string m_userID;
  std::string m_tradeID;
  std::string m_buyOrderID;
  std::string m_sellOrderID;
};
} // namespace Trading
} // namespace Market::core

#endif