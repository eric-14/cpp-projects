#include <string> 
#include <format>
#include <optional>
#include <expected>
#include "../error/error.hpp"

namespace Market::core {
using uint8_t = unsigned char;

namespace Order {

enum class OrderState : std:: uint8_t
{
    FILLED, 
    PARTIALLY_FILLED, 
    VALID, 
    INVALID, 
    REJECTED, 
    CANCELED, 
    PENDING 
}; 

enum class OrderDirection:std:: uint8_t
{
    BID, 
    ASK, 
}; 

enum class OrderType:std:: uint8_t
{
    LIMIT, 
    MARKET
}; 

enum class OrderTimeFrame :std::uint8_t 
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
    OrderState m_state;
    OrderType m_type; 
    OrderTimeFrame m_frame; 

    double m_price; 
    double m_amount; 
    double m_filledQuantity; 

    std::size_t m_timestamp; 
    // SSO 
    std::string m_symbol;   
    std::string m_Id; 
    std::string m_userId;  
    //Order is non copyable as it losses its information
    Order(Order &other) = delete; 
    Order operator=(Order &other) = delete; 
    //default move constructor suffices for the order 
    Order(Order &&other) = default; 
    Order& operator=(Order&&) = default; 
public: 
    Order(double price, double amount, 
          OrderState state, OrderType type, OrderTimeFrame timestamp, 
          std::string symbol, std::string Id, std::string userId
        ):
        m_state{state}, m_type{type}, m_frame{timestamp},
        m_price{price}, m_amount{amount}, 
        m_symbol{symbol}, m_Id{Id}, m_userId{userId}
    { }
    [[nodiscard]] 
    std::optional<OrderState> 
    updateOrderState(OrderState state) noexcept
    {
        m_state = state;  
        return m_state; 
    }

    [[nodiscard]] constexpr std::string_view getId() noexcept; 
    [[nodiscard]] constexpr std::string_view getUserId() noexcept; 
    [[nodiscard]] constexpr std::string_view getSymbol() noexcept; 
    [[nodiscard]] constexpr double getFilledQuantity() noexcept; 
    [[nodiscard]] constexpr double getOriginalAmount() noexcept; 
    [[nodiscard]] constexpr double getPrice() noexcept; 
    [[nodiscard]] constexpr OrderState getOrderState() noexcept; 
    [[nodiscard]] constexpr OrderDirection getOrderDirection() noexcept; 
    [[nodiscard]] constexpr OrderTimeFrame getExecutionTimeFrame() noexcept; 
    [[nodiscard]] constexpr OrderType getOrderType() noexcept; 
    [[nodiscard]] constexpr std::size_t getTimestamp() noexcept; 

    [[nodiscard]] 
    std::expected<double, SystemError::OrderFillLimitError> updateFillORder(double fillAmount); 

    
}; 
inline std::expected<std::string_view, SystemError::UndefinedState> state_tostring(OrderState state)
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
            return std::unexpected(SystemError::UndefinedState("Order state undefined")); 
    }
}

inline std::expected<std::string_view, SystemError::UndefinedState> ordertype_tostring(OrderType type)
{
    switch(type){
        case OrderType::LIMIT:  
            return "LIMIT"; 
        case OrderType::MARKET: 
            return "MARKET"; 
        default:    
            return std::unexpected(SystemError::UndefinedState("Order type undefined")); 
    }
}

inline std::expected<std::string_view, SystemError::UndefinedState> orderdirection_tostring(OrderDirection state)
{
    switch(state){
        case OrderDirection::BID:  
            return "BID"; 
        case OrderDirection::ASK: 
            return "ASK"; 
        default:    
            return std::unexpected(SystemError::UndefinedState("Order direction undefined")); 
    }
}

// function returns string representation of the order
inline std::optional<std::string_view> formatOrder(std::string_view output, Order &order)
{
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
                order.getId(),
                order.getUserId(), 
                order.getSymbol(), 
                order.getTimestamp(), 
                state_tostring(order.getOrderState()).value_or(""),
                ordertype_tostring(order.getOrderType()).value_or(""), 
                orderdirection_tostring(order.getOrderDirection()).value_or(""), 
                order.getPrice(), 
                order.getOriginalAmount(),
                order.getFilledQuantity()     
    ); 
}

} //namespace order 
} // namespace market core 

