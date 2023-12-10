#pragma once

#include "../config/Config.h"

class Consumer
{
public:
    void init(const char* topic)
    {
        strcpy(_topic, topic);
    }
    virtual void consume(const char* payload) = 0;

    const char* getTopicName() { return _topic; }

protected:
    char _topic[MQTT_TOPIC_LEN] = {0};
};
