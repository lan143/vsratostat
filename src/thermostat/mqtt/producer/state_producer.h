#pragma once

#include <Arduino.h>
#include <mqtt.h>
#include <producer.h>
#include "thermostat/mqtt/entity/state_entity.h"

class StateProducer : public EDMQTT::Producer
{
public:
    StateProducer(EDMQTT::MQTT* mqtt) : Producer(mqtt) {}

    void publish(StateEntity* state);
};
