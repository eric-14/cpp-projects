

#include "include/synchronize.hpp"
#include <array>
#include <memory_resource>
// #include <vector>
std::array<char, 30'000> memory_resource2; 

int main()
{
    std::array<char, 30'000> memory_resource1; 
    //stack based
    std::pmr::monotonic_buffer_resource pool1(memory_resource1.data(), memory_resource1.size());
    //heap based 
    std::pmr::monotonic_buffer_resource pool2(memory_resource2.data(), memory_resource2.size());
    //multipool synchronized 


    //multipool unsynchronized 


    std::string name{"pmr resource allocator"};  
    debug_allocator alloc1{name, &pool1}; 
    
    debug_allocator alloc2{name, &pool2}; 

    std::pmr::vector<std::string> ints{&alloc1}; 

    ints.emplace_back("Hello world"); 
   // ints.emplace_back(40); 
}