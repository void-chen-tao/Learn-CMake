cmake_minimum_required(VERSION 3.5)
project(test-transitive)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)


add_library(archive archive.cpp)
target_compile_definitions(archive PUBLIC USING_ARCHIVE_LIB)

add_library(serialization serialization.cpp)
target_compile_definitions(serialization PUBLIC USING_SERIALIZATION_LIB)

add_library(archiveExtras extras.cpp)
#add_executable(archiveExtras extras.cpp)
target_link_libraries(archiveExtras PUBLIC archive)
target_link_libraries(archiveExtras PUBLIC serialization)
# archiveExtras is compiled with -DUSING_ARCHIVE_LIB
# and -DUSING_SERIALIZATION_LIB

add_executable(consumer consumer.cpp)
# consumer is compiled with -DUSING_ARCHIVE_LIB
target_link_libraries(
    consumer 
    PUBLIC archiveExtras
)