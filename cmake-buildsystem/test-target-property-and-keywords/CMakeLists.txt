# cmake_minimum_required(VERSION 3.5)

# project(include-and-compile-definition-test)

# add_library(temp temp.cpp)
# target_include_directories(
#     temp
#     PRIVATE  temp.h
# )
# target_compile_definitions(
#     temp
#     INTERFACE -DSECONDE
# )
# #################################
# add_executable(main main.cpp)
# target_link_libraries(
#     main
#     PRIVATE temp
# )


#################  猜想1  ################
cmake_minimum_required(VERSION 3.5)
project(guess1)

#########################################
#        以FIRST编译选项编译archive
#########################################
add_library(archive temp.cpp)
target_compile_definitions(
    archive
    PRIVATE -DFIRST
)
target_include_directories(
    archive
    PRIVATE temp.h
)

########################################
#       以PUBLIC的方式链接archive
########################################
add_executable(main main.cpp)
target_link_libraries(
    main
    PUBLIC archive
)
target_include_directories(
    main
    PRIVATE head.h
)
target_compile_definitions(
    main
    PRIVATE -DSECONDE
)