#include <memory_resource>
#include <benchmark/benchmark.h>
#include <array> 


#define NUM_ITERATIONS 50'000

//global memory resource 
// std::array<char, 30'000> memory_resource; 
static void BENCHMARK_vector(benchmark::State &state)
{
    // local memory resource 
    //stack based - ideally faster with risk of buffer overflow 
    std::array<char, 30'000> memory_resource; 
    
    std::pmr::monotonic_buffer_resource pool{memory_resource.data(), memory_resource.size()}; 

    std::pmr::vector<char> vec{&pool}; 


    for(auto _ : state) {
        std::vector<char> vec; 
        
        vec.reserve(NUM_ITERATIONS); 
        for(std::size_t i{0}; i < NUM_ITERATIONS; ++i )
        {
            // std::string tmp = generate_random_string(i); 
            vec.emplace_back(i); 
        }
    }
   
}

BENCHMARK(BENCHMARK_vector); 
BENCHMARK_MAIN(); 