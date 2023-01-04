#include <gtest/gtest.h>

#include "tools/config.h"
#include "tools/state.h"

#include "parser/arithmetic_test.hpp"
#include "parser/commands_test.hpp"
#include "parser/func_test.hpp"
#include "parser/vars_test.hpp"

#include "parser/parser_test.hpp"

Tools::Config globalconfig;
Tools::State globalstate;

int main(int argc, char **argv) {
    // Init state
    globalstate.Init();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
