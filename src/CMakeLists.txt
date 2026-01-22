configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tools/config.h.in tools/config.h)

add_library(TerminalCalculatorLibrary)

target_link_libraries(TerminalCalculatorLibrary re2)
target_link_libraries(TerminalCalculatorLibrary yaml-cpp)
if (UNIX)
    find_package(Curses REQUIRED)
    target_link_libraries(TerminalCalculatorLibrary ${CURSES_LIBRARY})
endif (UNIX)
if (WIN32)
    target_link_libraries(TerminalCalculatorLibrary PDCurses)
endif (WIN32)
target_include_directories(TerminalCalculatorLibrary PUBLIC ${CURSES_INCLUDE_DIR})

target_include_directories(TerminalCalculatorLibrary
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_sources(TerminalCalculatorLibrary
    PUBLIC
        tools/color.cpp
        tools/config.cpp
        tools/entry.cpp
        tools/state.cpp

        Parser/Arithmetic.cpp
        Parser/Commands.cpp
        Parser/Func.cpp
        Parser/Parser.cpp
        Parser/Vars.cpp
)

add_executable(TerminalCalculator main.cpp)
target_link_libraries(TerminalCalculator TerminalCalculatorLibrary)
