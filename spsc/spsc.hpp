#pragma once 
#include <cassert>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>> 
class Fifo1 : private Alloc 
{
    std::size_t capacity_; 
    T* ring_; 
    std::size_t pushCursor_{}; 
    std::size_t popCursor_{}; 


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
        auto pop(T* value)
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

