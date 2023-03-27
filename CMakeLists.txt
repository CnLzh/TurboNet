# 指定使用的CMake最低版本
cmake_minimum_required(VERSION 3.24)

# 项目名称
project(TurboNet)

# 指定C++编译器使用的C++标准版本
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 指定编译器包含目录
include_directories(
        ${PROJECT_SOURCE_DIR}/src/base
        ${PROJECT_SOURCE_DIR}/src/logger
        ${PROJECT_SOURCE_DIR}/src/net
)

# 指定目录下的源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base SRC_BASE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/logger SRC_LOG)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/net SRC_NET)

# 生成动态库
add_library(
        TurboNet SHARED
        ${SRC_BASE}
        ${SRC_LOG}
        ${SRC_NET}
)

# 设置生成的动态库路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置默认安装路径
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# 头文件安装路径
install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/src/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)

# 不指定默认release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# 仅在debug下编译单元测试
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/src/base/test)
    add_subdirectory(${PROJECT_SOURCE_DIR}/src/logger/test)
    add_subdirectory(${PROJECT_SOURCE_DIR}/src/net/test)
endif ()