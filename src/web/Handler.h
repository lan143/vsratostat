#pragma once

#ifdef ESP32
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ConfigMgr.h>

#include "config/ConfigEntity.h"
#include "thermostat/Boiler.h"

class Handler {
public:
    Handler(Config::ConfigMgr<ConfigEntity>* configMgr, Boiler* boiler) : _configMgr(configMgr), _boiler(boiler) {
        _server = new AsyncWebServer(80);
    }

    void init();

private:
    AsyncWebServer* _server;
    Config::ConfigMgr<ConfigEntity>* _configMgr;
    Boiler* _boiler;
};