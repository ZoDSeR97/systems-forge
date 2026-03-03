cmake_minimum_required(VERSION 3.16)
project(SystemsForge C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# Enable optimizations in Release
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(libs/allocator)
add_subdirectory(libs/highwayhash)
add_subdirectory(libs/hashtable)

add_subdirectory(benchmarks/allocator)