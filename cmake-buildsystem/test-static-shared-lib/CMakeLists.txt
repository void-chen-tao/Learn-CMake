# step one:  cmake default build static library

cmake_minimum_required(VERSION 3.5)
project(test_lib)
#add_library(archive lib.cpp)

# step two： shared library
#add_library(archive SHARED lib.cpp)

# step three:  keywords
option(BUILD_SHARED_LIBS "Set ON build SHARED, else STATIC" ON)
add_library(archive lib.cpp)