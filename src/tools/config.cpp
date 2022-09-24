#include <stdlib.h>
#include <sys/stat.h>

#include "config.h"

const char* home = getenv("HOME");

void Config::parse_recursive(std::string prefix, YAML::Node child) {
    if (child.IsMap()) {
        for (YAML::const_iterator it = child.begin(); it != child.end(); ++it) {
            parse_recursive(prefix + "." + it->first.as<std::string>(), it->second);
        }
    } else if (child.IsScalar()) {
        config_store[prefix] = child.as<std::string>();
    }
}

void Config::load_config() {
    struct stat buffer;

    char complete_path[strlen(home) + 23];

    strcpy(complete_path, home);
    strcat(complete_path, "/.tcconfig");

    if (stat(complete_path, &buffer)) {
        strcpy(complete_path, home);
        strcat(complete_path, "/.config/tc/config.yml");

        if (stat(complete_path, &buffer))
            return;
    }

    YAML::Node config = YAML::LoadFile(complete_path);
    for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
        parse_recursive(it->first.as<std::string>(), it->second);
    }
}

std::string Config::get_value(const char* key) {
    return config_store[std::string(key)];
}
