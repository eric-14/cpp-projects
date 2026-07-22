## Why implement custom memory allocators 
1. Fewer calls to new and delete 
2. Reduced thread contention 
3. Reduced false sharing 
4. Reduced memory diffusion 
5. Increase memory contiguity 
6. Wink out memory in a GC sense 

By implementing a custom allocator each thread can have its own memory resource, 
eliminating problem number 1,2,3 and 4 above. 

## Implementation
Allocator implementation 
Allocator traits 
containers uses information from allocator traits to perform implementations. 


## PMR 

Tailor container allocation strategy at runtime through dynamic polymorphism. 

## Design decisions on writing an allocator 
1. Plumbing - public interface. Traditional or PMR 
2. Structural management - where do I get data from. S-break, SHEMAT 
    - Addressing Model 
    - Storage model 
    - Pointer Interface model 
    - Allocation Strategy 

3. Concurrency Management 
    - Thread Safety 
    - Transaction Safety - database models 

## Strategies/Design decisions of Implementation 
1. Global allocator 
2. Type Parameter 
3. Abstract Base 
4. Monotomic, Multipool & Multipool<Monotonic>
5. Type Parameter or Abstract Base 
6. Normal Destruction or Magically Winked out. 

## Dimensions of Usage Scenarios 
1. Density of allocation operations 
2. Variation of allocated Sizes 
3. Locality of accessed memory 
4. Utilization of allocated memory 
5. Contention of concurrent allocations 

#### Assignment does not move the allocator(c++11)


## Based on CppCon talk by Bob Steagall. How to write a custom allocator 

***The basic purpose of an allocator is to provide a source of memory for a given type, and a place to return that memory once it is no longer needed.***


***Bjarne Stroustrup***

# References 

1. CPPcon 2017 John Lakos "Local Arena memory allocators"
2. CppCon 2017: Pablo Halpern. "Allocators the good parts"
