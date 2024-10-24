#pragma once

#include <Arduino.h>
#include <ConfigMgr.h>
#include <AsyncMqttClient.h>
#include <list>

#include "config/Config.h"
#include "utils/Utils.h"
#include "Consumer.h"

class MQTT {
public:
    MQTT(Config& config) : _config(config) { }
    void init();
    bool isConnected() { return _client.connected(); }
    bool publish(const char* topic, const char* payload, boolean retained);
    void subscribe(Consumer* consumer);
    void loop();

private:
    void onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

private:
    AsyncMqttClient _client;
    
    Config& _config;
    uint64_t _lastReconnectTime = 0;
    bool _isConfigured = false;
    std::list<Consumer*> _consumers;
};
