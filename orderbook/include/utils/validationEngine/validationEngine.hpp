/**
    validation Engine can be used to mark Orders in memory as erroneous
    By having this feature, time to kill of orders can be implemented

    The Orders have to have similar class layout in memory.
    Throughout the execution of this program

*/
#ifndef __VALIDATION_ENGINE__
#define __VALIDATION_ENGINE__
#include "../../order/order.hpp"
// #include "../../orderbook/orderbook.hpp"
#include <string>
namespace Market::core {

#define DAYORDDERHOURS 24
namespace Validation {

class validationEngine {
public:
  validationEngine(std::string name, std::string Id, system_time_t tm)
      : m_name{name}, m_Id{Id}, m_tm{tm} {}
  ~validationEngine() {}
  // Pass the memory address of the Order
  std::expected<bool, SystemError::OrderValidationError>
  removeOrder(std::shared_ptr<Order::Order> Order);

  /// Check Order state if not invalid
  /// This is close to spaghetti code.
  /// However the above implementation has an advantage in a hot path
  /// As only necessary implementation are conducted
  /// no need to do the 3rd validation if the first evaluation is erroneous
  [[nodiscard]] bool isOrderValid(std::shared_ptr<Order::Order> order) noexcept;
  [[nodiscard]] bool
  isOrderStateValid(std::shared_ptr<Order::Order> order) noexcept;
  [[nodiscard]] bool
  isOrderAmount(std::shared_ptr<Order::Order> order) noexcept;
  [[nodiscard]] bool
  isExecutionRequestReady(std::shared_ptr<Order::Order> order1,
                          std::shared_ptr<Order::Order> order2) noexcept;

  constexpr std::string_view getName() noexcept { return m_name; };
  constexpr std::string_view getId() const noexcept { return m_Id; };
  constexpr system_time_t getTimestamp() noexcept { return m_tm; };

private:
  //   constexpr bool isOrderValid(std::shared_ptr<Order::Order> order)
  //   noexcept;

  std::string m_name; // name of the validation engine
  std::string m_Id;
  system_time_t m_tm;
};

} // namespace Validation
} // namespace Market::core

#endif