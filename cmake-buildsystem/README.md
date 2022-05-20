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
  cmake还提供了**BUILD_SHARED_LIBS**变量用于在命令之外更改生成库的行为。库与其生成方式无关，库代表的是它与可执行文件之间的一种连接关系。特别需要说明的是，MODULE库类型的不同之处在于它通常也是不与可执行对象建立连接方式。也就是说，它不能存在于target_link_libraries()的右参数。因为它是作为一种新的插件技术在执行可执行文件之前被提前加载。同时还需要注意一点，如果库不导出任何符号表，该库只能是静态库；不能是动态库，因为动态库会生成一张符号表，以供连接时使用。
#####  Apple Frameworks
在cmake官方的介绍中，可以将一个动态库设置为**FRAMEWORK**属性来生成macOS或者IOS系统下的库架构包。根据Apple公司的统一习惯，还需要设置FRAMEWORK_VERSION参数，按照约定是设置为字符“A”；还有用于识别的一些符号(后缀？我也不清楚是什么，没用过IOS系统)
```cmake{.lines-number}
  add_library(MyFramework SHARED MyFramework.cpp)
  set_target_properties(
    MyFramework   PROPERTIES
    FRAMEWORK_VERSION   A # Version "A" is macOS convention
    MACOSX_FRAMEWORK_IDENTIFIER   org.cmake.MyFramework
  )
```
- 这儿为什么只是动态库需要这样设置呢？
  可能是在APPLE的系统上生成动态库了，但是在调用时需要满足APPLE系统的查找条件。静态库就没有这个问题，因为静态库不导出符号表，不需要调用程序与库之间建立动态连接。

###  Object Libraries
OBJECT库，个人理解应该是所给的源文件(.c/.cpp)通过编译器编译后生成的目标文件(.o)的一种库。但是在官方的解释中，还有一项重要的属性，非存档目标文件的集合(the object library type defines a non-archival collection of object file)。

*问题1：官方所指的non-archival是什么意思？*

目标文件的集合能作为源输入文件提供给其他的target目标，通过使用cmake的解释通式语法来使用。
```cmake{.lines-number}
add_library(archive OBJECT archive.cpp zip.cpp lzma.cpp)
add_library(archiveExtras STATIC $<TARGET_OBJECT:archive> extras.cpp)
add_executable(test_exe $<TARGET_OBJECT:archive> test.cpp)
```
其他target目标可能使用自身源文件(.c/.cpp)同时可以使用Object库来进行链接 ———— 省略了重复编译重复源文件的过程
```cmake{.lines-number}
add_library(archive OBJECT archive.cpp zip.cpp lzma.cpp)

add_library(archiveExtra STATIC extras.cpp)
target_link_libraries(
  archiveExtras 
  PUBLIC
  archive
)

add_executable(test_exe test.cpp)
target_link_libraries(test_exe archive)
```
上述cmake代码使用archive.cpp、zip.cpp、lzma.cpp通过编译器生成了archive.o、zip.o、lzma.o打包成了一个archive的OBJECT库。然后使用extras.cpp生成了archiveExtra的静态库,然后通过链接命令将OBJECT库链接到静态库中去。同时使用了该OBJECT与test.cpp生成了一个可执行文件。

*总结1：OBJECT库一般是用于有些模块或者代码会在整个工程中多次重复使用，这种情况下可以使用OBJECT库来减少重复编译的过程。*

*问题1解决：我是这样理解的，cmake只是把它们编译一遍，并且临时存储，使用时拷贝，cmake执行完毕后销毁*

##  Build Specification and Usage Requirements
**target_include_directories()、target_compile_definitions()、target_compile_options()**这三个目标指明了按什么规则(需要那些头文件呀、需要添加那些编译命令呀、需要那些编译选项呀)来构建target目标以及生成的二进制文件的使用方式。本质上来说，这些命令是用来填充下面的这些target属性的：INCLUDE_DIRECTORIES、COMPILE_DEFINITIONS、COMPILE_OPTION、INTERFACE_INCLUDE_DIRECTORIES、INTERFACE_COMPILE_DEFINITIONS、INTERFACE_COMPILE_OPTIONS。
上述的三个函数都有三种模式，分别是**PRIVATE**、**PUBLIC**和**INTERFACE**。 PRIVATE模式仅填充目标属性的非INTERFACE_变体，INTERFACE模式仅填充INTERFACE_变体。 PUBLIC模式填充各自目标属性的两个变体。它们都是在命令中通过关键字来激活使用的。

*我是这样理解的，对于每个target，cmake存储该target信息是设置了两个参数non-INTERFACE_variant和INTERFACE_variant,一旦你使用了PRIVATE参数target的non-INTERFACE_variant设置为true*

```cmake{.lines-number}
target_compile_definitions(
  archive
  PRIVATE   BUILDING_WITH_LZMA
  INTERFACE   USING_ARCHIVE_LIB
)
```
cmake建议使用命令来设置target目标的属性，而非直接写入特定的属性关键字。因为属性关键字可能会弄混。


###  Target Properties
###  Transitive Usage Requirements
###  Comatible Interface Properties
###  Property Origin Debugging
###  Build Specification with Generator Expressions
####  Include Directories and Usage Requirements
###  Link Libraries and Generator Expressions
###  Output Artifacts
####  Runtime Output Artifacts
####  Library Output Artifacts
####  Archive Output Artifacts
###  Directory-Scoped Commands
##  Build Configurations
###  Case Sensitivity
###  Default And Custom Configurations
##  Pseudo Targets
###  Imported Targets
###  Alias Targets
###  Interface Libraries