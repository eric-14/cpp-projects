#include "spsc.hpp"

auto Fifo1::push(T const& value)
{
    if(full()){
        return false; 
    }
    new(&ring_[pushCursor_ % capacity_]) T(value); 
    ++pushCursor_; 
    return true; 
}
autp Fifo1(T& value)
{
    if(emoty())
    {
        return false; 
    }
    value = ring_[popCursor_ % capacity_]; 
    ring_[popCursor_ % capacity_].~T(); 
    ++popCursor_; 
    return true; 
}

// erors to include 
// -werror -wextra -wconversion address sanitizer undefined behaviour santizer 
// thread sanitizer 
// writing tests -gtest 
// performance testing 
// add values from one thread 
//read the values from another thread 