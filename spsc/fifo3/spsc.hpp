#pragma once 
#include <cassert>
#include <atomic>
#include <memory>
#include <new>

template <typename T, typename Alloc = std::allocator<T>> 
class Fifo1 : private Alloc 
{
    std::size_t capacity_; 
    T* ring_; 
    alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> pushCursor_{}; 
    alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> popCursor_{}; 
    
    char padding[std::hardware_destructive_interference_size - sizeof(std::size_t)]; 
    //padding to avoid false sharing with adjacent objectz
    static_assert(std::atomic<std::size_t>::is_always_lock_free,"Cursors must be atomic"); 

    public: 
        explicit Fifo1(std::size_t capacity, Alloc const& alloc = Alloc{}) 
            : Alloc(alloc), capacity_(capacity), ring_{std::allocator_traits<Alloc>::allocate(*this, capacity)}
        {}
        ~Fifo1(){
            while(not empty()){
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
        auto size() const {
            assert(popCursor_ <= pushCursor_); 
            return pushCursor_ - popCursor_; 
        };
        auto empty() const { return size() == 0; }
        auto full() const {return size() == capacity(); }

        auto push(T const& value)
        {
            if(full()){
                return false; 
            }
            new(&ring_[pushCursor_ % capacity_]) T(value); 
            ++pushCursor_; 
            return true; 
        }
        auto pop(T& value)
        {
            if(empty())
            {
                return false; 
            }
            value = ring_[popCursor_ % capacity_]; 
            ring_[popCursor_ % capacity_].~T(); 
            ++popCursor_; 
            return true; 
        }

}; 

