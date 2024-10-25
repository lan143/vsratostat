#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_log.h>
#include <Utils.h>
#include "MQTT.h"

void MQTT::init()
{
    ESP_LOGI("mqtt", "init start");

    if (strlen(_config.mqttHost) == 0) {
        ESP_LOGI("mqtt", "host is empty. stop.");

        return;
    }

    _client.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
        onMessage(topic, payload, properties, len, index, total);
    });
    _client.onConnect([this](bool sessionPresent) {
        ESP_LOGI("mqtt", "connected");

        for (auto consumer : _consumers) {
            _client.subscribe(consumer->getTopicName(), 1);
        }
    });
    _client.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
        ESP_LOGI("mqtt", "disconnected");
    });
    _client.setServer(_config.mqttHost, _config.mqttPort);

    _isConfigured = true;
}

void MQTT::onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    std::string correctPayload(payload, len);
    ESP_LOGD(
        "mqtt",
        "receive message, topic: %s, payload: %s, len: %d, total: %d, index: %d",
        topic,
        correctPayload.c_str(),
        len,
        total,
        index
    );

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
        ESP_LOGI("mqtt", "connect...");
        _client.connect();
        _lastReconnectTime = millis();
    }
}

bool MQTT::publish(const char* topic, const char* payload, boolean retained)
{
    if (!isConnected()) {
        ESP_LOGE("mqtt", "can't publish message. mqtt is not connected yet.");
        return false;
    }

    ESP_LOGD("mqtt", "publish message, topic: %s, payload: %s", topic, payload);

    if (_client.publish(topic, 1, retained, payload)) {
        ESP_LOGD("mqtt", "publish successful");

        return true;
    } else {
        ESP_LOGE("mqtt", "publish failed");

        return false;
    }
}

void MQTT::subscribe(Consumer* consumer)
{
    ESP_LOGD("mqtt", "subscribe, topic: %s", consumer->getTopicName());

    _consumers.push_back(consumer);

    if (_isConfigured && _client.connected()) {
        _client.subscribe(consumer->getTopicName(), 1);
    }
}
