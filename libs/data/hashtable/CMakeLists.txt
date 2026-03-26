add_library(hashtable 
    src/hashtable.c
)

target_include_directories(hashtable
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hashtable
    PUBLIC
        highwayhash
)