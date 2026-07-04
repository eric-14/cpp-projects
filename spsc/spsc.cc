
#include <vector>
#include <print>
#include "spsc.hpp"
#include "workers.hpp"

// erors to include 
// -werror -wextra -wconversion address sanitizer undefined behaviour santizer 
// thread sanitizer 
// writing tests -gtest 
// performance testing 
// add values from one thread 
//read the values from another thread 

int main()
{
    std::println("SPSC starting...."); 
    std::size_t capacity = 100; 

    Fifo1<int,std::allocator<int>> fifo(capacity);
    Worker worker(fifo); 

    // std::vector<int> values{1,2,3,4,5,6,7,8,9,10}; 

    // for(std::size_t i{0}; i < values.size(); ++i)
    // {
    //     fifo.push(values[i]);  
    // }

    // while(!fifo.empty())
    // {
    //     int tmp ; 
    //     bool complete = fifo.pop(tmp);
    //     std::size_t size = fifo.size(); 
    //     std::println("value from pop is {} state of buffer {} size {}",tmp, complete, size); 
    // }
}
