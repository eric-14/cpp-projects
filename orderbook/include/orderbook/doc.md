### Order book implementation 

- Holds a flat map structure containing asks and bids 
- all orders are preallocated in a contiguous memory location 
- The orderbook should have the ability to decide memory allocation stategy 
- Based on order inputs should be able to rebuild state to the current state 


### Use benchmark  
 - to decide on the map to use for storing bid and ask pointers 


## Optimization 

Replace bid and ask map/ flat maps or hashing system with a vector. 
The expectation is a significant increase in speed of access of the values, for the following reasons: 
1. vector is a contiguos memory as compared to a map like unordered map. Note: Flat map is also contiguous. 
2. with vector cache lines are loaded quicker as compared to general maps. 
3. Hashing takes time. Which results in slower map data structures. 