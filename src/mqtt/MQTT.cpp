#include <Arduino.h>
#include <ArduinoJson.h>
#include "utils/Utils.h"
#include "MQTT.h"

void MQTT::init()
{
    Config* config = _configMgr->getConfig();

    if (strlen(config->mqttHost) == 0) {
        return;
    }

    _client.setCallback([&](char *raw_topic, byte *raw_payload, unsigned int length) {
        callback(raw_topic, raw_payload, length);
    });

    _wifiClient.setTimeout(MQTT_SOCKET_TIMEOUT * 1000);
    _client.setBufferSize(2048);
    _client.setSocketTimeout(MQTT_SOCKET_TIMEOUT);
    _client.setServer(config->mqttHost, config->mqttPort);

    _isConfigured = true;
}

void MQTT::reconnect()
{
    if (_client.connected()) {
        return;
    }

    Config* config = _configMgr->getConfig();

    if (_lastReconnectTime + 1000 < millis()) {
        if (_client.connect(_clientID, config->mqttLogin, config->mqttPassword)) {
            for (auto consumer : _consumers) {
                _client.subscribe(consumer->getTopicName());
            }
        }
        _lastReconnectTime = millis();
    }
}

void MQTT::callback(char *raw_topic, byte *raw_payload, unsigned int length)
{
    char* payload = (char*)malloc(length+1);

    unsigned int i;
    for (i = 0; i < length; i++) {
        payload[i] = (char)raw_payload[i];
    }
    payload[i] = 0;

    Serial.printf("MQTT receive message. Topic: %s, Payload: %s\n", raw_topic, payload);

    for (auto consumer : _consumers) {
        if (strcmp(consumer->getTopicName(), raw_topic) == 0) {
            consumer->consume(payload);
        }
    }

    free((void*)payload);
}

void MQTT::loop()
{
    if (!_isConfigured) {
        return;
    }

    _client.loop();
    reconnect();
}
