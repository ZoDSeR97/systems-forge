cmake_minimum_required(VERSION 3.16)
project(SystemsForge C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

# Enable optimizations in Release
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(libs/allocator/arena)
add_subdirectory(libs/allocator/pool)
add_subdirectory(libs/security/highwayhash)

add_subdirectory(benchmarks/allocator)