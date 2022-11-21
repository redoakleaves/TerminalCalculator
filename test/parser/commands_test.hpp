#ifndef _COMMANDS_TEST_H_
#define _COMMANDS_TEST_H_

#include <string>

#include <gtest/gtest.h>

#include "tools/config.h"
#include "tools/entry.h"
#include "tools/state.h"

#include "global.h"
#include "parser/commands.h"

TEST(CommandsTest, ExistentCommand) {
    Tools::Entry entry;
    std::string test_string;

    test_string = ":exit";
    EXPECT_EQ(parse_commands(entry, test_string, 1), Commands::Exit);

    test_string = ":deg";
    globalstate.use_deg = 0;
    EXPECT_EQ(parse_commands(entry, test_string, 0), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 0);
    EXPECT_EQ(parse_commands(entry, test_string, 1), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 1);

    test_string = ":rad";
    globalstate.use_deg = 1;
    EXPECT_EQ(parse_commands(entry, test_string, 0), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 1);
    EXPECT_EQ(parse_commands(entry, test_string, 1), Commands::NoAction);
    EXPECT_EQ(globalstate.use_deg, 0);
}

TEST(CommandsTest, NoCommand) {
    Tools::Entry entry;
    std::string test_string;

    test_string = ":xyz";
    EXPECT_EQ(parse_commands(entry, test_string, 0), 0);

    test_string = "exit";
    EXPECT_EQ(parse_commands(entry, test_string, 1), 0);
}

#endif
