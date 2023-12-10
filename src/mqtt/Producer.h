#pragma once

#include "MQTT.h"

class Producer
{
public:
    Producer(MQTT* mqtt) : _mqtt(mqtt) {}
    void init(const char* topic)
    {
        strcpy(_topic, topic);
    }

    const char* getTopicName() { return _topic; }

    bool publish(const char* payload)
    {
        return _mqtt->publish(_topic, payload, true);
    }

protected:
    MQTT* _mqtt = NULL;
    char _topic[MQTT_TOPIC_LEN] = {0};
};
