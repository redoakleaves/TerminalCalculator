# googletest
option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)
add_subdirectory(googletest)

# PDCurses
if (WIN32)
    file(GLOB SRC_FILES PDCurses/pdcurses/*.c PDCurses/wincon/*.c)

    add_library(PDCurses ${SRC_FILES})
    target_include_directories(PDCurses
        PUBLIC
        PDCurses
        PDCurses/wincon
    )

    set(CURSES_INCLUDE_DIR PDCurses)
endif (WIN32)

# re2
option(RE2_BUILD_TESTING "" OFF)
add_subdirectory(re2)

# yaml-cpp
add_subdirectory(yaml-cpp)
