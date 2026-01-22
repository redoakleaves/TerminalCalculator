#pragma once

#include "global.h"
#include "Parser/Func.h"

#include <gtest/gtest.h>

#include <string>

static Parser::FuncParser funcParser;

TEST(FuncTest, HandleValidDef) {
    std::string testString;

    testString = "f(x)=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), true);

    testString = "f(x;y)=x^2+y";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), true);
}

TEST(FuncTest, HandleInvalidDef) {
    std::string testString;

    testString = "f(x)==x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);

    testString = "f(x=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);

    testString = "fx)=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);

    testString = "f()=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);

    testString = "f=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);
}

TEST(FuncTest, HandleConstOverride) {
    std::string testString;

    testString = "sin(x)=x";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString), false);
}

TEST(FuncTest, HandleConstUsage) {
    std::string testString;

    globalstate.m_UseDeg = 1;

    testString = "sin(90)";
    funcParser.ParseConstFuncUsage(testString);
    EXPECT_EQ(testString, "1");

    testString = "pow(2;3)";
    funcParser.ParseConstFuncUsage(testString);
    EXPECT_EQ(testString, "8");

    testString = "sin(90)+abs(-2)";
    funcParser.ParseConstFuncUsage(testString);
    EXPECT_EQ(testString, "1+2");
}

TEST(FuncTest, HandleFuncUsage) {
    std::string testString;

    testString = "f(x)=x^2";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString, true), true);
    testString = "h(x;y)=x/y";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString, true), true);

    testString = "f(3)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "(3^2)");

    testString = "h(2;3)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "(2/3)");

    testString = "f(3)+h(2;3)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "(3^2)+(2/3)");

    testString = "2*f(3)+4";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "2*(3^2)+4");
}

TEST(FuncTest, HandleInvalidUsage) {
    std::string testString;

    testString = "f()";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "f()");

    testString = "f(2";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "f(2");

    testString = "f2)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "f2)");

    testString = "h(2)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "h(2)");
}

TEST(FuncTest, HandleOverwriteDef) {
    std::string testString;

    testString = "f(3)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "(3^2)");

    testString = "f(x;y)=x+y";
    EXPECT_EQ(funcParser.ParseFuncDefinition(testString, 1), 1);

    testString = "f(2;3)";
    funcParser.ParseFuncUsage(testString);
    EXPECT_EQ(testString, "(2+3)");
}
