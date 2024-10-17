#pragma once

#include <Arduino.h>
#include "mqtt/Consumer.h"
#include "../../Boiler.h"

class CommandConsumer : public Consumer
{
public:
    CommandConsumer(Boiler* boiler) : _boiler(boiler) {}
    void consume(std::string payload);

private:
    Boiler* _boiler;
};