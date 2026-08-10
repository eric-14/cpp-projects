#include <string> 
#include "../error/error.hpp"
namespace Market::core {

namespace Order {

enum class OrderState 
    :std:: uint8_t
{
    FILLED, 
    PARTIALLY_FILLED, 
    VALID, 
    INVALID, 
    REJECTED, 
    CANCELED, 
    PENDING 
}; 

enum class OrderDirection 
    :std:: uint8_t
{
    BID, 
    ASK, 
}; 

enum class OrderType 
    :std:: uint8_t
{
    LIMIT, 
    MARKET
}; 

enum class OrderTimeFrame 
    :std::unit8_t 
{
    DAYORDER, // Default order as per the SEC 
    GTC,  // Good till cancelled 
    IOC,   // Immediate or canceled. Any portion not filled should be killed 
    FOK,   // Fill or kill. Must be executed immediately in its entirety 
    AON    // All or none 
}; 

class Order 
{

private: 
   // order attributes 
    OrderState state;
    OrderType m_type; 
    OrderTimeFrame m_frame; 

    double price; 
    double amount; 
    double filledQuantity; 

    std::size_t timestamp; 
    // SSO 
    std::string symbol;   
    std::string m_Id; 
    std::string m_UserId;  
    //Order is non copyable as it losses its information
    Order(Order &other) = delete; 
    Order operator=(Order &other) = delete; 
    //default move constructor suffices for the order 
    Order(Order &&other) = default; 
    Order operator=(Order&& other) = default; 
public: 
    Order(double price, double amount, 
          OrderState state, OrderType type, OrderTimeFrame timestamp, 
          std::string symbol, std::string Id, std::string userId
        ):
        price{price},  amount{amount}, 
        state{state}, m_frame{timestamp}, 
        symbol{symbol}, m_Id{Id}, m_UserId{userId}
    { }
    [[nodiscard]] 
    std::optional<orderState> 
    updateOrderState(orderState state) noexcept
    {
        state = state;  
        return state; 
    }

    [[nodiscard]] constexpr std::string_view getId() noexcept; 
    [[nodiscard]] constexpr std::string_view getUserId() noexcept; 
    [[nodiscard]] constexpr std::string_view getSymbol() noexcept; 
    [[nodiscard]] constexpr double getFilledQuantity() noexcept; 
    [[nodiscard]] constexpr double getOriginalAmount() noexcept; 
    [[nodiscard]] constexpr double getPrice() noexcept; 
    [[nodiscard]] constexpr OrderState getOrderState() noexcept; 
    [[nodiscard]] constexpr OrderType getOrderType() noexcept; 
    [[nodiscard]] constexpr std::size_t getTimestamp() noexcept; 

    
}; 
inline std::expected<std::string_view, SystemError> state_tostring(OrderState state)
{
    switch(state)
    {
        case OrderState::FILLED: 
            return "FILLED"; 
        case OrderState::PARTIALLY_FILLED: 
            return "PARTIALLY_FILLLED"; 
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
            std::unexpected(SystemError::UndefinedState("order state undefined")); 
    }
}

// allocate 
inline std::optional<bool> forrmatOrder(std::string_view output, Order &order)
{
    std::format("Order-ID: {} \n 
                Order-UserID: {} \n
                Order-Symbol: {} \n 
                Order TimeStamp {} \n 
                Order-State {} \n 
                Order-Type {} \n 
                Order-Price {} \n 
                Order-Amount {} \n 
                Order-FilledAmount {} \n", 
                order.getId(),
                order.getUserId(), 
                order.getSymbol(), 
                order.getTimestamp(), 
                order.getOrderState(), 

    
    
    )
}

} //namespace order 
} // namespace market core 

