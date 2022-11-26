#ifndef _FUNC_TEST_H_
#define _FUNC_TEST_H_

#include <gtest/gtest.h>

#include "global.h"
#include "parser/func.h"

TEST(FuncTest, HandleValidDef) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "f(x)=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 1);

    test_string = "f(x;y)=x^2+y";
    EXPECT_EQ(parse_func_def(entry, test_string), 1);
}

TEST(FuncTest, HandleInvalidDef) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "f(x)==x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);

    test_string = "f(x=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);

    test_string = "fx)=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);

    test_string = "f()=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);

    test_string = "f=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);
}

TEST(FuncTest, HandleConstOverride) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "sin(x)=x";
    EXPECT_EQ(parse_func_def(entry, test_string), 0);
}

TEST(FuncTest, HandleConstUsage) {
    Tools::Entry entry;
    std::string test_string;

    globalstate.use_deg = 1;

    test_string = "sin(90)";
    parse_const_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "1");

    test_string = "pow(2;3)";
    parse_const_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "8");

    test_string = "sin(90)+abs(-2)";
    parse_const_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "1+2");
}

TEST(FuncTest, HandleFuncUsage) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "f(x)=x^2";
    EXPECT_EQ(parse_func_def(entry, test_string, 1), 1);
    test_string = "h(x;y)=x/y";
    EXPECT_EQ(parse_func_def(entry, test_string, 1), 1);

    test_string = "f(3)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "(3^2)");

    test_string = "h(2;3)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "(2/3)");

    test_string = "f(3)+h(2;3)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "(3^2)+(2/3)");

    test_string = "2*f(3)+4";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "2*(3^2)+4");
}

TEST(FuncTest, HandleInvalidUsage) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "f()";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "f()");

    test_string = "f(2";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "f(2");

    test_string = "f2)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "f2)");

    test_string = "h(2)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "h(2)");
}

TEST(FuncTest, HandleOverwriteDef) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "f(3)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "(3^2)");

    test_string = "f(x;y)=x+y";
    EXPECT_EQ(parse_func_def(entry, test_string, 1), 1);

    test_string = "f(2;3)";
    parse_func_usage(entry, test_string);
    EXPECT_EQ(test_string, "(2+3)");
}

#endif
