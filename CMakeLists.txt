cmake_minimum_required(VERSION 3.16)

project(bs_pricer_custom LANGUAGES CXX)

# --- C++ 标准 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 默认构建类型（没指定时用 Release） ---
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- 可执行文件 & 源码 ---
add_executable(bs_pricer_custom
  src/main.cpp
  src/pricer.cpp
  src/csv.cpp
  src/core/bs_engine.cpp
  src/core/implied.cpp
  src/core/date_util.cpp
)

# --- 头文件搜索路径（包含 src/，这样能找到 core/math_util.hpp 等） ---
target_include_directories(bs_pricer_custom PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- 警告等级 ---
if(MSVC)
  target_compile_options(bs_pricer_custom PRIVATE /W4 /permissive-)
else()
  target_compile_options(bs_pricer_custom PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- 优化设置（Release 会自动加 -O3 或 /O2；这里可选补充） ---
if(NOT MSVC)
  target_compile_options(bs_pricer_custom PRIVATE
    $<$<CONFIG:Release>:-O3>
  )
endif()
