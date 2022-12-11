# ~~~
# 3.21  - PROJECT_IS_TOPLEVEL
# 3.11  - FetchContent
# ~~~
cmake_minimum_required(VERSION 3.21)

project(
    samwarring_cpp_utils
    VERSION 0.0.1
)

add_library(samwarring_cpp_utils INTERFACE)
target_include_directories(
    samwarring_cpp_utils
    INTERFACE include
)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(test)
endif ()
