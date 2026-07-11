template<typename T>
class unique_ptr {
    private: 
        T* m_ptr = nullptr; 
    public: 
        explicit unique_ptr(T *_obj): 
            m_ptr{_obj} {}
        ~unique_ptr()
        {
            delete m_ptr; 
        }
        
        T* get() const {return m_ptr;}; 
        T* operator*() const {return m_ptr}; 
        T* operator->() const {return m_ptr}; 

        //delete copy constructor 
        unique_ptr(const unique_ptr&) = delete; 
        //delete copy assignment constructor 
        unique_ptr& operator=(const unique_ptr&) = delete; 

        explicit operator bool() const {
            if(m_ptr != nullptr) return true; 
            return false; 
        }

        //move constructor 
        unique_ptr(unique_ptr &&other) noexcept {
            m_ptr = other.m_ptr; 
            other.m_ptr = nullptr; 

        }
        unique_ptr& operator=(unique_ptr &&other) noexcept
        {
            if(this != &other)
            {
                delete m_ptr; 
                m_ptr = other.m_ptr; 
                other.m_ptr = nullptr; 
            }
            return *this; 
        }
        T* release() noexcept
        {
            // get the addr of the obj
            T* temp = m_ptr; 
            m_ptr = nullptr; 
            return temp; 
        }
        void reset(T *other = nullptr)
        {
            
                //delete the old obj
                delete m_ptr; 
                m_ptr = other; 
        }
        void swap(unique_ptr& other)
        {
            T* tmp = other.m_ptr; 
            other.m_ptr = m_ptr; 
            m_ptr = tmp;
        }

}; 


/**
 * TODO: 
 * 
 * - Add Custom deleter 
 * - Array support 
 * - Store pointer + deleter together 
 * - support nullptr constructor 
 * - Support conversion from Derived base to base 
 * - comparison hash 
 * - get deleter() returntthe default deleter 
 * - make_unique
 * 
 * 
 * 
 */