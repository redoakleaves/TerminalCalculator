#include <stdlib.h>
#include <sys/stat.h>
#include <cstring>

#include "config.h"

#if defined(_WIN64)
const char* paths[2] = { "\\.tcconfig", "\\.config\\tc\\config.yml" };
#else
const char* paths[2] = { "/.tcconfig", "/.config/tc/config.yml" };
#endif

Config::Config() {
#if defined(_WIN64)
    const char* homedrive = getenv("HOMEDRIVE");
    const char* homepath = getenv("HOMEPATH");
    home_path = (char*) malloc(strlen(homedrive) + strlen(homepath) + 1);
    strcpy(home_path, homedrive);
    strcat(home_path, homepath);
#else
    home_path = getenv("HOME");
#endif
}
Config::~Config() {
#if defined(_WIN64)
    free(home_path);
#endif
}

void Config::load_config() {
    struct stat buffer;

    char* complete_path = (char*) malloc(strlen(home_path) + strlen(paths[0]) + 1);
    strcpy(complete_path, home_path);
    strcat(complete_path, paths[0]);

    if (stat(complete_path, &buffer)) {
        complete_path = (char*) realloc(complete_path, strlen(home_path) + strlen(paths[1]) + 1);
        strcpy(complete_path, home_path);
        strcat(complete_path, paths[1]);

        if (stat(complete_path, &buffer))
            return;
    }

    try {
        YAML::Node config = YAML::LoadFile(complete_path);
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            parse_recursive(it->first.as<std::string>(), it->second);
        }
    }
    catch (YAML::Exception e) { }

    free(complete_path);
}

std::string Config::get_value(const char* key) {
    return config_store[std::string(key)];
}

void Config::parse_recursive(std::string prefix, YAML::Node child) {
    if (child.IsMap()) {
        for (YAML::const_iterator it = child.begin(); it != child.end(); ++it) {
            parse_recursive(prefix + "." + it->first.as<std::string>(), it->second);
        }
    } else if (child.IsScalar()) {
        config_store[prefix] = child.as<std::string>();
    }
}
