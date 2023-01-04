#ifndef _PARSER_TEST_H_
#define _PARSER_TEST_H_

#include <gtest/gtest.h>

#include "tools/entry.h"
#include "parser/parser.h"

static Parser::Parser parser;

TEST(ParserTests, HandleValidInput) {
    Tools::Entry entry;

    entry.m_RawContent = "2";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "2");

    entry.m_RawContent = "2*3";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "6");

    entry.m_RawContent = "2*3+4";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "10");

    entry.m_RawContent = "2-3*4";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "-10");

    entry.m_RawContent = "2*(3+4)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "14");

    entry.m_RawContent = ":deg";
    EXPECT_EQ(parser.Parse(entry, 1), 1);

    entry.m_RawContent = "3*sin(90)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "3");

    entry.m_RawContent = "A =  2";
    EXPECT_EQ(parser.Parse(entry, 1), 0);
    entry.m_RawContent = "3*A";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "6");

    entry.m_RawContent = "f(x;y) =  x + y *7";
    EXPECT_EQ(parser.Parse(entry, 1), 0);
    entry.m_RawContent = "3+4*f(2;3)";
    EXPECT_EQ(parser.Parse(entry), 0);
    EXPECT_EQ(entry.GetResult(), "95");
}

TEST(ParserTests, HandleInvalidInput) {
    Tools::Entry entry;

    entry.m_RawContent = "2(3*4)";
    EXPECT_EQ(parser.Parse(entry), -1);
    EXPECT_EQ(entry.GetResult(), "");
}

#endif
