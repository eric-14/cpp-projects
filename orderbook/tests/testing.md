## Testing

Testing is based on the Catch2 Framework written by Phil Nash. It is a single header file framework that can be included in the project to get started. 

Due to personal preference, simplicity and lightweight nature of the framework, it's used for testing and benchmarking. 


To run tests the the following commands sequentially: 

``cmake -B build_tests -DRUN_TEST=TRUE -DCMAKE_BUILD_TYPE=Debug --log-level=STATUS``

``cmake --build build_tests && ./build_tests/Tests``