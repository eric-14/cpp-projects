# SPSC 

In this repo, you will find work done to understand perfomance optimization of single consumer single producer FIFO in C++. 

This starts by implementing a single threaded fifo with push and pop cursors updated thorugh a consumer and a producer API. 
The journey is completed by having a multi-threaded SPSC with optimization made for pushing and poping elements. Spoiler alert! The cursors have been atomized and memory aligned reducing false sharing. 

Further, calculation of element position in the SPSC has been optimized from mod(%) to bitwise AND(\&) operator. The tradeoff is increased memory consumption but improved latency. i.e. division in x86 is anywhere from 9 cycles while AND is 1 clock cycle. 

Finally, caching of the cursors is introduced. In previous operations, the SPSC atomic cursors were accessed in every iteration. Although, not inherently costly, accessing the pointers in high throughput scenarios such as 10^6 operations introduces performance hits compared to caching the pointers and only updating the caches when the FIFO is full. 


#### NB: On closer review my clock might not be steady. 
The principles still hold and I will move the codebase to gbenchmarks. 

### FIFO1 
Implements a single threaded fifo with a push and pop API functions.

### FIFO2
Concurrency is introduced and implements a FIFO version with atomicity on the push and pop cursors.
Results in a lock free SPSC but false sharing `perf` metrics are relatively high.  

### FIFO3 

Optimization around false sharing measured in FIFO2. Cache line syncing introduces ineffeciencies which can eliminated. 
By having the cursors occupy the full width of the cache lines. 

### Results 

### Testing throughput in FIFO2 Atomics

The first measurement was of fifo2 with a capacity of 1,000,000 elems. Counterintuitively, fifo2 had increased throughput compared to fifo3. This only meant either of the following:
- The implementation of false sharing optimization was incorrect.
- Or the non flushed cache lines at the start of the test resulted in cache syncing eliminating the result of optimized cache lines. 

A simple test to validate any of the above hypothesis is to increase the capacity of the fifo. 
If throughput increases in fifo2 then it is a clear evidence that the performance decline at the start of the tests was a result of non flushed cached lines and pages and not an implementation error. 

I resulted to increasing the capacity of the queue and consequently the throughput dropped. 

**First test results running fifo2** 
![fifo2 perfomance results](results/fifo2_spsc.png)

**Second test results running fifo2** 
![fifo1 performance metrics](results/test2_fifo_.png)

**Third test results running fifo2** 
![fifo3 performance metrics](results/third_test_result.png)

# Testing throughput in FIFO3. Optimized for false sharing 
Using simple chrono timing to test throughput. Fifo3 performs as shown below. 
![fifo3 performance metrics](results/TestByWhenCacheLinesAreOptimized.png)

# At a capacity of 5,000,000 the throughput significantly increases 
![fifo3 performance metrics](results/fifo3_test2.png)

***Second test showing increase in performance***
![fifo3 performance metrics](results/test3_fifo3.png)



### Interesting Bug 

While testing fifo4. A further optimized implementation which includes caching the pointers the thread sanitizer started yelling about a data race. This should not be impossible. Turns out it is possible due to human error. Can you find woldo(the bug) in the function implementation below.  


**Can you spot the bug. Its kinda silly.** 

![Bug resulting in data race](results/Thread_Safety_bug.png)


**Bug. In the pop cursor function. I was overriding the cachePopAtomic with the value of pushCursor**. 

Effect: Everything looks fine until a data race occurrs. 

### Result of fifo 4 

Test 4 as with other tests at low capacities the optimization results are not as clear. 
But in higher capacities the throughput is clear. Further highlight the need to flush and warm 
caches and pages. 

![FIFO5 measurement results](results/fifo5_test.png)