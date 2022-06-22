cmake_minimum_required(VERSION 3.5)
project(compiler-feature)

add_library(TEST TEST.cpp)
target_compile_features(TEST PRIVATE cxx_abcdefg)

message(STATUS ${CMAKE_C_KNOWN_FEATURES})
message(STATUS "==============")
message(STATUS ${CMAKE_CUDA_KNOWN_FEATURES})
message(STATUS "==============")
message(STATUS ${CMAKE_CXX_KNOWN_FEATURES})
message(STATUS "==============")