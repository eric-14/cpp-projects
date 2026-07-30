cmake_minimum_required(VERSION 3.15)

project(SPSC 
    VERSION 1.0.0
    DESCRIPTION "Single producer single consumer"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SPSC spsc.cc)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(USE_SANITIZER "NONE" CACHE STRING "Choose Sanitizer: NONE, TSAN, or UBSCAN")
set_property(CACHE USE_SANITIZER PROPERTY STRINGS "NONE" "TSAN" "UBSAN")

set(RUN_TEST "FALSE" CACHE STRING "Enable testing of the system")
set_property(CACHE RUN_TEST PROPERTY STRINGS "FALSE" "TRUE")
#target_compile_options(SPSC PRIVATE -march=native -Wextra -Wconversion)

if(RUN_TEST STREQUAL "TRUE")
    message(STATUS "======================= RUNNING SPSC UNIT TESTS =============================================")
    Include(FetchContent)

    FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)

    add_executable(Tests Tests/test_fifo5.cpp)
    target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)
endif()


if(USE_SANITIZER STREQUAL "TSAN")
    message(STATUS "========================  Configuring build with ThreadSanitizer (TSAN) ================")
    set_target_properties(SPSC PROPERTIES POSITION_INDEPENDENT_CODE ON)

    target_compile_options(SPSC PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
    target_link_options(SPSC PRIVATE -fsanitize=thread)
endif()

if(USE_SANITIZER STREQUAL "UBSCAN")
    message(STATUS "====================    Configuring build with UBSCAN   ===================================")

    target_compile_options(SPSC PRIVATE -fsanitize=undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer -g)
    target_link_options(SPSC PRIVATE -fsanitize=undefined)
endif()

# Release mode 
if(USE_SANITIZER STREQUAL "NONE")
    message(STATUS "=======================     Building with SANITIZER=NONE    ==============================")
    target_compile_options(SPSC PUBLIC 
        -march=native 
        -Wextra 
        -Wconversion
        # Flags applied ONLY in Debug mode
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-fsanitize=thread>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        # Flags applied ONLY in Release mode (-O3 is handled by CMake automatically, but you can force it)
        $<$<CONFIG:Release>:-O3>
    )

    # 3. Link Options (Sanitizers MUST be linked, PIE is handled by CMAKE_POSITION_INDEPENDENT_CODE)
    target_link_options(SPSC PUBLIC
        # Flags applied ONLY in Debug mode
        $<$<CONFIG:Debug>:-fsanitize=thread>
    )
endif()
