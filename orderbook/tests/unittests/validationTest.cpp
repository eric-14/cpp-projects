#include "../../include/order/order.hpp"
#include "../../include/utils/validationEngine/validationEngine.hpp"
#include <chrono>
#include <cstddef>
#include <memory>
#include <print>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

TEST_CASE("Testing Validation", "ValidationEngine") {
  using namespace Market::core::Validation;
  using namespace Market::core::Order;
  auto timestamp = std::chrono::steady_clock::now();

  std::string Id = "validation1";
  std::string name = "validationEngine1";

  validationEngine VE(name, Id, timestamp);

  REQUIRE(VE.getId() == Id);
  REQUIRE(VE.getTimestamp() == timestamp);
  REQUIRE(VE.getName() == name);

  std::string symbol = "AAPL";
  std::string userId = "user-1";
  std::string orderID = "Order1-v1-time";
  auto order1 = std::make_shared<Order>(
      30.0, 100.0, OrderState::VALID, OrderType::LIMIT, OrderSide::BID,
      OrderTimeFrame::FOK, symbol, orderID, userId);
  auto order2 = std::make_shared<Order>(
      30.0, -50.0, OrderState::VALID, OrderType::LIMIT, OrderSide::BID,
      OrderTimeFrame::GTC, symbol, orderID, userId);

  SECTION("Validate") {
    CHECK(VE.isOrderValid(order1) == true);
    order1->updateOrderState(OrderState::INVALID);
    CHECK(VE.isOrderValid(order1) == false);
    order1->updateOrderState(OrderState::REJECTED);
    CHECK(VE.isOrderValid(order1) == false);
    order1->updateOrderState(OrderState::CANCELED);
    CHECK(VE.isOrderValid(order1) == false);
    CHECK(VE.isExecutionRequestReady(order1, order2) == true);
  }
}
