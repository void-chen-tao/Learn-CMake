cmake_minimum_required(VERSION 3.5)
project(test-generator-expression)


##############################  generator expression  ################################
# set(CMAKE_DEBUG_TARGET_PROPERTIES
#     INCLUDE_DIRECTORIES
#     COMPILE_DEFINITIONS
# )

# add_library(lib1 lib1.cpp)
# target_compile_definitions(lib1 INTERFACE
#     $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:LIB1_WITH_EXE>
#     $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:LIB1_WITH_SHARED_LIB>
#     $<$<TARGET_POLICY:CMP0041>:CONSUMER_CMP0041_NEW>
# )


# add_executable(exe1 exe1.cpp)
# target_link_libraries(exe1 lib1)

# cmake_policy(SET CMP0041 NEW)
# add_library(shared_lib shared_lib.cpp)
# target_link_libraries(shared_lib lib1)

###########################  export、 install and generator expression  ####################
add_library(ClimbingStats lib1.cpp)
target_compile_definitions(ClimbingStats INTERFACE
    $<BUILD_INTERFACE:BUILT_DEFINITION>
    $<INSTALL_INTERFACE:INSTALL_DEFINITION>
)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS ClimbingStats EXPORT EX_PACKAGE
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib-include        
)
install(EXPORT EX_PACKAGE NAMESPACE Upstream:: 
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib-export
)

export(EXPORT EX_PACKAGE NAMESPACE Upstream::)