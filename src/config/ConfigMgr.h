#pragma once

#include "Config.h"

class ConfigMgr {
public:
    bool load();
    bool store();

    Config* getConfig() { return &_config; }

private:
    uint16_t calculateChecksum(Config &config);

private:
    Config _config;
};
