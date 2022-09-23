cmake_minimum_required(VERSION 3.23)

project(TerminalCalculator VERSION 0.0.0.1)
configure_file(src/config.h.in config.h)

add_executable(TerminalCalculator src/main.cpp)
set_property(TARGET TerminalCalculator PROPERTY CXX_STANDARD 11)

add_subdirectory(vendor)

if (UNIX)
    find_package(Curses REQUIRED)
    target_link_libraries(TerminalCalculator ${CURSES_LIBRARY})
endif (UNIX)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(src)
include_directories(build)

target_sources(TerminalCalculator
    PUBLIC
        src/entry.cpp
        src/state.cpp
        src/tools/color.cpp
        src/parser/parser.cpp
        src/parser/algebra.cpp
        src/parser/vars.cpp
)
