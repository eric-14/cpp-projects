#include <cstdint>
#include <type_traits>
/// Bit 0 is not used. It can be used to debug initialization errors and or
/// undefined state system bugs

enum class STATEBITS : uint16_t {
  NC = 1 << 0,
  BID_LOWER = 1 << 1,
  ORDER_FILLED = 1 << 2,
  INVALID_USER_AMOUNT = 1 << 3,
  INVALID_ASK_PRICE = 1 << 4,
  ORDER_ERROR = 1 << 5,
  PARTIALLY_FILLED_ORDER = 1 << 6,
  SYMBOL_OR_MARKET_ERROR = 1 << 7
};

inline STATEBITS operator|(STATEBITS lhs, STATEBITS rhs) {
  using stateType = std::underlying_type_t<STATEBITS>;
  return static_cast<STATEBITS>(static_cast<stateType>(lhs) |
                                static_cast<stateType>(rhs));
}
inline STATEBITS operator&(STATEBITS lhs, STATEBITS rhs) {
  using stateType = std::underlying_type_t<STATEBITS>;
  return static_cast<STATEBITS>(static_cast<STATEBITS>(lhs) &
                                static_cast<STATEBITS>(rhs));
}