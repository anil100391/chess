cmake_minimum_required(VERSION 3.13)
project(chess CXX)
set (CMAKE_CXX_STANDARD 17)

# for youcompleteme to work
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

file (GLOB main_src "${PROJECT_SOURCE_DIR}/*.cpp" )

# undefine min/max macro from msvc minwindef.h
add_compile_options(-DNOMINMAX)

# for definition of M_PI for msvc
add_compile_options(-D_USE_MATH_DEFINES)

add_executable (chess ${main_src})

target_link_libraries (chess)