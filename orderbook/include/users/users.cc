#include "users.hpp"

namespace MCORE = Market::core;
namespace MCORE_U = Market::core::Users;

using namespace Market::core::Users;

bool User::withdrawAmount(double amount) noexcept {
  if (account_amount < 0) [[unlikely]] {
    return false;
  }
  account_amount -= amount;
  return true;
}
void User::depositAmount(double amount) noexcept { account_amount += amount; }
bool User::updatePosition(std::string symbol, position pos) noexcept {
  User::m_position[symbol].push_back(pos);
  return true;
}
