#ifndef __ORDER__
#define __ORDER__
#include "../error/error.hpp"
#include <chrono>
#include <expected>
#include <format>
#include <optional>
#include <print>
#include <string>

namespace Market::core {

using uint8_t = unsigned char;
using system_time_t = std::chrono::steady_clock::time_point;

namespace Order {

enum class OrderState : std::uint8_t {
  FILLED,
  PARTIALLY_FILLED,
  VALID,
  INVALID,
  REJECTED,
  CANCELED,
  PENDING
};

enum class OrderDirection : std::uint8_t {
  BID,
  ASK,
};

enum class OrderType : std::uint8_t { LIMIT, MARKET };

enum class OrderTimeFrame : std::uint8_t {
  DAYORDER, // Default order as per the SEC
  GTC,      // Good till cancelled
  IOC,      // Immediate or canceled. Any portion not filled should be killed
  FOK,      // Fill or kill. Must be executed immediately in its entirety
  AON       // All or none
};

class Order {

private:
  // order attributes
  OrderState m_state;
  OrderType m_type;
  OrderTimeFrame m_frame;
  OrderDirection m_direction;

  double m_price;
  double m_amount;
  double m_filledQuantity;

  system_time_t m_timestamp;
  // SSO
  std::string m_symbol;
  std::string m_Id;
  std::string m_userId;

public:
  explicit Order(double price, double amount, OrderState state, OrderType type,
                 OrderDirection direction, OrderTimeFrame timeframe,
                 std::string symbol, std::string Id, std::string userId)
      : m_state{state}, m_type{type}, m_frame{timeframe},
        m_direction{direction}, m_price{price}, m_amount{amount},
        m_symbol{symbol}, m_Id{Id}, m_userId{userId} {
    m_timestamp = std::chrono::steady_clock::now();
  }
  // Order is non copyable as it losses its information
  Order(Order &other) = delete;
  Order operator=(Order &other) = delete;
  // default move constructor suffices for the order
  Order(Order &&other) = default;
  Order &operator=(Order &&) = default;

  [[nodiscard]]
  std::optional<OrderState> updateOrderState(OrderState state) noexcept {
    m_state = state;
    return m_state;
  }

  [[nodiscard]] constexpr std::string_view getId() const noexcept {
    return m_Id;
  }

  [[nodiscard]] constexpr std::string_view getUserId() const noexcept {
    return m_userId;
  }

  [[nodiscard]] constexpr std::string_view getSymbol() const noexcept {
    return m_symbol;
  }

  [[nodiscard]] constexpr double getFilledQuantity() const noexcept {
    return m_filledQuantity;
  }
  [[nodiscard]] constexpr double getOriginalAmount() const noexcept {
    return m_amount;
  }

  [[nodiscard]] constexpr double getPrice() const noexcept { return m_price; }

  [[nodiscard]] constexpr system_time_t getTimeStamp() const noexcept {
    return m_timestamp;
  }

  [[nodiscard]] constexpr OrderTimeFrame
  getExecutionTimeFrame() const noexcept {
    return m_frame;
  }

  [[nodiscard]] constexpr OrderType getOrderType() const noexcept {
    return m_type;
  }

  [[nodiscard]] constexpr OrderState getOrderState() const noexcept {
    return m_state;
  }
  [[nodiscard]] constexpr OrderDirection getOrderDirection() const noexcept {
    return m_direction;
  }

  std::expected<double, SystemError::OrderFillLimitError>
  updateFillOrder(double fillAmount);

  inline std::expected<std::string_view, SystemError::UndefinedState>
  stateToString() {
    switch (m_state) {
    case OrderState::FILLED:
      return "FILLED";
    case OrderState::PARTIALLY_FILLED:
      return "PARTIALLY_FILLED";
    case OrderState::VALID:
      return "VALID";
    case OrderState::INVALID:
      return "INVALID";
    case OrderState::REJECTED:
      return "REJECTED";
    case OrderState::CANCELED:
      return "CANCELED";
    case OrderState::PENDING:
      return "PENDING";
    default:
      return std::unexpected(
          SystemError::UndefinedState("Order state undefined"));
    }
  }

  inline std::expected<std::string_view, SystemError::UndefinedState>
  TypeToString() {
    switch (m_type) {
    case OrderType::LIMIT:
      return "LIMIT";
    case OrderType::MARKET:
      return "MARKET";
    default:
      return std::unexpected(
          SystemError::UndefinedState("Order type undefined"));
    }
  }
  inline std::expected<std::string_view, SystemError::UndefinedState>
  directionToString() {
    switch (m_direction) {
    case OrderDirection::BID:
      return "BID";
    case OrderDirection::ASK:
      return "ASK";
    default:
      return std::unexpected(
          SystemError::UndefinedState("Order direction undefined"));
    }
  }
};

// function returns string representation of the order
inline std::optional<std::string_view> formatOrder(std::string_view output,
                                                   Order &order) {
  return std::format(
      R"(Order-ID: {} \n 
                Order-UserID: {} \n
                Order-Symbol: {} \n 
                Order TimeStamp {} \n 
                Order-State {} \n 
                Order-Type {} \n 
                Order-Direction {} \n
                Order-Price {} \n 
                Order-Amount {} \n 
                Order-FilledAmount {} \n)",
      order.getId(), order.getUserId(), order.getSymbol(),
      order.getTimestamp().time_since_epoch(),
      order.stateToString().value_or(""), order.TypeToString().value_or(""),
      order.directionToString().value_or(""), order.getPrice(),
      order.getOriginalAmount(), order.getFilledQuantity());
}

} // namespace Order
} // namespace Market::core
#endif