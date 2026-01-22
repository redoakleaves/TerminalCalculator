#include "tools/config.h"
#include "tools/state.h"

#include "Parser/ArithmeticTest.hpp"
#include "Parser/CommandsTest.hpp"
#include "Parser/FuncTest.hpp"
#include "Parser/VarsTest.hpp"

#include "Parser/ParserTest.hpp"

#include <gtest/gtest.h>

Tools::Config globalconfig;
Tools::State globalstate;

int main(int argc, char **argv) {
    // Init state
    globalstate.Init();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
