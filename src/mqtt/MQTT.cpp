#include <Arduino.h>
#include <ArduinoJson.h>
#include "utils/Utils.h"
#include "MQTT.h"

void MQTT::init()
{
    ConfigEntity& config = _configMgr->getConfig();

    if (strlen(config.mqttHost) == 0) {
        return;
    }

    Serial.println("mqtt: init");

    _client.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
        onMessage(topic, payload, properties, len, index, total);
    });
    _client.onConnect([this](bool sessionPresent) {
        Serial.println("mqtt: connected");

        for (auto consumer : _consumers) {
            _client.subscribe(consumer->getTopicName(), 1);
        }
    });
    _client.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
        Serial.println("mqtt: disconnected");
    });
    _client.setServer(config.mqttHost, config.mqttPort);

    _isConfigured = true;
}

void MQTT::onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    std::string correctPayload(payload, len);
    Serial.printf(
        "mqtt: receive message, topic: %s, payload: %s, len: %d, total: %d, index: %d\n",
        topic,
        correctPayload.c_str(),
        len,
        total,
        index);

    for (auto consumer : _consumers) {
        if (strcmp(consumer->getTopicName(), topic) == 0) {
            consumer->consume(correctPayload);
        }
    }
}

void MQTT::loop()
{
    if (_client.connected() || WiFi.status() != WL_CONNECTED) {
        return;
    }

    if (_lastReconnectTime + 1000 < millis()) {
        Serial.println("mqtt: connect...");
        _client.connect();
        _lastReconnectTime = millis();
    }
}
