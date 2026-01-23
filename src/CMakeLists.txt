configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Tools/Config.h.in Tools/Config.h)

add_library(TerminalCalculatorLibrary)

if (NOT WIN32)
target_compile_options(TerminalCalculatorLibrary PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

target_link_libraries(TerminalCalculatorLibrary re2)
target_link_libraries(TerminalCalculatorLibrary yaml-cpp)
if (UNIX)
    find_package(Curses REQUIRED)
    target_link_libraries(TerminalCalculatorLibrary ${CURSES_LIBRARY})
endif ()
if (WIN32)
    target_link_libraries(TerminalCalculatorLibrary PDCurses)
endif ()
target_include_directories(TerminalCalculatorLibrary PUBLIC ${CURSES_INCLUDE_DIR})

target_include_directories(TerminalCalculatorLibrary
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_sources(TerminalCalculatorLibrary
    PUBLIC
    Parser/Arithmetic.cpp
    Parser/Commands.cpp
    Parser/Func.cpp
    Parser/Parser.cpp
    Parser/Vars.cpp

    Tools/Color.cpp
    Tools/Config.cpp
    Tools/Entry.cpp
    Tools/State.cpp
)

add_executable(TerminalCalculator main.cpp)
target_link_libraries(TerminalCalculator TerminalCalculatorLibrary)
