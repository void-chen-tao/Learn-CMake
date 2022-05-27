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

演示：
- MSVC
![binary-targets](/test-binary-target/images/step1.png)
![binary-targets](/test-binary-target/images/step2.png)
- gcc
![binary-targets](test-binary-target/images/step3.jpg)
![binary-targets](test-binary-target/images/step4.jpg)

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

演示：
- MSVC
  - default staict library
    ![library-demo](test-static-shared-lib/images/step1.png)
    ![library-demo](test-static-shared-lib/images/step2.jpg)
    ![library-demo](test-static-shared-lib/images/step3.jpg)
  - shared library
    ![library-demo](test-static-shared-lib/images/step4.jpg)
    ![library-demo](test-static-shared-lib/images/step5.jpg)
    ![library-demo](test-static-shared-lib/images/step6.png)
  - KeyWord
    ![library-demo](test-static-shared-lib/images/step7.jpg)
    ![library-demo](test-static-shared-lib/images/step8.png)
    ![library-demo](test-static-shared-lib/images/step9.png)
- gcc
  pass

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

演示：
- MSVC
  ![object-library-test-msvc](test-object-library/image/step1.png)
  ![object-library-test-msvc](test-object-library/image/step2.png)
- gcc
  ![object-library-test-gcc](test-object-library/image/step3.jpg)
  ![object-blirary-test-gcc](test-object-library/image/step4.jpg)
- 总结：
  OBJECT库既可以作为源文件用于生成target的组件，也可以作为链接的组件，并且能够重复使用。

##  Build Specification and Usage Requirements
**target_include_directories()、target_compile_definitions()、target_compile_options()**这三个命令指明了按什么规则(需要哪些头文件呀、需要添加哪些编译命令呀、需要哪些编译选项呀)来构建target目标以及生成二进制文件的makefile说明书。本质上来说，这些命令是用来填充下面的这些target属性的：INCLUDE_DIRECTORIES、COMPILE_DEFINITIONS、COMPILE_OPTION、INTERFACE_INCLUDE_DIRECTORIES、INTERFACE_COMPILE_DEFINITIONS、INTERFACE_COMPILE_OPTIONS。
上述的三个函数都有三种模式，分别是**PRIVATE**、**PUBLIC**和**INTERFACE**。 PRIVATE模式仅填充目标属性的非INTERFACE_变体，INTERFACE模式仅填充INTERFACE_变体。 PUBLIC模式填充各自目标属性的两个变体。它们都是在命令中通过关键字来激活使用的。

*我是这样理解的，对于每个target，cmake存储该target信息是设置了两个参数non-INTERFACE_variant和INTERFACE_variant,一旦你使用了PRIVATE参数target的non-INTERFACE_variant设置为true*
*append：*
  target                  --->      class
  private properties      --->      private
  public properties       --->      struct(默认全部开放)
  interface properties    --->      public

```cmake{.lines-number}
target_compile_definitions(
  archive
  PRIVATE   BUILDING_WITH_LZMA
  INTERFACE   USING_ARCHIVE_LIB
)
```
cmake建议使用命令来设置target目标的属性，而非直接写入特定的属性关键字。因为属性关键字可能会弄混。

猜想与验证：
- 猜想1：PUBLIC会发生继承
  现在我以FIRST宏编译archive库，然后使用PUBLIC链接main.exe。结果如下：
  - gcc
    ![transitive](test-target-property-and-keywords/image/step1.jpg)
  猜想1总结：可以看出我只对archive库使用了编译宏定义，可是main.exe中却开启了FIRST宏定义，说明如果在编译中间库是PUBLIC，且目标是PUBLIC，目标将会继承中间库的编译选项。
- 猜想2：PRIVATE不会继承
  中间库使用PRIVATE，目标target使用PUBLIC
  - gcc
    ![transitive](test-target-property-and-keywords/image/step2.jpg)
  猜想2总结，可以看到在main.exe中因为没有使用编译选项，所以打印的是默认的字符。而archive使用了FIRST编译选项，打印的是相应的first的内容。
