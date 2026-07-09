### SPSC 

Exploration Single consumer single producer in C++. 

## What is in the repo 

In the repo there is a built SPSC from scratch. 
The fifo is replicated across 3 subfolders, each with an improved implementation.

The core ideas about SPSC remain intact, with improvements on performance across the repos. 

## FIFO1 
Implements a basic fifo with a push and pop cursor 

## FIFO2
Implements an advanced version with atomics introducted.
Results in a lock free SPSC but not wait free 

## FIFO3 

Introduces the idea of false sharing in cache lines. 
Cache value having to sync with different cachelines introducing inefficiency. 


### Results 

# Testing throughput in FIFO2 Atomics
The first experiment was of fifo2 with a capacity of 1,000,000. Counterintuitively, fifo2 had increased throughput compared to fifo3. This only meant either of the following:
- The implementation of false sharing optimization was incorrect.
- Or the non flushed cache lines at the start of the test resulted in false sharing. 
A simple test to validate any of the above hypothesis was to increase the capacity of the fifo, 
if a performance increased is noticed in fifo2 then this was clear evidence that the performance decline at the start of the tests was a result of non flushed cached lines and pages and not an implementation error. 

I resulted to increasing the capacity of the queue and consequently the throughput dropped. 

**First test results running fifo2** 
![alt text](results/fifo2_spsc.png)

**Second test results running fifo2** 
![alt text](results/test2_fifo_.png)

**Third test results running fifo2** 
![alt text](results/third_test_result.png)

# Testing throughput in FIFO3. Optimized for false sharing 
Using simple chrono timing to test throughput. Fifo3 performs as shown below. 
![alt text](results/TestByWhenCacheLinesAreOptimized.png)

# At a capacity of 5,000,000 the throughput significantly increases 
![alt text](results/fifo3_test2.png)

***Second test showing increase in performance***
![alt text](results/test3_fifo3.png)



## Interesting Finding 


While testing fifo4. A further optimized implementation which includes caching the pointers the thread sanitizer started yelling about a data race. This should be impossible. Turns out it is possible due to human error. Can you find woldo(the bug) in the function implementation below.  


**Can you spot the bug. Its kinda silly.** 

![alt text](results/Thread_Safety_bug.png)


**Bug. In the pop cursor function. I was overriding the cachePopAtomic with the value of pushCursor**. 

Effect: Everything looks fine until a data race occurrs. 

# Result of fifo 4 

Test 4 as with other tests at low capacities the optimization results are not as clear. 
But in higher capacities the throughput is clear. Further highlight the need to flush and warm 
caches and pages. 

![alt text](results/fifo5_test.png)