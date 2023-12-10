#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include <list>
#include "Base.h"

class WaterHeater : Base
{
public:
    WaterHeater(
        const char* name,
        Device* device,
        EntityCategory* entityCategory,
        const char* objectID,
        const char* uniqueID,
        bool forceUpdate
    ) : Base(name, device, entityCategory, objectID, uniqueID, forceUpdate) {}
    const char* marshalJSON();

    void setModeCommandTemplate(const char* modeCommandTemplate)
    {
        _modeCommandTemplate = modeCommandTemplate;
    }

    void setModeCommandTopic(const char* modeCommandTopic)
    {
        _modeCommandTopic = modeCommandTopic;
    }

    void setModeStateTemplate(const char* modeStateTemplate)
    {
        _modeStateTemplate = modeStateTemplate;
    }

    void setModeStateTopic(const char* modeStateTopic)
    {
        _modeStateTopic = modeStateTopic;
    }

    void setCurrentTemperatureTemplate(const char* currentTemperatureTemplate)
    {
        _currentTemperatureTemplate = currentTemperatureTemplate;
    }

    void setCurrentTemperatureTopic(const char* currentTemperatureTopic)
    {
        _currentTemperatureTopic = currentTemperatureTopic;
    }

    void setMinTemp(float minTemp)
    {
        _minTemp = minTemp;
    }

    void setMaxTemp(float maxTemp)
    {
        _maxTemp = maxTemp;
    }

    void setTemperatureCommandTemplate(const char* temperatureCommandTemplate)
    {
        _temperatureCommandTemplate = temperatureCommandTemplate;
    }

    void setTemperatureCommandTopic(const char* temperatureCommandTopic)
    {
        _temperatureCommandTopic = temperatureCommandTopic;
    }

    void setModes(const char** modes, uint8_t size)
    {
        _modes = modes;
        _modesSize = size;
    }

private:
    const char* _modeCommandTemplate = NULL;
    const char* _modeCommandTopic = NULL;
    const char* _modeStateTemplate = NULL;
    const char* _modeStateTopic = NULL;
    const char* _currentTemperatureTemplate = NULL;
    const char* _currentTemperatureTopic = NULL;
    float _minTemp;
    float _maxTemp;
    const char* _temperatureCommandTemplate = NULL;
    const char* _temperatureCommandTopic = NULL;
    const char** _modes = NULL;
    uint8_t _modesSize = 0;

    char _payload[JSON_DYNAMIC_MSG_BUFFER] = {0};
};
