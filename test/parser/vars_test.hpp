#ifndef _VARS_TEST_H_
#define _VARS_TEST_H_

#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>

#include <gtest/gtest.h>

#include "tools/entry.h"
#include "parser/vars.h"

TEST(VarTest, HandleValidDef) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "A=2";
    EXPECT_EQ(parse_var_def(entry, test_string), 1);

    test_string = "A=-2";
    EXPECT_EQ(parse_var_def(entry, test_string), 1);

    test_string = "A=2.2";
    EXPECT_EQ(parse_var_def(entry, test_string), 1);

    test_string = "A=-2.2";
    EXPECT_EQ(parse_var_def(entry, test_string), 1);
}

TEST(VarTest, HandleInvalidDef) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "A==2";
    EXPECT_EQ(parse_var_def(entry, test_string), 0);

    test_string = "A=B";
    EXPECT_EQ(parse_var_def(entry, test_string), 0);
}

TEST(VarTest, HandleConstOverride) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "pi=3";
    EXPECT_EQ(parse_var_def(entry, test_string), 0);
}

TEST(VarTest, HandleConstUsage) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "1/pi+9";
    parse_var_usage(entry, test_string);
    std::stringstream stream;
    stream << "1/" << M_PI << "+9";
    EXPECT_EQ(test_string, stream.str());
}

TEST(VarTest, HandleVarUsage) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "A=2";
    parse_var_def(entry, test_string, 1);

    test_string = "3*A";
    parse_var_usage(entry, test_string);
    EXPECT_EQ(test_string, "3*2");

    test_string = "3*A+4";
    parse_var_usage(entry, test_string);
    EXPECT_EQ(test_string, "3*2+4");
}

TEST(VarTest, HandleInvalidUsage) {
    Tools::Entry entry;
    std::string test_string;

    test_string = "3*B+4";
    parse_var_usage(entry, test_string);
    EXPECT_EQ(test_string, "3*B+4");
}

#endif
