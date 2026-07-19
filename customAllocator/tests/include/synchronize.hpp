#include <memory>

#ifndef DEBUG_ALLOCATOR 
#define DEBUG_ALLOCATOR 


class debug_allocator :public std::pmr::memory_resource 
{
    public:

    explicit debug_allocator(std::string name, std::pmr::memory_resource *upstream); 
    ~debug_allocator(); 


    void* do_allocate(std::size_t bytes, std::size_t alignment) override; 
    void do_deallocate(void* memory, std::size_t bytes, std::size_t alignment) override; 

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override ; 


    private: 
        std::string name; 
        std::pmr::memory_resource *_upstream; 
}; 

#endif