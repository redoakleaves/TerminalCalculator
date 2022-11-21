configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../src/tools/config.h.in tools/config.h)

add_executable(TerminalCalculatorTests main.cpp)

target_link_libraries(TerminalCalculatorTests TerminalCalculatorLibrary)

target_link_libraries(TerminalCalculatorTests gtest)

target_include_directories(TerminalCalculatorTests PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_sources(TerminalCalculatorTests
    PUBLIC
        parser/commands_test.hpp
)
