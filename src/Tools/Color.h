#pragma once

// Forward declarations
namespace Tools {
    class Config;
}

namespace Tools {

enum ColorType : short {
    Result = 1,
    Command = 2
};

class Color {
public:
    /**
     * @brief Initializes color definitions from a config.
     *
     * @param config Reference to a config.
    */
    static void InitializeColorDefinitions(const Config& config);
};

}