- 猜想3：编译选项是可以覆盖的
  我现在使用FIRST编译archive，使用SECONDE编译main
  - gcc
    ![transitive](test-target-property-and-keywords/image/step3.jpg)
  猜想3总结：可以看出虽然main是继承了archive库且archive也是PUBLIC状态，但是通过重写实现了编译选项的更改

总结：INTERFACE其实是介于PUBLIC与PRIVATE之间的一个选项，既不会全部开放自身的属性，也不会全部隐藏自己的属性。
    

###  Target Properties
一般只有在将源文件编译为二进制目标文件时，使用目标参数**INCLUDE_DIRECTORIES**、**COMPILER_DEFINITIONS**和**COMPILER_OPTIONS**。

INCLUDE_DIRECTORIES中出现的参数将会以-I或者-isystem的前缀的方式添加到编译行命令中去。

COMPILE_DEFINITIONS的条目解释是以前缀-D或者/D且顺序不定的方式添加到编译行命令当中去。其中DEFINE_SYMBOL目标参数可能作为对SHARED、MODULE库目标的快捷方式被添加到命令行，以一种编译命令的方式。

COMPILER_OPTIONS是来源于shell，按其属性值出现的顺序添加到编译命令行。

而INTERFACE_INCLUDE_DIRECTORIES、INTERFACE_COMPILE_DEFINITIONS和INTERFACE_COMPILE_OPTIONS对于设置target的编译属性是有语法要求的 ———— 对于使用这些的调用方，必须要保证编译和连接的正确性。对于任何二进制目标而言，使用target_link_libraries()命令中指定的每个目标的INTERFACE_属性会被使用。

```cmake{.line-numbers}
set(srcs archive.cpp zip.cpp)
if(LZMA_FOUND)
  list(APPEND srcs lzma.cpp)
endif()
add_library(archive SHARED ${srcs})
if(LZMA_FOUND)
  # The archive library sources are compiled with -DBUILDING_WITH_LZMA
  target_compile_definitions(archive PRIVATE BUILDING_WITH_LZMA)
endif()
target_compile_definitions(archive INTERFACE USING_ARCHIVE_LIB)

add_executable(consumer)
# Link consumer to archive and  consume its usage requirements. The consumer
# executable sources are compiled with -DUSING_ARCHIVE_LIB.
target_link_libraries(consumer archive)
```

一种很常见的现象是对于一个target，你可能不仅仅需要包括源文件夹，还需要包括相关的用于之前过程产生的生成文件夹中的文件，将这些文件路径都添加到INCLUDE_DIRECTORIES中去，此时CMAKE_INCLUDE_CURRENT_DIR关键字变量可以很好的将你需要的文件都添加到INCLUDE_DIRECTORIES属性中去。同理，变量CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE能够方便的将文件添加给INTERFACE_INCLUDE_DIRECTORIES的所有target。这样就能通过将多重不同路径下的文件夹能够通过target_link_libraries()方便的设置。

- gcc
  ![mulit-properties-test](test-target-property/image/step1.jpg)

###  Transitive Usage Requirements
target目标的属性设置可以传递给依赖项。target_link_libraries()命令中存在PRIVATE、INTERFACE和PUBLIC三个关键字来控制依赖项与target之间的设置参数的传递。

