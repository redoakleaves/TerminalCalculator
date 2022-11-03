cmake_minimum_required(VERSION 3.23)

project(TerminalCalculator VERSION 0.0.0.1)
configure_file(src/tools/config.h.in config.h)

add_executable(TerminalCalculator src/main.cpp)
set_property(TARGET TerminalCalculator PROPERTY CXX_STANDARD 11)

add_subdirectory(vendor)

target_link_libraries(TerminalCalculator re2)
target_link_libraries(TerminalCalculator yaml-cpp)
if (UNIX)
    find_package(Curses REQUIRED)
    target_link_libraries(TerminalCalculator ${CURSES_LIBRARY})
endif (UNIX)
if (WIN32)
    target_link_libraries(TerminalCalculator PDCurses)
endif (WIN32)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_sources(TerminalCalculator
    PUBLIC
        src/entry.cpp
        src/state.cpp
        src/tools/color.cpp
        src/tools/config.cpp
        src/parser/parser.cpp
        src/parser/commands.cpp
        src/parser/algebra.cpp
        src/parser/vars.cpp
        src/parser/func.cpp
)
