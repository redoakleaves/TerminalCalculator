#pragma once

#include <stdint.h>
#include <string_view>
#include <string>

namespace Tools {

class Entry {
public:
    const uint32_t identifier_m;

    std::string rawContent_m;

    Entry();

    void SetStylized(std::string_view stylized);
    std::string GetStylized() const;

    void SetResult(std::string_view result);
    std::string GetResult() const;

    void PrintStylized() const;
    void PrintResult() const;

    bool operator==(const Entry& second) const;
    bool operator!=(const Entry& second) const;

private:
    std::string stylizedContent_m;
    std::string result_m;
};

}
