#define BENCHMARK_STATIC_DEFINE
#include <benchmark/benchmark.h>
#include <vector> 

#define NUM_ITERATIONS 1'000'000

static void BENCHMARK_vector(benchmark::State &state)
{
    for(auto _ : state) {
        std::vector<int> vec; 
        vec.reserve(NUM_ITERATIONS); 
        for(std::size_t i{0};i < NUM_ITERATIONS; ++i )
        {
            vec.emplace_back(i); 
        }
    }
   
}

BENCHMARK(BENCHMARK_vector); 
BENCHMARK_MAIN(); 