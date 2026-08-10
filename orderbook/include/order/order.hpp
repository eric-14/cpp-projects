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
    [[nodiscard]] const std::string_view getId() noexcept; 
    [[nodiscard]] const std::string_view getUserId() noexcept; 
    [[nodiscard]] const double getFilledQuantity() noexcept; 
    [[nodiscard]] const double getOriginalAmount() noexcept; 
}; 