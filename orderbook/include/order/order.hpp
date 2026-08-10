#include <string> 

enum class orderState : std:: uint8_t
{
    VALID, 
    INVALID, 
}; 

enum class ordedrDirection : std:: uint8_t
{
    BUY, 
    ASK, 
}; 


class Order {
    double price; 
    double amount; 
    double filledQuantity; 
    orderState state;
    std::string symbol;   
    std::string m_Id; 
    std::string m_UserId;

    Order(std::string symbol, double price, double amount, orderState state) :
    price{price},  amount{amount}, symbol{symbol}, state{state}
    { }
    //Order is non copyable as it losses its information
    Order(Order &other) = delete; 
    Order operator=(Order &other) = delete; 

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