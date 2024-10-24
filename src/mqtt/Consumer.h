#pragma once

#include "config/ConfigEntity.h"

class Consumer
{
public:
    void init(const char* topic)
    {
        strcpy(_topic, topic);
    }
    virtual void consume(std::string payload) = 0;

    const char* getTopicName() { return _topic; }

protected:
    char _topic[MQTT_TOPIC_LEN] = {0};
};
