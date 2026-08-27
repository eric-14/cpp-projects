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
            data_access.store(state, std::memory_order_release); 
        }
        Spinlock(Spinlock &) = delete; 
        Spinlock operator=(Spinlock &) = delete; 
        // Do not support move operations at the moment 
        Spinlock(Spinlock &&) = delete; 
        Spinlock operator=(Spinlock &&) = delete; 
        bool lock()
        {
            while(true)
            {
                if(data_access.compare_exchange_strong(false,true)) // returns 1 when 
                {
                    // acquire lock
                    return true; 
                }
                // variable is for internal use only  
                ++backoff_counter; 
                
                if(backoff_counter == 6)
                {
                    nanosleep(&req, &rem); // wait for the thread with the spinlock to release it 
                     return false; 
                }
            } 
        }
        bool unlock()
        {
            data_access.store(false, std::memory_order_release); 
            return true; 
        }
    private: 
        std::atomic<bool> data_access; 
        std::size_t backoff_counter; 
        struct timespec req = {0, 20}; // 0 seconds, 20 nanoseconds
        struct timespec rem;
}; 

#endif 