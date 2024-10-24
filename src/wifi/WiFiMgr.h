#pragma once

#include <Arduino.h>
#include <ConfigMgr.h>

#include "config/ConfigEntity.h"

class WiFiMgr {
public:
    WiFiMgr(Config::ConfigMgr<ConfigEntity>* configMgr) : _configMgr(configMgr) {}

    void init();

private:
    Config::ConfigMgr<ConfigEntity>* _configMgr;
    bool _isConnected;
};
