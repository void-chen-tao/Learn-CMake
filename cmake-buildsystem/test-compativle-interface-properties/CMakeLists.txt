cmake_minimum_required(VERSION 3.5)
project(compative-interface-properties)

set(
    CMAKE_DEBUG_TARGET_PROPERTIES
    INCLUDE_DIRECTORIES
    COMPILE_DEFINITIONS
    #POSITION_INDEPENDENT_CODE
    CONTAINER_SIZE_REQUIRED
    #LIB_VERSION
    #LIB_TEST_USER
    #CHECK_VERSION
    PACKAGE_VERSION
)

# add_executable(exe1 exe1.cpp)
# set_property(TARGET exe1 PROPERTY POSITION_INDEPENDENT_CODE ON)

# add_library(lib1 SHARED lib1.cpp)
# set_property(TARGET lib1 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

# add_library(lib2 SHARED lib2.cpp)
# set_property(TARGET lib2 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

# add_executable(exe2 exe2.cpp)
# target_link_libraries(exe2 lib1 lib2)


# ###########  question one
# add_library(lib1 SHARED lib1.cpp)
# set_property(TARGET lib1 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

# add_executable(exe1 exe1.cpp)
# target_link_libraries(exe1 lib1)
# set_property(TARGET exe1 PROPERTY POSITION_INDEPENDENT_CODE OFF)


# ########### question two
# add_library(lib1 SHARED lib1.cpp)
# set_property(TARGET lib1 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

# add_library(lib2 SHARED lib2.cpp)
# set_property(TARGET lib2 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE OFF)

# add_executable(exe2 exe2.cpp)
# target_link_libraries(exe2 lib1 lib2)


# add_library(libV2 lib1.cpp)
# set_property(TARGET libV2 APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL LIB_TEST_USER)
# set_property(TARGET libV2 PROPERTY INTERFACE_LIB_TEST_USER ON)

# add_executable(exe1 exe1.cpp)
# target_link_libraries(exe1 libV2)

# add_library(lib1V2 SHARED lib1.cpp)
# add_library(lib2V2 SHARED lib2.cpp)
# #set_property(TARGET lib1V2 APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL CHECK_VERSION)
# set_property(TARGET lib1V2 PROPERTY INTERFACE_CHECK_VERSION ON)
# set_property(TARGET lib2V2 PROPERTY INTERFACE_CHECK_VERSION ON)

# add_executable(exe2 exe2.cpp)
# target_link_libraries(exe2 lib2V2 lib1V2)


################### COMPATIBLE INTERFACE STRING
# add_library(lib1S1 SHARED lib1.cpp)
# set_property(TARGET lib1S1 PROPERTY INTERFACE_LIB_VERSION 2)
# set_property(TARGET lib1S1 APPEND PROPERTY COMPATIBLE_INTERFACE_STRING LIB_VERSION)

# add_executable(exe1 exe1.cpp)
# target_link_libraries(exe1 lib1S1)


# add_library(lib1S1 SHARED lib1.cpp)
# set_property(TARGET lib1S1 PROPERTY INTERFACE_LIB_VERSION 2)
# set_property(TARGET lib1S1 APPEND PROPERTY COMPATIBLE_INTERFACE_STRING LIB_VERSION)

# add_library(lib2S1 SHARED lib2.cpp)
# set_property(TARGET lib2S1 PROPERTY INTERFACE_LIB_VERSION 3)

# add_executable(exe2 exe2.cpp)
# target_link_libraries(exe2 lib1S1 lib2S1)


add_library(lib1V2 SHARED lib1.cpp)
set_property(TARGET lib1V2 PROPERTY INTERFACE_PACKAGE_VERSION 200)
set_property(TARGET lib1V2 APPEND PROPERTY COMPATIBLE_INTERFACE_NUMBER_MAX PACKAGE_VERSION)

add_library(lib2V2 SHARED lib2.cpp)
set_property(TARGET lib2V2 PROPERTY INTERFACE_PACKAGE_VERSION 1000)

add_executable(exe2 exe2.cpp)
target_link_libraries(exe2 lib1V2 lib2V2)