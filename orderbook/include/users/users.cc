#include "users.hpp"
#include <cstddef>
#include <optional>

namespace MCORE = Market::core;
namespace MCORE_U = Market::core::Users;

using namespace Market::core::Users;
using namespace MCORE::Order;
bool User::withdrawAmount(double amount) noexcept {
  if (account_amount < 0) [[unlikely]] {
    return false;
  }
  account_amount -= amount;
  return true;
}
void User::depositAmount(double amount) noexcept { account_amount += amount; }
void User::updatePosition(std::string symbol, position pos) noexcept {
  User::m_position[symbol].emplace_back(pos);
}
std::optional<std::vector<position>> User::findPosition(std::string symbol) {
  if (m_position.find(symbol) == m_position.end()) [[unlikely]] {
    return std::nullopt;
  }
  return m_position[symbol];
}
/**
  @brief User defines the order .i.e the price, symbol, bid, state and side.
         The order is then encoded for communication to the matching engine, for
  execution.
          Order is validated sent to the validation Engine
          Then communicated to the matching engine

  @return True when there is no errors
          false when unable to create the order

*/

bool User::executeOrder(Order::Order &order) noexcept { return true; }