```cmake{.line-numbers}
add_library(archive archive.cpp)
target_compile_definitions(archive INTERFACE USING_ARCHIVE_LIB)

add_library(serialization serialization.cpp)
target_compile_definitions(serialization INTERFACE USING_SERIALIZATION_LIB)

add_library(archiveExtras extras.cpp)
target_link_libraries(archiveExtras PUBLIC archive)
target_link_libraries(archiveExtras PRIVATE serialization)
# archiveExtras is compiled with -DUSING_ARCHIVE_LIB
# and -DUSING_SERIALIZATION_LIB

add_executable(consumer consumer.cpp)
# consumer is compiled with -DUSING_ARCHIVE_LIB
target_link_libraries(consumer archiveExtras)
```
![logic-flow-chart](test-transitive/logic-flow-chart.png)
~~对于archive库而言，使用了**INTERFACE**编译选型定义了USING_ARCHIVE_LIB宏，表明该宏不仅仅适用于archive静态库，同时该宏也是一个能够传递的参数，然后使用该archive静态库与archiveExtras建立了链接，但是它们之间的链接方式是PUBLIC(公开的)，~~
- MSVC
  ![result](test-transitive/image/result1.png)
  ![result](test-transitive/image/result2.png)
  ![result](test-transitive/image/result3.jpg)
  ![result](test-transitive/image/result4.jpg)
  ![result](test-transitive/image/result5.jpg)
  ![result](test-transitive/image/result6.jpg)
  ![result](test-transitive/image/result7.jpg)
- 现象
  从结果来看，archive库和serialization库被编译时没有开启INTEFACE的宏定义，而archiveExtrea库却开启了对两个INTERFACE的宏定义。最后consumer只是开启了其中PUBLIC的宏定义。
- 结论
  这表明我之前的理解是错误的，INTERFACE并没有参与本次target对象的编译中去，而是通过链接被编译到了使用该target新生成的target中去了。这表明INTERFACE在仅仅是向上了一个接口以便被使用，但是本target是没有使用INTERFACE中的任何参数的。而对于新生成的archiveExtreas对象，两个参数通过链接被加入其编译选项中，然后主动将其中一个定义为了PUBLIC另一个定义为了PRIVATE，表明该target使用了这连个编译参数，并且对其中一个参数进行了公开，而另一个参数却选择了隐藏。此时consumer target在链接该库时，只使用了PUBLIC定义的相关参数。

- INTERFACE test
  - MSVC
    ![INTERFACE-test](test-transitive/image/result8.png)
  - 结论
    可以看到，如果我将所有的库的链接方式都使用INTERFACE，那么所有库包括可执行文件都没有使用相关的宏进行编译。这表明INTERFACE只是定义了一种交互的可能，如果在别的target中没有使用PUBLIC或者PRIVATE将它们转化为自己的属性，则不会采用INTERFACE中的参数来参与编译。
  - MSVC 验证
    ![check](test-transitive/image/result9.png)
- PUBLIC test
  - MSVC
    ![PUBLIC-test](test-transitive/image/result10.jpg)
  - 结论
    可以看到，PUBLIC与INTERFACE的表现完全不同，INTERFACE不参与target对象的编译，只是传递参数；而PUBLIC不仅参与target对象的编译，同时还起到了传递参数的作用。PRIVATE参与本次target的编译，但是不参与参数的传递。

一般而言，如果你只在本库中使用该宏定义且该库的头文件中的调用与该宏定义完全无关，建议使用PRIVATE。这样做的好处是隐藏了库的实现的同时避免了宏污染，不然编译参数会在整个依赖项中进行传递，可能造成一些意想不到的错误。如果该库不仅仅使用到了相关定义进行编译，同时被第三方调用时其头文件也是依赖该宏定义的，这种情况下就应该使用PUBLIC关键字开放编译条件，这样使用第三方的头文件就不会有问题。如果一个库的实现没有使用到相关的宏定义，但是在一个总的调度同文件中需要该编译命令才能正确使用该库文件时，应该使用INTERFACE。
```cmake{.line-numbers}
target_link_libraries(
  archiveExtra
  PUBLIC    archive
  PRIVATE   serialization
)
```

