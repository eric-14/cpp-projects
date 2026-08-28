#include "../../include/orderbook/orderbook.hpp"
#include "../../include/order/order.hpp"
#include <chrono>
#include <cstddef>
#include <memory>
#include <print>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Testing OrderBook", "OrderBook") {
  using namespace Market::core::Trading;
  using namespace Market::core::Order;

  auto timestamp = std::chrono::steady_clock::now();

  std::string userId = "user-1";
  std::string orderBookID = "OrderBook1-v1-time";
  std::string symbol = "AAPL";
  std::string orderID = "Order1-v1-time";

  Orderbook book1(orderBookID, symbol, timestamp);

  symbol = "AAPL";
  userId = "user-1";
  std::shared_ptr<Order> order3 = std::make_shared<Order>(
      30.0, 100.0, OrderState::VALID, OrderType::LIMIT, OrderSide::BID,
      OrderTimeFrame::GTC, symbol, orderID, userId);
  symbol = "GOOGL";
  userId = "user-1";
  orderID = "Order1-v1-time";
  std::shared_ptr<Order> order4 = std::make_shared<Order>(
      30.0, 100.0, OrderState::VALID, OrderType::LIMIT, OrderSide::BID,
      OrderTimeFrame::GTC, symbol, orderID, userId);

  //   book1.printLayout();

  REQUIRE(book1.getId() == orderBookID);
  REQUIRE(book1.getTimestamp() == timestamp);
  REQUIRE(book1.getSymbol() == "AAPL");

  SECTION("Book check Invalid state") {
    CHECK(book1.getId() == orderBookID);
    CHECK(book1.getBestAsk() == 0.0);
    CHECK(book1.getBestBid() == 0.0);
    CHECK(book1.getSpread() == 0.0);
  }

  SECTION("Orderbook add Order") {
    CHECK(book1.addOrder(order3) == true);
    CHECK(book1.addOrder(order4).value_or(false) == false);
  }
}
