cmake_minimum_required(VERSION 3.5)
project(test-link-generator-expression)

set(
    CMAKE_DEBUG_TARGET_PROPERTIES
    POSITION_INDEPENDENT_CODE
)

add_library(lib1 lib1.cpp)
add_library(lib2 lib2.cpp)


#add_custom_target(print ${CMAKE_COMMAND} -D echo $<TARGET_PROPERTY:POSITION_INDEPENDENT_CODE:lib2>)
#set_property(TARGET lib2 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)
set_property(TARGET lib2 PROPERTY POSITION_INDEPENDENT_CODE ON)
#set_property(TARGET lib1 PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET lib1 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)
target_link_libraries(lib1 PUBLIC
  $<$<TARGET_PROPERTY:POSITION_INDEPENDENT_CODE>:lib2>
)


#add_library(lib3 lib3.cpp)
#set_property(TARGET lib3 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)


#add_executable(exe1 exe1.cpp)
#target_link_libraries(exe1 lib1 lib3)