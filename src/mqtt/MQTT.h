#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <list>
#include "../config/ConfigMgr.h"
#include "../utils/Utils.h"
#include "Consumer.h"

class MQTT {
public:
    MQTT(ConfigMgr* configMgr) : _configMgr(configMgr)
    {
        _clientID = getChipID();
        _client = PubSubClient(_wifiClient);
    }
    void init();
    void loop();
    void reconnect();
    bool publish(const char* topic, const char* payload, boolean retained)
    {
        if (_client.publish(topic, payload, retained)) {
            Serial.printf("successful mqtt publish. topic: %s\n", topic);
            return true;
        } else {
            Serial.printf("failed mqtt publish. topic: %s\n", topic);
            return false;
        }
    }
    void subscribe(Consumer* consumer)
    {
        Serial.printf("MQTT subscribe: %s\n", consumer->getTopicName());

        _consumers.push_back(consumer);

        if (_client.connected()) {
            _client.subscribe(consumer->getTopicName());
        }
    }

private:
    void callback(char *raw_topic, byte *raw_payload, unsigned int length);

private:
    const char* _clientID;
    WiFiClient _wifiClient;
    PubSubClient _client;
    ConfigMgr* _configMgr;
    uint64_t _lastReconnectTime = 0;
    bool _isConfigured;
    std::list<Consumer*> _consumers;
};
