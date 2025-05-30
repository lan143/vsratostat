#pragma once

#include <Arduino.h>
#include <consumer.h>
#include "thermostat/boiler.h"

class CommandConsumer : public EDMQTT::Consumer
{
public:
    CommandConsumer(Boiler* boiler) : _boiler(boiler) {}
    void consume(std::string payload);

private:
    Boiler* _boiler;
};
