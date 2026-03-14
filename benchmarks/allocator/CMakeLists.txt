add_executable(allocator_bench
    allocator_bench.cpp
)

target_link_libraries(allocator_bench
    PRIVATE
        arena
)

target_include_directories(allocator_bench
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/arena/include
)