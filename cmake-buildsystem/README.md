**Contents**
[toc]

#  cmake-buildsystem
##  Introduction
cmake构建系统是一种自上而下的设计方案。以target(可执行文件、库、用户自定义等)为操作目标，以命令和规则来对target进行生成限制。

##  Binary Targets
使用add_executable()和add_library()cmake命令来分别生成可执行文件和库文件。而实际生成的可执行文件和库文件将会根据相应的操作平台而带有特定平台的**前缀**和**后缀**以及相应的扩展。可以使用target_link_libraries()来设置target目标的依赖项。
```CMake{.lines-number}
add_library(archive archive.cpp zip.cpp lzma.cpp)
add_executable(zipapp zipapp.cpp)
target_link_libraries(zipapp archive)
```
说明：
首先使用archive.cpp、zip.cpp、lzma.cpp这些源文件编译的目标文件来生成库文件，然后使用zipapp.cpp文件来生成zipapp可执行文件。但是因为可执行文件是需要使用到archive库文件，所以需要在链接过程将库文件与zipapp文件组合成一个整体。

###  Binary Executables
- 语法：
  add_executable(mytool mytool.cpp)

###  Binary Library Types
####  Normal Libraries
- 语法：
  使用add_library命令默认情况下会生成静态库，除非在使用命令时显式指明。
  - default
    add_library(archive STATIC archive.cpp zip.cpp lzma.cpp) == add_library(archive archive.cpp zip.cpp lzma.cpp)
  - specified
    add_library(archive SHARED archive.cpp zip.cpp lzma.cpp)
  - MODULE
    add_library(archive MODULE 7z.cpp)

- 注意：
  cmake还提供了**BUILD_SHARED_LIBS变量用于在命令之外更改生成库的行为。库与其生成方式无关，库代表的是它与可执行文件之间的一种连接关系。特别需要说明的是，MODULE库类型的不同之处在于它通常也是不与可执行对象建立连接方式。也就是说，它不能存在于target_link_libraries()的右参数。因为它是作为一种新的插件技术在执行可执行文件之前被提前加载。同时还需要注意一点，如果库不导出任何符号表，该库只能是静态库；不能是动态库，因为动态库会生成一张符号表，以供连接时使用。
#####  Apple Frameworks
###  Object Libraries