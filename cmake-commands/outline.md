#  cmake-commands
##  Scripting Commands
##  Project Commands
###  add_compile_definitions
**New in version 3.12**

- context
  Add preprocessor definitions to the compilation of source files.
  ```cmake{.line-numbers}
  add_compile_definition(<definition> ...)
  ```
  Add preprocessor definitions to the compiler command line.

  The preprocessor definitions are added to the **COMPILE_DEFINITIONS** directory property for the current CMakeLists file. They are also added to the **COMPILE_DEFINITIONS** target property for each target in the current CMakeLists.txt file.

  Definitions are specified using the syntax **VAR** or **VAR=value**. Function-style definitions are not supported. CMake will automatically escape the value correctly for the native build system(note that CMake lanuage syntax may require escapes to specify some values).

  Arguments to **add_compile_definitions** may use "generator expressions" with the syntax $<...>. See the **cmake-generator-expressions** manual for available expressions. See the **cmake-buildsystem** manual for more on defining buildsystem properties.

  - 作用：
  设置当前CMakeLIsts.txt的**COMPILE_DEFINITIONS**，然后由当前环境下的**COMPILE_DEFINITIONS**下发给当前CMakeLists.txt中生成的所有target中的编译属性。

  - 语法：
    - VAR or VAR=value
    - $<...>


###  add_compile_options
- context
  Add options to the compilation of source files.
  ```cmake{.line-numbers}
  add_compile_options(<option> ...)
  ```

  Adds options to the **COMPILE_OPTIONS** directory property. These options are used when compiling targets from the current directory and below.

- Arguments
  Arguments to **add_compile_options** may use "generator expressions" with the syntax &<...>. See the **cmake-generator-expression** manual for available expressions. See the **cmake-buildsystem** manual for more on defining buildsystem properties.

- Option De-duplication
  The final set of options used for a target is constructed by accumulating options from the current target and the usage requirrments of its dependencies. The set of options is de-duplicated to avoid repetition.

  New in version 3.12: While beneficial for individual options, the de-duplication step can break up option groups. For example, **-option A -option B** becomes **-option A B**. One may specify a group of options using shell-like quoting along with a **SHELL:** prefix. The **SHELL:** prefix is dropped, and the rest of the option string is parsed using the **separate_arguments()** **UNIX_COMMAND** mode. For example, **"SHELL:-option A" "SHELL:-option B"** becomes **-option A -option B**.

- Example
  Since different compilers support different options, a typical use of this command is in a compiler-specific conditional clause.
  ```cmake{.line-numbers}
  if(MSCV)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
  else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
  endif()
  ```

- See Also
  This command can be used to add any options. However, for adding preprocessor definitions and include directories it is recommended to use the more specific commands **add_compile_definitions()** and **include_directories()**.

  The command **target_compile_option()** adds target-specific options.
  The source file property **COMPILE_OPTIONS** adds options to one source file.


- 作用：
  为当前CMakeLists.txt及其子CMakeList.txt文件添加编译选项，该命令同样通过设置**COMPILE_OPTIONS**来设置每个target。

- 语法：
  - xxx(具体选项，如MSVC的_CRT_NO_SECURE_WARNINGS)
  - $<...>

- 优点与缺点，以及缺点的解决
  - 优点：该CMake函数将所有的选项都会划分为单独的最小单元，然后依次添加。
  - 缺点：有时候的编译选项可能是一个字符串，这样就被划分为小单元，然后使编译器理解错了程序员的本意。此时可以使用SHELL前缀加编译选项字符串来解决。————"SHELL:-option A" "SHELL:-option B"

- 补充与区分
  没错，是可以使用该命令来添加预处理定义和引用的头文件。但是CMake并不推荐这样做。主要是一个分层管理的问题：compile-option对应编译器级别的定义，而compile-definition用于对应具体项目和target的预处理宏定义等相关的设置。


###  add_custom_target
Add a custom build rule to the generated build system.
There are two main signatures for **add_custom_command**.

- Generating Files
  The first signature is for adding a custom command to produce an output:
  ```cmake{.line-numbers}
  add_custom_command(
    OUTPUT output1 [output2 ...]
    COMMAND command1 [ARGS] [args1...]
    [COMMAND command2 [ARGS] [args2...] ...]
    [MAIN_DEPENDENCY depend]
    [BYPRODUCTS [files...]]
    [IMPLICIT_DEPENDS <lang1> depend1
                      [<lang2> depend2] ...]
    [WORKING_DIRECTORY dir]
    [COMMENT comment]
    [DEPFILE depfile]
    [JOB_POOL job_pool]
    [VERBATIM] [APPEND] [USES_TERMINAL]
    [COMMAND_EXPAND_LISTS]
  )
  ```
  This defines a command to generate specified **OUTPUT** files(s). A target created in the same directory(CMakeLists.txt file) that specifies any output of the custom command as a source file is given a rule to generate the file using the command at build time. Do not list the output in more than one independent target that may build in parallel or the two instances of the rule may conflict(instead use the **add_custom_target()** command to drive the command and make the other targets depend on that one). In makefile terms this creates a new target in the folloing form:
  ```cmake{.line-numbers}
  OUTPUT: MAIN_DEPENDENCY DEPENDS
          COMMAND
  ```

  - APPEND
    Append the **COMMAND** and **DEPENDS** option values to the custom command for the first output specified. There must have already been a previous call to this command with the same output.

###  add_definitions
###  add_dependencies
###  add_executable
###  add_library
###  add_link_options
###  add_subdirectory
###  add_test

##  CTest Commands
##  Deprecated Commands