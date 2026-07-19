
#include <memory_resource>
#include <print> 
#include <string>
#include "synchronize.hpp"


debug_allocator::debug_allocator(std::string name, std::pmr::memory_resource * up = std::pmr::get_default_resource()) : 
                        name{name}, _upstream{up} {}

debug_allocator::~debug_allocator(){
    std::println("[Deallocate] class destructor called"); 
} 

void* debug_allocator::do_allocate(std::size_t bytes,std::size_t alignment) 
{
    std::println("[Allocator]  bytes allocated {} alignment {}",bytes, alignment); 
    void * allocated = _upstream->allocate(bytes, alignment); 
    return allocated; 
}

void debug_allocator::do_deallocate(void *ptr, std::size_t bytes, std::size_t alignment) 
{
    std::println("[Deallocator]  bytes allocated {} alignment {}",bytes, alignment); 
    _upstream->deallocate(ptr, bytes, alignment); 
}   
bool debug_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept
{
    return this == &other; 
}

