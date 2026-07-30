#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../fifo5/spsc.hpp"

TEST_CASE( "SPSC pushing poping functionality", "[SPSC]" ) {
    std::size_t capacity{26}; 
    // Implement FIFO5 testing 
    Fifo1<int> spsc(capacity); // SPSC with a capacity of 2 ^ 26 

    REQUIRE( spsc.capacity() == (1<<capacity) - 1 );
    REQUIRE( spsc.empty() == true ); 

    SECTION( "Adding item to SPSC" ) {
        CHECK(spsc.push(1) == true); 
        REQUIRE( spsc.size() == 1 );
        CHECK(spsc.push(2) == true); 
        REQUIRE( spsc.size() == 2 );
        CHECK(spsc.push(3) == true); 
        REQUIRE( spsc.size() == 3 );
        CHECK(spsc.push(4) == true);  
        REQUIRE( spsc.size() == 4 );
        CHECK(spsc.push(5) == true);  
        
        REQUIRE( spsc.empty() == false ); // fifo is no loner empty 
        REQUIRE( spsc.size() >= 1 );
    }
    SECTION( "Poping elements from SPSC " ) {
        CHECK(spsc.push(1) == true); 
        REQUIRE( spsc.size() == 1 );
        CHECK(spsc.push(2) == true); 
        REQUIRE( spsc.size() == 2 );
        CHECK(spsc.push(3) == true); 
        REQUIRE( spsc.size() == 3 );
        CHECK(spsc.push(4) == true);  
        REQUIRE( spsc.size() == 4 );
        CHECK(spsc.push(5) == true); 

        int return_val; 
        CHECK(spsc.pop(return_val) == true); 

        REQUIRE( return_val == 1 );
        REQUIRE( return_val != 0 );        
        REQUIRE( spsc.size() > 1 );

        REQUIRE(spsc.pop(return_val) == true); 

        REQUIRE( return_val == 2);
        REQUIRE( return_val != 1 );        
        REQUIRE( spsc.size() == 3 );

        REQUIRE(spsc.pop(return_val) == true ); 

        REQUIRE( return_val == 3 );
        REQUIRE( return_val != 0 );          
        REQUIRE( spsc.size() == 2 );

        REQUIRE(spsc.pop(return_val) == true); 

        REQUIRE( return_val == 4);
        REQUIRE( return_val != 3);        
        REQUIRE( spsc.size() == 1 );
        
        REQUIRE(spsc.pop(return_val) == true); 

        REQUIRE( return_val == 5 );
        REQUIRE( return_val != 4);        
        REQUIRE( spsc.size() == 0  );

        REQUIRE( spsc.empty() == true);
        // Test over susciption of elements in the Queue
        // Since queue is empty this should be false 
        REQUIRE( spsc.pop(return_val) == false  );
    }
    SECTION( "Iterator tests. Can you access elements in iterators " ) {
        for(int i{0}; i < 30; ++i)
        {
            spsc.push(i); 
        }

        int *ptr = spsc.cbegin(); 
        REQUIRE(  *ptr == 0 ); // first element should be 0 

        for(int k{0}; k < 30; ++k)
        {
            int *ptr = spsc.cbegin() + k; 
            REQUIRE(  *ptr == k );
        }
    
       
        REQUIRE( spsc.size() >= 30 );
    }
    
    int INT_MA = 1 << 31; 
    BENCHMARK("Benchmark for push max") {
        spsc.push(INT_MA); 
    };
    BENCHMARK("Benchmark for push min") {
        spsc.push(0); 
    };
    BENCHMARK("Benchmark for push and pop operation") {
        int ret_val; 
        spsc.pop(ret_val); 

    };
  
}
