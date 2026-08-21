#include "order.hpp"

namespace M_CORE_O = Market::core::Order;
[[nodiscard]] constexpr std::string_view
M_CORE_O::Order::getId() const noexcept {
  return m_Id;
}

[[nodiscard]] constexpr std::string_view
M_CORE_O::Order::getUserId() const noexcept {
  return m_userId;
}

[[nodiscard]] constexpr std::string_view
M_CORE_O::Order::getSymbol() const noexcept {
  return m_symbol;
}

[[nodiscard]] constexpr double
M_CORE_O::Order::getFilledQuantity() const noexcept {
  return m_filledQuantity;
}
[[nodiscard]] constexpr double
M_CORE_O::Order::getOriginalAmount() const noexcept {
  return m_amount;
}

[[nodiscard]] constexpr double M_CORE_O::Order::getPrice() const noexcept {
  return m_price;
}

[[nodiscard]] constexpr Market::core::system_time_t
M_CORE_O::Order::getTimestamp() const noexcept {
  return m_timestamp;
}

[[nodiscard]] constexpr M_CORE_O::OrderTimeFrame
M_CORE_O::Order::getExecutionTimeFrame() const noexcept {
  return m_frame;
}

[[nodiscard]] constexpr M_CORE_O::OrderType
M_CORE_O::Order::getOrderType() const noexcept {
  return m_type;
}

[[nodiscard]] constexpr M_CORE_O::OrderState
M_CORE_O::Order::getOrderState() const noexcept {
  return m_state;
}

[[nodiscard]]
std::expected<double, SystemError::OrderFillLimitError>
M_CORE_O::Order::updateFillORder(double fillAmount) {

  if (m_amount > 0.0 && m_amount >= fillAmount) {
    m_amount = m_amount - fillAmount;
    if (m_amount == 0.0) {
      m_state = OrderState::FILLED;
    } else {
      m_state = OrderState::PARTIALLY_FILLED;
    }
    return m_amount;
  }
  return std::unexpected(
      SystemError::OrderFillLimitError("Update fill Order Error"));
  /// Implementation is determined from the x86 compiler output
  /// The validation above results in xor compare , sub and store
  /// A total of 12 assembly instructions
  /// See compiler explorer https://godbolt.org/z/jMbjGsY44
}
