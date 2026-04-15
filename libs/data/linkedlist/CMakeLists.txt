add_library(linkedlist 
    src/linkedlist.c
)

target_include_directories(linkedlist
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
