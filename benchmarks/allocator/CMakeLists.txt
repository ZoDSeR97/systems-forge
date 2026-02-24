add_executable(allocator_bench
    allocator_bench.cpp
)

target_link_libraries(allocator_bench
    PRIVATE
        allocator
)

target_include_directories(allocator_bench
    PRIVATE
        ${CMAKE_SOURCE_DIR}/components/allocator/include
)