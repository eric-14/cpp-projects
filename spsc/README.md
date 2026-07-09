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


# Testing throughput in FIFO1 
Using simple chrono timing to test throughput. Fifo3 performs as shown below. 
![alt text](results/TestByWhenCacheLinesAreOptimized.png.png)