cmake_minimum_required(VERSION 3.5)

project(test-object-library)

add_library(archive OBJECT object1.cpp object2.cpp)

add_library(lib STATIC $<TARGET_OBJECTS:archive> lib.cpp)

add_executable(test_exe main.cpp)

target_link_libraries(
    test_exe
    PUBLIC $<TARGET_OBJECTS:archive> lib
)