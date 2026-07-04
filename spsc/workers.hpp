#pragma once 
#include <random> 
#include <vector>
#include <thread> 
#include <string>
#include <print>
#include "spsc.hpp"

/**
 * Worker is tightly coupled to fifo of type int
 */
class Worker {
    std::vector<int> source; 
    std::vector<int> destination; 
    
    std::mt19937 gen; 
    std::jthread produce_; 
    
    std::jthread consume_;


    int getInt(int range_min, int range_max)
    {
        std::uniform_int_distribution<int> distrib(range_min, range_max);
        return distrib(gen); 
    }

    public: 
        explicit Worker(Fifo1<int,std::allocator<int>> &fifo): gen(std::random_device{}())
        {
            std::random_device rd; 
            produce_= std::jthread([this, &fifo](std::stop_token token) {
                 producer(token, "producer", fifo);
            }); 

            consume_ = std::jthread([this, &fifo](std::stop_token token) {
                 consumer(token, "consumer", fifo); 
            });    
        }
        
        void producer(std::stop_token token,std::string prefix, Fifo1<int, std::allocator<int>> &fifo)
        {
            while(!token.stop_requested())
            {
                std::println("Thread producer {}", prefix); 
                // send items 
                std::size_t capacity = fifo.capacity(); 
                // append(100) items
                for(std::size_t i{0}; i < capacity; ++i )
                {
                    int rand_val = getInt(0,1000); 
                    fifo.push(rand_val); 
                }
               
                
            }
        }
        void consumer(std::stop_token token,std::string prefix,Fifo1<int, std::allocator<int>> &fifo)
        {
            while(!token.stop_requested())
            {
                std::println("Thread consumer {}", prefix);

                std::size_t capacity = fifo.capacity(); 
                // append(100) items
                for(std::size_t i{0}; i < capacity; ++i )
                {
                    int item_{}; 
                    fifo.pop(item_); 
                    std::println("[consumer] received {}",item_); 
                }
                
            }
            
        }
}; 