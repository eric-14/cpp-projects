#include "../error/error.hpp" 
#include <vector> 
class Orderbook 
{
    public: 
        [[nodiscard]] 
        std::expected<bool, SystemError::OrderEntryError> 
            addOrder(shared_ptr<Order> Order) noexcept; 

        [[nodiscard]] 
        std::expected<bool, SystemError::OrderEntryError> 
            removeOrder(shared_ptr<Order> Order) noexcept; 
    
        double getBestAsk() noexcept; 
        double getBestBid() noexcept; 
        double getSpread() noexcept; 

        [[nodiscard]]
        constexpr std::flatmap<double, std::vector<shared_ptr<Order>>& 
        getAsks() noexcept; 
        [[nodiscard]]
        constexpr std::flatmap<double, std::vector<shared_ptr<Order>>& 
        getBids() noexcept; 

    private: 
        std::string Id; 
        std::timestamp tm;
        // C++23 flatmap implementation
        // cache friendly 
        std::flatmap<double, std::vector<shared_ptr<Order>> m_asks; 
        std::flatmap<double, std::vector<shared_ptr<Order>> m_bids; 
}