如何实现传播的呢？~~cmake通过读取依赖项target中的target属性参数中具有INTERFACE_变体的参数，并且其值添加到当前target对象中的non-INTERFACE参数中去，来实现了PRIVATE。例如：依赖项的INTERFACE_INCLUDE_DIRECTORIES将被读取并且添加到操作对象的INCLUDE_DIRECTORIES中去。但是有些参数和顺序有关并且需要可维护，此时target_link_libraries()的参数可能不为被正确的编译，因为顺序原因。但是可以通过适当的命令来更新顺序。~~
cmake在链接时将读取被链接对象的INTERFACE参数(也就是target属性中带有INTERFACE_前缀的变体)，PUBLIC参数；如果此时是INTERFACE链接的，则当前主动链接方将只会读取target PUBLIC中的参数内容；如果此时是PUBLIC链接的，则当前调用方将会读取INTERFACE参数和PUBLIC参数。

- 链接与include的顺序
  - 顺序相同
    正常链接即可。
  - 顺序相反
    使用generator expression表达式来指明顺序
    如下面的例子，链接包的过程必须是lib1、lib2、lib3但是include包的过程必须是lib3、lib2、lib1.可以这样做
    ```cmake{.line-numbers}
    target_link_libraries(myExe lib1 lib2 lib3)
    target_include_directories(myExe
      PRIVATE   $<TARGET_PROPERTY:lib3,INTERFACE_INCLUDE_DIRECTORIES>
    )
    ```

###  Comatible Interface Properties
存在一些target在设计之初就被要求兼容其他的target或者是众多依赖项的性质。
例如，目标属性POSITION_INDEPENDENT_CODE可以用来指定一个布尔值，然后使用该布尔值来判断是否编译出独立于位置的target。同样的参数还有INTERFACE_INDEPENDENT_CODE。

- POSITION_INDEPENDENT_CODE
  是否创建一个位置独立的target目标，该target属性用于可执行文件或者是动态库，作用是这两个对象是否是位置独立的。

  在默认情况下，对于动态库和模块target，都是默认为True；其他的默认为False。如果在创建一个target之前，存在CMAKE_POSITION_INDEPENDENT_CODE变量，POSITION_INDEPENDENT_CODE将会被其初始化。

- INTERFACE_POSITION_INDEPENDENT_CODE
  使用者是否需要创建一个位置独立的target

  该参数用于通知使用方，并设置使用方的POSITION_INDEPENDENT_CODE的参数与该具有INTERFACE_POSITION_INDEPENDENT_CODE的target设置为相同参数。

```cmake{.line-numbers}
add_executable(exe1 exe1.cpp)
set_property(
  TARGET    exe1
  PROPERTY  POSITION_INDEPENDENT_CODE   ON
)

add_library(lib1 SHARED lib1.cpp)
set_property(
  TARGET      lib1
  PROPERTY    INTERFACE_POSITION_INDEPENDENT_CODE   ON
)

add_executable(exe2  exe2.cpp)
target_link_libraries(exe2  lib1)
```
从上述代码可以看出，对于exe1可执行文件，通过使用set_property函数主动设置了它的POSITION_INDEPENDENT_CODE为ON。
而对于lib1动态库文件，将其设置为了INTERFACE_POSITION_INDEPENDENT_CODE，但是对于可执行文件exe2，同样链接了该库文件，这样就会导致exe2可执行文件的POSITION_INDEPENDENT_CODE参数因为链接了lib1导致同样被设置为了ON。

```cmake{.line-numbers}
add_library(lib1 SHARED lib1.cpp)
set_property(
  TARGET    lib1
  PROPERTY  INTERFACE_POSITION_INDEPENDENT_CODE     ON
)

add_library(lib2 SHARED lib2.cpp)
set_property(
  TARGET    lib2
  PROPERTY  INTERFACE_POSITION_INDEPENDENT_CODE     OFF
)

add_executable(exe1  exe1.cpp)
target_link_libraries(exe1 lib1)
set_property(
  TARGET    exe1
  PROPERTY  POSITION_INDEPENDENT_CODE     OFF
)

add_executable(exe2  exe2.cpp)
target_link_libraries(exe2  lib1  lib2)
```
上述代码存在两个问题：其中之一是对于exe1可执行文件而言，它链接了lib1动态库，可是生成动态库lib1时显式指明了该动态库的调用方的target属性为POSITION_INDEPENDENT_CODE，可是exe1被设置为了OFF，这儿就会产生冲突。其中之二是exe2可执行文件同时使用了lib1库和lib2库文件，它们连个文件一个要求调用方生成位置独立，一个要求调用方位置不独立。上述的冲突都会使CMake发出错误警告：
```cmake{.line-numbers}
CMake Error: The INTERFACE_POSITION_INDEPENDENT_CODE property of "lib2" does not agree with the value of POSITION_INDEPENDENT_CODE already determined for "exe2"
```

