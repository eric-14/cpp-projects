
template <typename T, typename Alloc = std::allocator<T>> 
class Fifo1 : private Alloc 
{
    std::size_t capacity_; 
    T* ring_; 
    std::size_t pushCursor_{}; 
    std::size_t popCursor_{}; 


    public: 
        explicit Fifo1(std::size_t capacity, Alloc const& alloc = Alloc{}) 
            : Alloc(alloc), capacity_(capacity), ring_{std::allocator_traits::allocate(*this, capacity)}
        {}
        ~Fifo1(){
            while(not empty()){
                ring_[popCursor_ % capacity_].~T(); 
                ++popCursor_; 
            }
            std::allocator_traits::deallocate(*this, ring_, capacity_); 
        }
        auto capacity() const {return capacity_;}
        auto size() const {return pushCursor_ - popCursor; };
        auto empty() const { return size() == 0; }
        auto full() const {return size() == capacity(); }

        auto push(T const& value); 
        auto pop(T* value); 

}
