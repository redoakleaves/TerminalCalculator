cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)

project(TerminalCalculator VERSION 0.0.0.1)

add_subdirectory(vendor)

add_subdirectory(src)
add_subdirectory(test)