这样就存在一个问题：
所有使用方必须和使用的target具有同样的POSITION_INDEPENDENT_CODE属性。

解决冲突的方法：
cmake可以通过使用COMPATIBLE_INTERFACE_BOOL目标属性中来扩展使用方的兼容方式。但是需要注意，每个使用方的属性必须和对应方的属性之间兼容。
```cmake{.line-numbers}
add_library(lib1Version2 SHARED lib1_v2.cpp)
set_property(
  TARGET    lib1Version2
  PROPERTY  INTERFACE_CUSTOM_PROP   ON
)
set_property(
  TARGET    libVersion2
  APPEND    PROPERTY    COMPATIBLE_INTERFACE_BOOL   CUSTOM_PROP
)

add_library(lib1Version3  SHARED  lib1_v3.cpp)
set_property(
  TARGET      libVersion3
  PROPERTY    INTERFACE_CUSTOM_PROP  OFF
)

add_executable(exe1  exe1.cpp)
target_link_libraries(exe1  lib1Version2)   # CUSTOM_PROP  will be ON

add_executable(exe2  exe2.cpp)
target_link_libraries(exe2  lib1Version2  lib1Version3)   # Diagnostic
```
从上述代码可以看出，lib1Version2动态库使用set_property命令打开了INTERFACE_CUSTOM_PROP(用户属性兼容接口)，然后又通过set_property命令来添加了接口兼容的COMPATIBLE_INTERFACE_BOOL，并且使用了CUSTOM_PROP参数。
对于exe1target可以看出，其POSITON_INDEPENDENT_CODE应该是被设置为OFF，而lib1Version2库是一个动态库，其POSITON_INDEPENDENT_CODE应该是ON，此时通过扩展了兼容属性，CUSTOM_PROP将会被设置为ON，表示兼容ON与OFF所产生的冲突。
所以对于exe2target对象而言，关闭了其接口兼容，此时一个为ON，一个为OFF当然后产生Cmake错误。

使用上述的兼容接口布尔值并不是唯一的解决方案。还可以使用**COMPATIBLE_INTERFACE_STRING**属性来到达兼容的效果。COMPATIBLE_INTERFACE_STRING属性中指定的属性必须是未指定的或与所有传递指定的依赖项中的字符串进行比较。该方法对于确保库兼容不同版本的链接方式是非常有用的。
```cmake{.line-numbers}
add_library(lib1Version2  SHARED  lib1_v2.cpp)
set_property(
  TARGET  lib1Version2  
  PROPERTY  INTERFACE_LIB_VERSION  2
)
set_property(
  TARGET  lib1Version2
  APPEND  PROPERTY  COMPATIBLE_INTERFACE_STRING  LIB_VERSION
)

add_library(lib1Version3  SHARED  lib1_v3.cpp)
set_property(
  TARGET  lib1Version3
  PROPERTY  INTERFACE_LIB_VERSION  3
)

add_executable(exe1  exe1.cpp)
target_link_libraries(exe1  lib1Version2)  #  LIB_VERSION will be "2"

add_executable(exe2  exe2.cpp)
target_link_libraries(exe2  lib1Version2  lib1Version3)  #  Diagnostic
```
对于lib1Version2而言，对其添加了LIB_VERSION字符串，然后将该字符串通过COMPATIBLE_INTERFACE_STRING设置为了兼容字符串。所以对于可执行文件exe1来说，它与target属性将会被设置为2。此时如果同时对于可执行文件exe2链接了lib1Version2和lib1Version3，此时它们两个库的LIB_VERSION将会发生冲突。

