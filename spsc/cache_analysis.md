# Cache Analysis results 

Running perf on the cache optimized fifo to investigate cache misses metrics. 
## FIFO 2

Performance counter stats for './build/SPSC':

       214,634,022      cache-references                                                      
         4,667,503      cache-misses                     #    2.17% of all cache refs         
    16,480,250,363      cycles                                                                
    21,485,054,876      instructions                     #    1.30  insn per cycle            
     4,113,270,980      branches                                                              
            18,631      faults                                                                
                 2      migrations                                                            

       2.526940280 seconds time elapsed

       4.605653000 seconds user
       0.035973000 seconds sys

## FIFO 3 

 Performance counter stats for './build/SPSC':

       211,082,296      cache-references                                                      
         4,859,164      cache-misses                     #    2.30% of all cache refs         
    15,818,708,724      cycles                                                                
    20,951,941,252      instructions                     #    1.32  insn per cycle            
     3,883,108,094      branches                                                              
            18,635      faults                                                                
                 7      migrations                                                            

       2.894623357 seconds time elapsed

       5.293462000 seconds user
       0.046968000 seconds sys

## FIFO4 

Performance counter stats for './build/SPSC':

       253,401,934      cache-references                                                      
         2,266,752      cache-misses                     #    0.89% of all cache refs         
    16,467,544,645      cycles                                                                
    20,506,279,535      instructions                     #    1.25  insn per cycle            
     3,776,717,028      branches                                                              
            18,630      faults                                                                
                 2      migrations                                                            

       2.607155339 seconds time elapsed

       5.039237000 seconds user
       0.039009000 seconds sys

## FIFO5 

 Performance counter stats for './build/SPSC':

     8,224,034,775      cache-references                                                      
       138,770,713      cache-misses                     #    1.69% of all cache refs         
   530,816,462,781      cycles                                                                
   627,578,313,027      instructions                     #    1.18  insn per cycle            
   119,304,615,724      branches                                                              
           367,472      faults                                                                
               268      migrations                                                            

      84.679590475 seconds time elapsed

     165.371018000 seconds user
       1.033044000 seconds sys


