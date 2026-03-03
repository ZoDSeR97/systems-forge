add_library(allocator
    src/allocator.c
    src/threadsafe_allocator.c
    src/tls_allocator.c
)

target_include_directories(allocator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    # nothing needed
else()
    target_link_libraries(allocator PUBLIC pthread)
endif()