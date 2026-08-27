#include "../../include/order/order.hpp"
#include <chrono>
#include <cstddef>
#include <print>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Testing Order", "Order") {
  using namespace Market::core::Order;

  auto timestamp = std::chrono::steady_clock::now();

  std::string symbol = "AAPL";
  std::string userId = "user-1";
  std::string orderID = "Order1-v1-time";
  Order order1(30.0, 100.0, OrderState::VALID, OrderType::LIMIT, OrderSide::BID,
               OrderTimeFrame::GTC, symbol, orderID, userId);

  order1.printLayout();

  REQUIRE(order1.getId() == "Order1-v1-time");
  REQUIRE(order1.getPrice() == 30.0);
  REQUIRE(order1.getOriginalAmount() == 100);
  REQUIRE(order1.getSymbol() == "AAPL");

  REQUIRE(order1.getFilledQuantity() == 0.0);
  REQUIRE(order1.getOrderType() == OrderType::LIMIT);
  REQUIRE(order1.getOrderState() == OrderState::VALID);
  REQUIRE(order1.getOrderSide() == Market::core::Order::OrderSide::BID);
  REQUIRE(order1.getOrderType() == OrderType::LIMIT);
  REQUIRE(order1.getExecutionTimeFrame() ==
          Market::core::Order::OrderTimeFrame::GTC);
  REQUIRE(order1.getTimeStamp() > timestamp);

  SECTION("Update Order State") {
    CHECK(order1.updateOrderState(OrderState::INVALID) == OrderState::INVALID);
  }
  SECTION("Order State to String") {
    order1.updateOrderState(OrderState::INVALID);
    CHECK(std::string(order1.stateToString().value()) == "INVALID");
    order1.updateOrderState(OrderState::VALID);
    CHECK(std::string(order1.stateToString().value()) == "VALID");
    order1.updateOrderState(OrderState::REJECTED);
    CHECK(std::string(order1.stateToString().value()) == "REJECTED");
    order1.updateOrderState(OrderState::FILLED);
    CHECK(std::string(order1.stateToString().value()) == "FILLED");
    order1.updateOrderState(OrderState::CANCELED);
    CHECK(std::string(order1.stateToString().value()) == "CANCELED");
    order1.updateOrderState(OrderState::PARTIALLY_FILLED);
    CHECK(std::string(order1.stateToString().value()) == "PARTIALLY_FILLED");
    // order1.updateOrderState(OrderState::PENDING);
    // CHECK(std::string(order1.stateToString().value()) == "PENDING");
  }
  SECTION("Order Update Fill Order") {
    // check if filling order behaves as expected
    CHECK(order1.updateFillOrder(20.0).value_or(-1) == 80);
    CHECK(order1.stateToString() == "PARTIALLY_FILLED");
    CHECK(order1.updateFillOrder(80.0).value_or(-1) == 0.0);
    CHECK(order1.stateToString() == "FILLED");
    // Check false state of updating the order state
    CHECK(order1.updateFillOrder(10.0).value_or(-1) == -1);
  }
  SECTION("Order Type") {
    // check Order Type to String
    CHECK(order1.TypeToString() == "LIMIT");
  }
  SECTION("Order Direction") {
    // check Order Type to String
    CHECK(order1.TypeToString() == "LIMIT");
  }
}
