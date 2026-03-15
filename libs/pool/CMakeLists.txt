add_library(pool
    src/pool.c
)

target_include_directories(pool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
