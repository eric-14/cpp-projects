
#include <vector>
#include <print>
#include "./fifo5/spsc.hpp"
#include "workers.hpp"
#include <locale>
#include <chrono>
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
    std::size_t capacity = 26; 

    Fifo1<int,std::allocator<int>> fifo(capacity);

    std::println("value of capacity {}",fifo.capacity()); 

    auto start_time = std::chrono::steady_clock::now(); 
    Worker worker(fifo); 
    auto end_time = std::chrono::steady_clock::now(); 

    std::chrono::duration<double> elapsed = end_time - start_time; 
    std::size_t cap = fifo.capacity();  
    double nops = static_cast<double>(cap) / elapsed.count(); 

    std::println("----------------------------------------------------------------"); 

    std::println("Number of items into the queue {} ",
        std::format(std::locale("en_US.UTF-8"), "{:L}", 
        fifo.capacity()));
        
    std::println(" Time taken: {}",elapsed.count()); 

    std::println("Throughput {}",
        std::format(std::locale("en_US.UTF-8"),"{:.4L}", 
        nops)); 
    std::println("----------------------------------------------------------------"); 



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
