#pragma once

#include "Parser/Vars.h"

#include <gtest/gtest.h>

#include <numbers>
#include <sstream>
#include <string>

static Parser::VarParser varParser;

TEST(VarTest, HandleValidDef) {
    std::string testString;

    testString = "A=2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), true);

    testString = "A=-2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), true);

    testString = "A=2.2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), true);

    testString = "A=-2.2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), true);
}

TEST(VarTest, HandleInvalidDef) {
    std::string testString;

    testString = "A==2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), false);

    testString = "A=B";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), false);
}

TEST(VarTest, HandleConstOverride) {
    std::string testString;

    testString = "pi=3";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString), false);
}

TEST(VarTest, HandleConstUsage) {
    std::string testString;

    testString = "1/pi+9";
    varParser.ParseVarUsage(testString);
    std::stringstream stream;
    stream << "1/" << std::numbers::pi << "+9";
    EXPECT_EQ(testString, stream.str());
}

TEST(VarTest, HandleVarUsage) {
    std::string testString;

    testString = "A=2";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString, true), true);
    testString = "B=5";
    EXPECT_EQ(varParser.ParseVarDefinitions(testString, true), true);

    testString = "A+4";
    varParser.ParseVarUsage(testString);
    EXPECT_EQ(testString, "2+4");

    testString = "3*A";
    varParser.ParseVarUsage(testString);
    EXPECT_EQ(testString, "3*2");

    testString = "3*A+4";
    varParser.ParseVarUsage(testString);
    EXPECT_EQ(testString, "3*2+4");

    testString = "A+B";
    varParser.ParseVarUsage(testString);
    EXPECT_EQ(testString, "2+5");
}

TEST(VarTest, HandleInvalidUsage) {
    std::string testString;

    testString = "3*C+4";
    varParser.ParseVarUsage(testString);
    EXPECT_EQ(testString, "3*C+4");
}
