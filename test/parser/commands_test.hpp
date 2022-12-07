#ifndef _COMMANDS_TEST_H_
#define _COMMANDS_TEST_H_

#include <string>

#include <gtest/gtest.h>

#include "tools/config.h"
#include "tools/entry.h"
#include "tools/state.h"

#include "global.h"
#include "parser/commands.h"

static Parser::CommandParser commandParser;

TEST(CommandsTest, ExistentCommand) {
    Tools::Entry entry;
    std::string testString;

    testString = ":exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 1), Commands::Exit);

    testString = ":deg";
    globalstate.use_deg = 0;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 0), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 0);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 1), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 1);

    testString = ":rad";
    globalstate.use_deg = 1;
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 0), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 1);
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 1), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 0);
}

TEST(CommandsTest, InvalidCommand) {
    Tools::Entry entry;
    std::string testString;

    testString = ":xyz";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 0), 0);

    testString = "xzy:exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 1), 0);

    testString = "exit";
    EXPECT_EQ(commandParser.ParseSubstring(entry, testString, 1), 0);
}

#endif
