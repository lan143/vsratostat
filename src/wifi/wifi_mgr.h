#pragma once

#include <Arduino.h>
#include <ConfigMgr.h>

#include "config.h"

class WiFiMgr {
public:
    WiFiMgr(Config& config) : _config(config) {}

    void init();
    bool isConnected() { return _isConnected; }

private:
    Config& _config;
    bool _isConnected;
};
