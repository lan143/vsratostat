#pragma once

#include <Arduino.h>
#include "../config/ConfigMgr.h"

class WiFiMgr {
public:
    WiFiMgr(ConfigMgr* configMgr) : _configMgr(configMgr) {}

    void init();

private:
    void onEvent(WiFiEvent_t event, WiFiEventInfo_t info);

private:
    ConfigMgr* _configMgr;
    bool _isConnected;
};
