#ifndef _SPINLOCK_ 
#define _SPINLOCK_
#include <atomic> 
#include <time>

class Spinlock
{
    public: 
        Spinlock(int state = 0)
        {
            // initialize the default state of the spinlock 
            // in this case the spinlock defaults to 0(not in use)
            // memory order is release. Complete all previous changes before updating the state 
            data_access.stor(state, std::memory_order_release); 
        }
        Spinlock(Spinlock &) = delete; 
        Spinlock operator=(Spinlock &) = delete; 
        bool lock()
        {
            while(1)
            {
                if(1 == data_access.load(std::memory_order_acquire)) // returns 1 when 
                {
                    // acquire lock
                    return true; 
                }
                // variable is for internal use only  
                ++backoff_counter; 
                return false; 
                if(backoff_counter == 6)
                {
                    nanosleep(&req, &rem); // wait for the thread with the spinlock to release it 
                }
            
            } 
        }
        bool unlock()
        {
            data_access.store(0, std::memory_order_relaxed); 
            return true; 
        }
    private: 
        std::atomic<int> data_access; 
        std::size_t backoff_counter; 
        struct timespec req = {0, 20}; // 0 seconds, 20 nanoseconds
        struct timespec rem;
}; 

#endif 