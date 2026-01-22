#pragma once

#include "global.h"
#include "Parser/Commands.h"
#include "tools/config.h"
#include "tools/entry.h"
#include "tools/state.h"

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
    globalstate.m_UseDeg = 0;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, false), Parser::Command::NoAction);
    EXPECT_EQ(globalstate.m_UseDeg, 0);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), Parser::Command::NoAction);
    EXPECT_EQ(globalstate.m_UseDeg, 1);

    testString = ":rad";
    globalstate.m_UseDeg = 1;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, false), Parser::Command::NoAction);
    EXPECT_EQ(globalstate.m_UseDeg, 1);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, true), Parser::Command::NoAction);
    EXPECT_EQ(globalstate.m_UseDeg, 0);
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
