cmake_minimum_required(VERSION 3.28)

project(STCFG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gtirb 2.2.0 REQUIRED)
find_package(capstone 5.0.5 REQUIRED)

add_compile_options(-Wall -Wextra -Werror -pedantic-errors)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Og)
elseif (CMAKE_BUILD_TYPE STREQUAL "Default")
    add_compile_options(-O1)
endif ()

add_executable(stcfg src/main.cpp src/Init.cpp
    src/SemanticAnalysis.cpp
    src/CFGAnalysis.cpp)

target_include_directories(stcfg PRIVATE include)

target_link_libraries(stcfg PRIVATE gtirb capstone)
