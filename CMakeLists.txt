cmake_minimum_required(VERSION 3.28)

project(STCFG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.83.0 REQUIRED)
find_package(gtirb 2.2.0 REQUIRED)

add_compile_options(-Wall -Wextra -Werror -pedantic-errors)

add_executable(stcfg src/main.cpp)

target_include_directories(stcfg PRIVATE include Boost::boost gtirb)

target_link_libraries(stcfg PRIVATE gtirb)
