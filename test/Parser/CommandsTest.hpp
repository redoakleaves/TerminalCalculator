#pragma once

#include "global.h"
#include "Parser/Commands.h"
#include "Tools/Config.h"
#include "Tools/Entry.h"
#include "Tools/State.h"

#include <gtest/gtest.h>

#include <optional>
#include <string>

static Parser::CommandParser commandParser;

TEST(CommandsTest, ExistentCommand) {
    Tools::Entry entry;
    std::string testString;

    testString = ":exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), Parser::Command::Exit);

    testString = ":deg";
    globalState.useDeg_m = false;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, false), Parser::Command::NoAction);
    EXPECT_EQ(globalState.useDeg_m, false);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), Parser::Command::NoAction);
    EXPECT_EQ(globalState.useDeg_m, true);

    testString = ":rad";
    globalState.useDeg_m = true;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, false), Parser::Command::NoAction);
    EXPECT_EQ(globalState.useDeg_m, true);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), Parser::Command::NoAction);
    EXPECT_EQ(globalState.useDeg_m, false);
}

TEST(CommandsTest, InvalidCommand) {
    Tools::Entry entry;
    std::string testString;

    testString = ":xyz";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, false), std::nullopt);

    testString = "xzy:exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), std::nullopt);

    testString = "exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), std::nullopt);
}
