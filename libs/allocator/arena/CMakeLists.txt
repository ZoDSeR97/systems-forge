add_library(arena
    src/arena.c
)

target_include_directories(arena
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
