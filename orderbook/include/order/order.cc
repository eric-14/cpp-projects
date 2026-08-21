#include "order.hpp"

namespace M_CORE_O = Market::core::Order;

std::expected<double, SystemError::OrderFillLimitError>
M_CORE_O::Order::updateFillOrder(double fillAmount) {

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