如何解决这样的冲突呢？通常，我们对于存在同一个库的多个版本的链接，要么是选择版本最高的库要么是选择版本最低的库。可以通过COMPATIBLE_INTERFACE_NUMBER_MAX/MIN参数来设置。
```cmake{.line-numbers}
add_library(lib1Version2 SHARED lib1_v2.cpp)
set_property(
  TARGET  lib1Version2
  PROPERTY  INTERFACE_CONTAINER_SIZE_REQUIRED  200
)
set_property(
  TARGET  lib1Version2
  APPEND  PROPERTY  COMPATIBLE_INTERFACE_NUMBER_MAX  CONTAINER_SIZE_REQUIRED
)

add_library(lib1Version3  SHARED  lib1_v3.cpp)
set_property(
  TARGET  lib1Version3
  PROPERTY  INTERFACE_CONTAINER_SIZE_REQUIRED  1000
)

add_executable(exe1  exe1.cpp)
#  CONTAINER_SIZE_REQUIRED  will be "200"

add_executable(exe2 exe2.cpp)
#  CONTAINER_SIZE_REQUIRED will be "1000"
target_link_libraries(exe2  lib1Version2  lib1Version3)
```
从上述可以看出，对于只存在单个库lib1Version2的exe1可执行文件而言，由于存在COMPATIBLE_INTERFACE_SIZE_REQUIRED将会被设置为200；但是如果存在多个库，他将会按最大的参数来链接指定的库。


###  Property Origin Debugging
使用cmake来创建项目时，脚本的执行成功与否很大程度上取决于代码是否正确、依赖项是否正确以及cmake脚本代码是否正确。者众多的因素导致当脚本出错时，很难判断是那一部分出错了。可以使用**CMAKE_DEBUG_TARGET_PROPERTY**变量来打印出target的编译信息。

```cmake{.line-numbers}
set(
  CMAKE_DEBUG_TARGET_PROPERTIES
  INCLUDE_DIRECTORIES
  COMPILE_DEFINITIONS
  POSITION_INDEPENDENT_CODE
  CONTAINER_SIZE_REQUIRED
  LIB_VERSION
)
add_executable(exe1  exe1.cpp)
```
对于关键字COMPATIBLE_INTERFACE_BOOL和COMPATIBLE_INTERFACE_STRING，debug将会打印该兼容性是由哪一个依赖项传递给target的，并且还将显示其他的target是否也依赖该兼容性接口。而对于COMPATIBLE_INTERFACE_NUMBER_MAX和COMPATIBLE_INTERFACE_NUMBER_MIN参数，debug将会打印每个依赖项的的参数。


###  Build Specification with Generator Expressions
一个项目的生成手册可能需要使用到**generator expressions**，它能够处理cmake构建时的条件以及一些已知内容。例如，读取兼容性参数时可以使用TARGET_PROPERTY表达式。
```cmake{.line-numbers}
add_library(lib1Version2 SHARED lib1_v2.cpp)

# 为lib1Version2添加了一个名为CONTAINER_SIZE_REQUIRED的参数，并且设置为了200
set_property(
  TARGET  lib1Version2
  PROPERTY  INTERFACE_CONTAINER_SIZE_REQUIRED  200
)

# 将CONTAINER_SIZE_REQUIRED参数设置为了COMPATIBLE_INTERFACE_NUMBER_MAX
set_property(
  TARGET  lib1Version2
  APPEND PROPERTY COMPATIBLE_INTERFACE_NUMBER_MAX  CONTAINER_SIZE_REQUIRED
)


add_executable(exe1  exe1.cpp)
target_link_libraries(exe1  lib1Version2)
target_compile_definitions(exe1
  PRIVATE  CONTAINER_SIZE=$<TARGET_PROPERTY:CONTAINER_SIZE_REQUIRED>
)
```


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