#pragma once 
#include <cassert>
#include <atomic>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>> 
class Fifo1 : private Alloc 
{
    std::size_t capacity_; 
    T* ring_; 
    char padding_[std::hardware_destructive_interference_size - sizeof(std::size_t)]; 
    alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> pushCursor_{}; 
    alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> popCursor_{}; 
    
    //padding to avoid false sharing with adjacent object 
    char padding1_[std::hardware_destructive_interference_size - sizeof(std::size_t)]; 
    static_assert(std::atomic<std::size_t>::is_always_lock_free,"Cursors must be atomic"); 

    alignas(std::hardware_destructive_interference_size) std::size_t cachedPushCursor{}; 
    alignas(std::hardware_destructive_interference_size) std::size_t cachedPopCursor{}; 

    public: 
        explicit Fifo1(std::size_t capacity, Alloc const& alloc = Alloc{}) 
            : Alloc(alloc), capacity_(capacity), ring_{std::allocator_traits<Alloc>::allocate(*this, capacity)}
        {}
        ~Fifo1(){
            while(not empty(pushCursor_,popCursor_)){
                ring_[popCursor_ % capacity_].~T(); 
                ++popCursor_; 
            }
            std::allocator_traits<Alloc>::deallocate(*this, ring_, capacity_); 
        }

        Fifo1(Fifo1 const&) = delete; 
        Fifo1& operator=(Fifo1 const&) = delete; 
        Fifo1(Fifo1 &&) = delete; 
        Fifo1& operator=(Fifo1&&) = delete; 

        auto capacity() const {return capacity_;}
        auto size(std::size_t pushCursor, std::size_t popCursor) const {
            assert(popCursor_ <= pushCursor_); 
            return pushCursor_ - popCursor_; 
        };

        auto empty(std::size_t pushCursor, std::size_t popCursor) const { return size(pushCursor,popCursor ) == 0; }
        auto full(std::size_t pushCursor, std::size_t popCursor) const {return size(pushCursor, popCursor) == capacity(); }

        auto push(T const& value)
        {
            auto pushCursor = pushCursor_.load(std::memory_order_relaxed); 
            // auto popCursor = popCursor_.load(std::memory_order_acquire); 

            if(full(pushCursor, cachedPopCursor)){
                cachedPopCursor =  popCursor_.load(std::memory_order_acquire); 
                if(full(pushCursor, cachedPopCursor)){
                    return false; 
                }
            }
            new(&ring_[pushCursor_ % capacity_]) T(value); 
            pushCursor_.store(pushCursor + 1, std::memory_order_release);  
            return true; 
        }
        auto pop(T& value)
        {
            auto popCursor = popCursor_.load(std::memory_order_relaxed); 
            if(empty(cachedPushCursor,popCursor))
            {
                cachedPushCursor = pushCursor_.load(std::memory_order_acquire); 
                if(empty(cachedPushCursor, popCursor))
                {
                    return false; 
                }
            }
            value = ring_[popCursor_ % capacity_]; 
            ring_[popCursor_ % capacity_].~T(); 
            popCursor_.store(popCursor + 1, std::memory_order_release); 
            return true; 
        }

}; 

