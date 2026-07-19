#include <benchmark/benchmark.h>
#include <vector> 
#include <string> 
#include <random>

#define NUM_ITERATIONS 50
std::string generate_random_string(std::size_t length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    // 1. Initialize random engine with a non-deterministic seed
    std::random_device rd;
    std::mt19937 generator(rd());
    
    // 2. Map random numbers uniformly to our character index range
    std::uniform_int_distribution<std::size_t> distribution(0, characters.size() - 1);
    
    // 3. Pre-allocate string memory for speed
    std::string random_string;
    random_string.reserve(length);
    
    for (std::size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }
    
    return random_string;
}

static void BENCHMARK_vector(benchmark::State &state)
{
    for(auto _ : state) {
        std::vector<std::string> vec; 
        vec.reserve(NUM_ITERATIONS); 
        for(std::size_t i{0}; i < NUM_ITERATIONS; ++i )
        {
            std::string tmp = generate_random_string(i); 
            vec.push_back(tmp); 
        }
    }
   
}

BENCHMARK(BENCHMARK_vector); 
BENCHMARK_MAIN(); 