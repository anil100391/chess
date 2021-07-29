cmake_minimum_required(VERSION 3.13)
project(chess CXX)
set (CMAKE_CXX_STANDARD 17)

# for youcompleteme to work
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

file (GLOB core_src "${PROJECT_SOURCE_DIR}/zobristkeys.cpp" "${PROJECT_SOURCE_DIR}/board.cpp" )
file (GLOB main_src ${core_src} "${PROJECT_SOURCE_DIR}/main.cpp" )
file (GLOB test_src ${core_src} "${PROJECT_SOURCE_DIR}/test.cpp" )

# undefine min/max macro from msvc minwindef.h
add_compile_options(-DNOMINMAX)

# for definition of M_PI for msvc
add_compile_options(-D_USE_MATH_DEFINES)

add_executable (chess ${main_src})
add_executable (test ${test_src})

target_link_libraries (chess)
target_link_libraries (test)