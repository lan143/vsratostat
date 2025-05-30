#pragma once

#include <Arduino.h>
#include <ConfigMgr.h>
#include <list>

#include "config.h"

typedef std::function<void(bool isConnected)> ConnectFunction;

class WiFiMgr {
public:
    WiFiMgr(Config& config) : _config(config) {}

    void init();
    bool isConnected() { return _isConnected; }
    void OnConnect(ConnectFunction fn) { _connectCallbacks.push_back(fn); }

private:
    Config& _config;
    bool _isConnected;
    std::list<ConnectFunction> _connectCallbacks;
};
