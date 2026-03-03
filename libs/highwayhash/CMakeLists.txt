add_library(highwayhash
    src/highwayhash.c
)

target_include_directories(highwayhash
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
