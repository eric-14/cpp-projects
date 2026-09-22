#include <cstdint>

const uint16_t BID_LOWER = 1 << 1;
const uint16_t ORDER_FILLED = 1 << 2;
const uint16_t INVALID_USER_AMOUNT = 1 << 3;
const uint16_t INVALID_ASK_PRICE = 1 << 4;
const uint16_t ORDER_ERROR = 1 << 5;
const uint16_t PARTIALLY_FILLED_ORDER = 1 << 6;
const uint16_t SYMBOL_OR_MARKET_ERROR = 1 << 7;