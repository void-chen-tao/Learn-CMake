cmake_minimum_required( VERSION 3.5 )
project(test_binary_target)

add_library(archive lib1.cpp lib2.cpp)

add_executable(exe main.cpp)

target_link_libraries(exe archive)