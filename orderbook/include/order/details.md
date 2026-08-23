## Areas of Optimization research 

Since the order has an organization of byte x 3, (8bytes x 5) based on frequency of access of the member variables 
would it be worth to increase the size of the orders by optimizing member variable for cache alignment

### Order Class layout 

========================= Class sizeof 136 internals Information ===================


State 0 | Type 1 | Frame 2 | Direction 3 | Price 8 | Amount 16 | Filled 24 | TimeStamp 32 | Symbol 40 | OrderBook Id 72 | UserID 104


## Varying size of the order object

Due to short string optimizations there might be varying size of the class object size. There alternative is to reserve strings, use experimental basic_string or use boost strings which allow definition of the SSO sizes. 