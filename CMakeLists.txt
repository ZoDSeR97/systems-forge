cmake_minimum_required(VERSION 3.16)
project(SystemsForge C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# Enable optimizations in Release
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(components/allocator)
add_subdirectory(benchmarks/allocator)