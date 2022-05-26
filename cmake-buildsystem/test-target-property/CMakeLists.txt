cmake_minimum_required(VERSION 3.5)
project(test-target-property)

add_library(archive source.cpp)
target_compile_definitions(
    archive
    PRIVATE USE_FILE1
)

target_compile_definitions(
    archive
    INTERFACE USE_FILE2
)

add_executable(main main.cpp)
target_link_libraries(main archive)