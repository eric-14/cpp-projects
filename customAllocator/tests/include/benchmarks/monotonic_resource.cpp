#include <memory_resource> 
//#include <print> 
#include <vector> 
#include <array> 

static void vectorGlobalAllocator(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> vec{1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10}; 
    // Make sure the variable is not optimized away by compiler
   // benchmark::DoNotOptimize(vec);
  }
}


// Register the function as a benchmark
BENCHMARK(vectorGlobalAllocator);

static void vectorPMRGlobalAllocator(benchmark::State& state) {
   for (auto _ : state) {
    std::array<char, 200> buffer; 
    std::pmr::monotonic_buffer_resource alloc(buffer.data(), buffer.size());
    
    std::pmr::vector<int> vec2{{1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10}, &alloc}; 
    
    benchmark::DoNotOptimize(vec2.data());
    benchmark::ClobberMemory();
  }
}
// Register the function as a benchmark
BENCHMARK(vectorPMRGlobalAllocator);

static void vectorPMRGlobalAllocator_Reset(benchmark::State& state) {
  std::array<char, 200> buffer; 
  std::pmr::monotonic_buffer_resource alloc(buffer.data(), buffer.size());
  
  for (auto _ : state) {
    std::pmr::vector<int> vec2{{1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10}, &alloc}; 
    benchmark::DoNotOptimize(vec2.data());
    benchmark::ClobberMemory();
    
    alloc.release();
  }
}
BENCHMARK(vectorPMRGlobalAllocator_Reset);