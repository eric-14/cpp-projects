#include "users.hpp"

namespace MCORE = Market::core;
namespace MCORE_U = Market::core::Users;

namespace Market::core {
namespace Users {

bool User::withdrawAmount(double amount) noexcept { account_amount -= amount; }
bool User::depositAmount(double amount) noexcept { account_amount += amount; }

} // namespace Users
} // namespace Market::core