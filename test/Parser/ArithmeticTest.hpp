#pragma once

#include "Parser/Arithmetic.h"

#include <gtest/gtest.h>

#include <string>

static Parser::ArithmeticParser arithmeticParser;

TEST(ArithmeticTest, HandleSingleOperations) {
    std::string testString;

    testString = "2.2+6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "8.2");

    testString = "2.2--6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "8.2");

    testString = "-2.2+6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "3.8");

    testString = "2.2-6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "-3.8");

    testString = "2.2+-6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "-3.8");

    testString = "2.2*6";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "13.2");

    testString = "6/3";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "2");

}

TEST(ArithmeticTest, HandleMultipleOperations) {
    std::string testString;

    testString = "2*6-3";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "9");

    testString = "2+6*3";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "20");

    testString = "2*6/3";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "4");

    testString = "2+6-3";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "5");
}

TEST(ArithmeticTest, HandleInvalidInput) {
    std::string testString;

    testString = "3/0";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "3/0");

    testString = "2*(3+4)";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "2*(3+4)");

    testString = "2*A";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "2*A");

    testString = "2*sin(90)";
    arithmeticParser.ParseArithmetic(testString);
    EXPECT_EQ(testString, "2*sin(90)");
}
