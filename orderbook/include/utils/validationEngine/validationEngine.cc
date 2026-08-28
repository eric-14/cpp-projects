#include "validationEngine.hpp"
#include <cstdint>
#include <sys/types.h>

#define DAYDURATION 24h

namespace MCORE = Market::core;
namespace MCORE_V = Market::core::Validation;
namespace MCORE_O = Market::core::Order;
using MCOREO_O = MCORE_O::Order;

std::expected<bool, SystemError::OrderValidationError>
MCORE_V::validationEngine::removeOrder(std::shared_ptr<Order::Order> order) {
  uint8_t isTrue = 0;
  auto result =
      order->updateOrderState(MCORE_O::OrderState::INVALID)
          .and_then([&](MCORE_O::OrderState state)
                        -> std::expected<bool, SystemError::OrderEntryError> {
            if (state == MCORE_O::OrderState::INVALID) [[likely]] {
              return isTrue |= 1;
            } else {
              return std::unexpected(
                  SystemError::OrderEntryError("Invalidation of Order Error"));
            }
          });
  return isTrue == 1;
}

bool MCORE_V::validationEngine::isOrderValid(
    std::shared_ptr<MCORE_O::Order> order) noexcept {
  /// Only necessary implementations
  /// no need to do further  validation if the previous check is wrong
  bool valid1 = isOrderAmount(order);
  bool valid2 = isOrderStateValid(order);
  return valid1 and valid2;
}

bool MCORE_V::validationEngine::isOrderStateValid(
    std::shared_ptr<MCORE_O::Order> order) noexcept {
  MCORE_O::OrderState state = order->getOrderState();
  /// CAN this be converted to bit AND operation
  /// It would be faster. Should be validated with benchmarks
  uint8_t isTrue = 0;
  if (state == MCORE_O::OrderState::PARTIALLY_FILLED ||
      state == MCORE_O::OrderState::VALID) [[likely]] {
    isTrue |= 1;
  } else [[unlikely]] {
    isTrue &= 0;
  }
  return isTrue == 1;
}

bool MCORE_V::validationEngine::isOrderAmount(
    std::shared_ptr<MCORE_O::Order> order) noexcept {
  double amount = order->getPrice();
  uint8_t isTrue = 0;
  if (amount > 0) {
    isTrue |= 1;
  }
  return isTrue == 1;
}
/**
  @brief Pass two orders to the function, one bid and one ask.
  The orders are matched to execute a trade.
  The function evaluates the state of both orders.
  and based on exchange rules decides whether the execution can be done

  @param order1 - BID
  @param order2 - ASK

  @return bool - True the match can be done
               - False the matching cannot be executed
*/
bool MCORE_V::validationEngine::isExecutionRequestReady(
    std::shared_ptr<MCORE_O::Order> order1,
    std::shared_ptr<MCORE_O::Order> order2) noexcept {

  MCORE::system_time_t order1Timestamp = order1->getTimeStamp();
  MCORE_O::OrderTimeFrame order1Timeframe = order1->getExecutionTimeFrame();
  MCORE::system_time_t order2Timestamp = order2->getTimeStamp();
  MCORE_O::OrderTimeFrame order2Timeframe = order2->getExecutionTimeFrame();
  uint8_t tmpFrame = 0;
  tmpFrame |= static_cast<uint8_t>(order1Timeframe);
  uint8_t acceptableMatch = static_cast<uint8_t>(MCORE_O::OrderTimeFrame::AON) |
                            static_cast<uint8_t>(MCORE_O::OrderTimeFrame::GTC) |
                            static_cast<uint8_t>(MCORE_O::OrderTimeFrame::IOC) |
                            static_cast<uint8_t>(MCORE_O::OrderTimeFrame::FOK);
  uint8_t resultState = 0;
  // std::println("The original amount is {}", order2->getOriginalAmount());
  if ((acceptableMatch & tmpFrame) == 1 and
      order1->getOriginalAmount() < order2->getOriginalAmount()) {
    resultState |= 1;
  }
  MCORE::system_time_t now = std::chrono::steady_clock::now();
  auto duration1 = now - order1Timestamp;
  auto duration2 = now - order2Timestamp;
  if (duration1 < std::chrono::hours(24) and
      duration2 < std::chrono::hours(24)) {
    resultState |= 1;
  }
  return resultState == 1;
}
