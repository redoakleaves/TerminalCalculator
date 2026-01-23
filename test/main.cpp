#include "Tools/Config.h"
#include "Tools/State.h"

#include "Parser/ArithmeticTest.hpp"
#include "Parser/CommandsTest.hpp"
#include "Parser/FuncTest.hpp"
#include "Parser/VarsTest.hpp"

#include "Parser/ParserTest.hpp"

#include <gtest/gtest.h>

Tools::Config globalConfig;
Tools::State globalState;

int main(int argc, char **argv) {
    // Init state
    globalState.Init();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
