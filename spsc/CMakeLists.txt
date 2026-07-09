cmake_minimum_required(VERSION 3.15)

project(SPSC 
    VERSION 1.0.0
    DESCRIPTION "Single producer single consumer"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SPSC spsc.cc)
# -Werror
target_compile_options(SPSC PRIVATE -march=native  -Wextra -Wconversion -fsanitize=thread)
target_compile_options(SPSC PRIVATE 
    $<$<CONFIG:Debug>:-fsanitize=thread>
    $<$<CONFIG:Debug>:-fPIE>
    $<$<CONFIG:Debug>:-g>
)
target_link_options(SPSC PRIVATE 
    $<$<CONFIG:Debug>:-fsanitize=thread>
    $<$<CONFIG:Debug>:-pie>
)