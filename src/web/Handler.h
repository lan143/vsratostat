#pragma once

#ifdef ESP32
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ConfigMgr.h>
#include <healthcheck.h>

#include "config.h"
#include "thermostat/boiler.h"
#include "wifi/wifi_mgr.h"

class Handler {
public:
    Handler(
        EDConfig::ConfigMgr<Config>* configMgr,
        Boiler* boiler,
        WiFiMgr* wifiMgr,
        EDHealthCheck::HealthCheck* healthcheck
    ) : _configMgr(configMgr), _boiler(boiler), _wifiMgr(wifiMgr), _healthcheck(healthcheck) {
        _server = new AsyncWebServer(80);
    }

    void init();

private:
    AsyncWebServer* _server;
    EDConfig::ConfigMgr<Config>* _configMgr;
    Boiler* _boiler;
    WiFiMgr* _wifiMgr;
    EDHealthCheck::HealthCheck* _healthcheck;
};