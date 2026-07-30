# Debug mode / Contributing Guide

The project currently utilises GCC version 14. To eliminate trivial bugs in production the project leverages tools availed by compiler developers. 
In this case, the thread sanitizer and UBSCAN. Changes will not be approved if they fail any of these tooling tests. 


Building the project in debug mode and turning on the UBSCAN and TSCAN will add debug symbols in the project and notify the developer of trivial mistakes. 

## Guide to build

Before creating a PR in the SPSC project, run the update in UBSCAN to catch potential UB that will happen in runtime. 
To have readable error message update the compiler with the following messages. 

Have the following options enabled to print readable errors from thread sanitizer and UBSCAN

`export UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1`
`export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer`

### CMAKE commands to build the project

##### To build with the address sanitizer off and in release mode. Run CMAKE commands below: 

`cmake -B build_release -DUSE_SANITIZER=NONE -DCMAKE_BUILD_TYPE=Release --log-level=STATUS && cmake --build build_release`

Release binaries are be located in: 

`./build_release/SPSC`

##### To enable UBSCAN and run in debug mode. Run the command below: 

`cmake -B build_ubscan -DUSE_SANITIZER=UBSCAN -DCMAKE_BUILD_TYPE=Debug --log-level=STATUS && cmake --build build_ubscan`


##### To enable TSCAN and run in debug mode. Use the command below 

`cmake -B build_tscan -DUSE_SANITIZER=TSCAN -DCMAKE_BUILD_TYPE=Debug --log-level=STATUS && cmake --build build_tscan`

Thread sanitizer enabled binaries will be located in: 

`./build_tscan/SPSC`


### NOTE 

UBSCAN and Thread sanitizer are independent instances and therefore the project buids separate binaries for each 
