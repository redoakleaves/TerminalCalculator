#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <cstring>

#include <yaml-cpp/yaml.h>

#include "config.h"

#if defined(_WIN32)
static const char* paths[2] = { "\\.tcconfig", "\\.config\\tc\\config.yml" };
#else
static const char* paths[2] = { "/.tcconfig", "/.config/tc/config.yml" };
#endif

Config::Config() {
#if defined(_WIN32)
    size_t len = 0;

    char homedrive[3];
    getenv_s(&len, homedrive, 3, "HOMEDRIVE");

    char homepath[40];
    getenv_s(&len, homepath, 40, "HOMEPATH");

    size_t home_path_size = strlen(homedrive) + strlen(homepath) + 1;
    home_path = (char*) malloc(home_path_size);
    if (home_path) {
        strcpy_s(home_path, home_path_size, homedrive);
        strcat_s(home_path, home_path_size, homepath);
    }
#else
    home_path = getenv("HOME");
#endif
}
Config::~Config() {
#if defined(_WIN32)
    free(home_path);
#endif
}

void Config::load_config() {
    struct stat buffer;

    size_t complete_path_size = strlen(home_path) + strlen(paths[0]) + 1;
    char* complete_path = (char*) malloc(complete_path_size);
    if (!complete_path)
        return;

#if defined(_WIN32)
    strcpy_s(complete_path, complete_path_size, home_path);
    strcat_s(complete_path, complete_path_size, paths[0]);
#else
    strcpy(complete_path, home_path);
    strcat(complete_path, paths[0]);
#endif

    if (stat(complete_path, &buffer)) {
        char* complete_path_temp = complete_path;
        complete_path_size = strlen(home_path) + strlen(paths[1]) + 1;
        complete_path = (char*) realloc(complete_path_temp, complete_path_size);
        if (!complete_path) {
            free(complete_path_temp);
            return;
        }

#if defined(_WIN32)
        strcpy_s(complete_path, complete_path_size, home_path);
        strcat_s(complete_path, complete_path_size, paths[1]);
#else
        strcpy(complete_path, home_path);
        strcat(complete_path, paths[1]);
#endif

        if (stat(complete_path, &buffer)) {
            free(complete_path);
            return;
        }
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
