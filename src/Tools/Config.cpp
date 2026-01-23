#include "Tools/Config.h"

#include <yaml-cpp/yaml.h>

#include <array>
#include <filesystem>
#include <format>
#include <stdlib.h>
#include <string_view>
#include <string>

#if defined(_WIN32)
static const std::array<const char*, 2> kConfigPaths = { "\\.tcconfig", "\\.config\\tc\\config.yml" };
#else
static const std::array<const char*, 2> kConfigPaths = { "/.tcconfig", "/.config/tc/config.yml" };
#endif

namespace Tools {

Config::Config() {
#if defined(_WIN32)
    size_t len = 0;

    char homedrive[3];
    getenv_s(&len, homedrive, 3, "HOMEDRIVE");

    char homepath[40];
    getenv_s(&len, homepath, 40, "HOMEPATH");

    homePath_m = std::filesystem::path(std::format("{}\\{}", homedrive, homepath));
#else
    homePath_m = std::filesystem::path(getenv("HOME"));
#endif
}

void Config::LoadConfig() {
    for (const auto& configPath : kConfigPaths) {
        const std::filesystem::path completePath =  homePath_m / std::filesystem::path(configPath);
        if (std::filesystem::exists(completePath)) {
            try {
                YAML::Node config = YAML::LoadFile(completePath.string());
                for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
                    ParseRecursive(it->first.as<std::string>(), it->second);
                }
            }
            catch (YAML::Exception e) { return; }
        }
    }
}

std::string Config::GetValue(std::string_view key) const {
    if (configStore_m.count(std::string(key))) {
        return configStore_m.at(std::string(key));
    } else {
        return "";
    }
}

void Config::ParseRecursive(const std::string& prefix, const YAML::Node& child) {
    if (child.IsMap()) {
        for (YAML::const_iterator it = child.begin(); it != child.end(); ++it) {
            ParseRecursive(prefix + "." + it->first.as<std::string>(), it->second);
        }
    } else if (child.IsScalar()) {
        configStore_m[prefix] = child.as<std::string>();
    }
}

}
