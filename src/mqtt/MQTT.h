#pragma once

#include <Arduino.h>
#include <list>
#include <AsyncMqttClient.h>
#include "../config/ConfigMgr.h"
#include "../utils/Utils.h"
#include "Consumer.h"

class MQTT {
public:
    MQTT(ConfigMgr* configMgr) : _configMgr(configMgr) { }
    void init();
    bool isConnected() { return _client.connected(); }
    bool publish(const char* topic, const char* payload, boolean retained)
    {
        if (!isConnected()) {
            return false;
        }

        if (_client.publish(topic, 1, retained, payload)) {
            Serial.printf("mqtt: publish successful, topic: %s, payload: %s\n", topic, payload);
            return true;
        } else {
            Serial.printf("mqtt: publish failed, topic: %s, payload: %s\n", topic, payload);
            return false;
        }
    }
    void subscribe(Consumer* consumer)
    {
        Serial.printf("mqtt: subscribe, topic: %s\n", consumer->getTopicName());

        _consumers.push_back(consumer);

        if (_isConfigured && _client.connected()) {
            _client.subscribe(consumer->getTopicName(), 1);
        }
    }
    void loop();

private:
    void onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

private:
    AsyncMqttClient _client;
    
    ConfigMgr* _configMgr;
    uint64_t _lastReconnectTime = 0;
    bool _isConfigured = false;
    std::list<Consumer*> _consumers;
};
