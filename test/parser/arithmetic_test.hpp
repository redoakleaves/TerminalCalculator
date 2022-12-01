#ifndef _ARITHMETIC_TEST_H_
#define _ARITHMETIC_TEST_H_

#include <string>

#include <gtest/gtest.h>

#include "tools/entry.h"
#include "parser/arithmetic.h"

TEST(ArithmeticTest, HandleSingleOperations) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "2.2+6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "8.2");

    test_string = "2.2--6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "8.2");

    test_string = "-2.2+6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "3.8");

    test_string = "2.2-6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "-3.8");

    test_string = "2.2+-6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "-3.8");

    test_string = "2.2*6";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "13.2");

    test_string = "6/3";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "2");

}

TEST(ArithmeticTest, HandleMultipleOperations) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "2*6-3";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "9");
    
    test_string = "2+6*3";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "20");

    test_string = "2*6/3";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "4");

    test_string = "2+6-3";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "5");
}

TEST(ArithmeticTest, HandleInvalidInput) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "2*(3+4)";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "2*(3+4)");

    test_string = "2*A";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "2*A");

    test_string = "2*sin(90)";
    parse_arithmetic(entry, test_string);
    EXPECT_EQ(test_string, "2*sin(90)");
}

#endif
