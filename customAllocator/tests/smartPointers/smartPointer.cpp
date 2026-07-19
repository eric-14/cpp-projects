/**
 * 
 * Refactor required 
 * 
 */

class RefCounter{
    private: 
        RefCounter(const RefCounter&) = delete; 
        RefCounter operator=(const RefCounter&) = delete; 
        std::size_t m_counter; 
    public: 
        explicit RefCounter() 
        {
            m_counter =0; 
        }
        std::size_t get()
        {
            return m_counter; 
        }
        void operator++()
        {
            m_counter++; 
        }
        void operator++(int)
        {
            m_counter++; 
        }
        void operator--()
        {
            m_counter--; 
        }
        void operator--(int)
        {
            m_counter--; 
        }
}; 

template <typename T>
class shared_ptr {
    RefCounter* counter; 
    T *obj; 
public: 
    shared_ptr(T *raw)
    {
        counter = new RefCounter(); 
        obj = raw; 
        if (obj) (*counter)++; 
    }
    shared_ptr (const shared_ptr& that)
    {
        counter = that.counter; 
        (*counter)++; 
        obj = that.obj; 
    }
    shared_ptr operator=(const SmartPtr& that)
    {
        if(this != &that)
        {
            (*counter)--; 
            if(counter->get() == 0)
            {
                delete counter; 
                delete obj; 
            }
            counter = that.counter; 
            (*counter)++; 
            obj = that.obj; 
        }
        return *this; 
    }
    int use_count()
    {
        return counter->get(); 
    }
    shared_ptr()
    {
        (*counter)--; 
        if(counter->get()==0)
        {
            delete counter; 
            delete obj; 
        }
    }
};
