#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <cstring>

#include <yaml-cpp/yaml.h>

#include "tools/config.h"

#if defined(_WIN32)
static const char* paths[2] = { "\\.tcconfig", "\\.config\\tc\\config.yml" };
#else
static const char* paths[2] = { "/.tcconfig", "/.config/tc/config.yml" };
#endif

namespace Tools
{
    Config::Config() {
    #if defined(_WIN32)
        size_t len = 0;

        char homedrive[3];
        getenv_s(&len, homedrive, 3, "HOMEDRIVE");

        char homepath[40];
        getenv_s(&len, homepath, 40, "HOMEPATH");

        size_t homePathSize = strlen(homedrive) + strlen(homepath) + 1;
        m_HomePath = (char*) malloc(homePathSize);
        if (m_HomePath) {
            strcpy_s(m_HomePath, homePathSize, homedrive);
            strcat_s(m_HomePath, homePathSize, homepath);
        }
    #else
        m_HomePath = getenv("HOME");
    #endif
    }
    Config::~Config() {
    #if defined(_WIN32)
        free(m_HomePath);
    #endif
    }

    void Config::LoadConfig() {
        struct stat buffer;

        size_t completePathSize = strlen(m_HomePath) + strlen(paths[0]) + 1;
        char* completePath = (char*) malloc(completePathSize);
        if (!completePath)
            return;

    #if defined(_WIN32)
        strcpy_s(completePath, completePathSize, m_HomePath);
        strcat_s(completePath, completePathSize, paths[0]);
    #else
        strcpy(completePath, m_HomePath);
        strcat(completePath, paths[0]);
    #endif

        if (stat(completePath, &buffer)) {
            char* complete_path_temp = completePath;
            completePathSize = strlen(m_HomePath) + strlen(paths[1]) + 1;
            completePath = (char*) realloc(complete_path_temp, completePathSize);
            if (!completePath) {
                free(complete_path_temp);
                return;
            }

    #if defined(_WIN32)
            strcpy_s(completePath, completePathSize, m_HomePath);
            strcat_s(completePath, completePathSize, paths[1]);
    #else
            strcpy(completePath, m_HomePath);
            strcat(completePath, paths[1]);
    #endif

            if (stat(completePath, &buffer)) {
                free(completePath);
                return;
            }
        }

        try {
            YAML::Node config = YAML::LoadFile(completePath);
            for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
                ParseRecursive(it->first.as<std::string>(), it->second);
            }
        }
        catch (YAML::Exception e) { }

        free(completePath);
    }

    std::string Config::GetValue(const char* key) {
        return m_ConfigStore[std::string(key)];
    }

    void Config::ParseRecursive(std::string prefix, YAML::Node child) {
        if (child.IsMap()) {
            for (YAML::const_iterator it = child.begin(); it != child.end(); ++it) {
                ParseRecursive(prefix + "." + it->first.as<std::string>(), it->second);
            }
        } else if (child.IsScalar()) {
            m_ConfigStore[prefix] = child.as<std::string>();
        }
    }
}
