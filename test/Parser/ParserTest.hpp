#pragma once

#include "Parser/Parser.h"
#include "Tools/Entry.h"

#include <gtest/gtest.h>

static Parser::Parser parser;

TEST(ParserTests, HandleValidInput) {
    Tools::Entry entry;

    entry.rawContent_m = "2";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "2");

    entry.rawContent_m = "2*3";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "6");

    entry.rawContent_m = "2*3+4";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "10");

    entry.rawContent_m = "2-3*4";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "-10");

    entry.rawContent_m = "2*(3+4)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "14");

    entry.rawContent_m = ":deg";
    EXPECT_EQ(parser.Parse(entry, 1), 1);

    entry.rawContent_m = "3*sin(90)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "3");

    entry.rawContent_m = "A =  2";
    EXPECT_EQ(parser.Parse(entry, 1), 0);
    entry.rawContent_m = "3*A";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "6");

    entry.rawContent_m = "f(x;y) =  x + y *7";
    EXPECT_EQ(parser.Parse(entry, 1), 0);
    entry.rawContent_m = "3+4*f(2;3)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "95");
}

TEST(ParserTests, HandleInvalidInput) {
    Tools::Entry entry;

    entry.rawContent_m = "2(3*4)";
    EXPECT_EQ(parser.Parse(entry), -1);
    EXPECT_EQ(entry.GetResult(), "");
}
