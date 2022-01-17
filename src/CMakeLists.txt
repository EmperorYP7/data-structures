# Copyright 2021 Yash Pandey(YP) - EmperorYP7
add_executable(testarray
    test.cpp
)

add_executable(testmap
    test_map.cpp
        headers/hashmap)

target_include_directories(testarray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_include_directories(testmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
