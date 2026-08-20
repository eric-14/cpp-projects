### Order book implementation 

- Holds a flat map structure containing asks and bids 
- all orders are preallocated in a contiguous memory location 
- The orderbook should have the ability to decide memory allocation stategy 
- Based on order inputs should be able to rebuild state to the current state 


### Use benchmark  
 - to decide on the map to use for storing bid and ask pointers 