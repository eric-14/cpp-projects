#include "users.hpp"

namespace MCORE = Market::core;
namespace MCORE_U = Market::core::Users;

using namespace Market::core::Users;

bool User::withdrawAmount(double amount) noexcept { account_amount -= amount; }
bool User::depositAmount(double amount) noexcept { account_amount += amount; }
bool User::updatePosition(std::string symbol, position pos) noexcept {
  User::m_position[symbol].push_back(pos);
  return true;
}
