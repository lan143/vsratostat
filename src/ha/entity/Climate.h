#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include <list>
#include "Base.h"

class Climate : Base
{
public:
    Climate(
        const char* name,
        Device* device,
        EntityCategory* entityCategory,
        const char* objectID,
        const char* uniqueID,
        bool forceUpdate
    ) : Base(name, device, entityCategory, objectID, uniqueID, forceUpdate) {}
    std::string marshalJSON();

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

    void setTemperatureCommandTemplate(const char* temperatureCommandTemplate)
    {
        _temperatureCommandTemplate = temperatureCommandTemplate;
    }

    void setTemperatureCommandTopic(const char* temperatureCommandTopic)
    {
        _temperatureCommandTopic = temperatureCommandTopic;
    }

    void setTemperatureStateTemplate(const char* temperatureStateTemplate)
    {
        _temperatureStateTemplate = temperatureStateTemplate;
    }

    void setTemperatureStateTopic(const char* temperatureStateTopic)
    {
        _temperatureStateTopic = temperatureStateTopic;
    }

    void setModes(const char** modes, uint8_t size)
    {
        _modes = modes;
        _modesSize = size;
    }

    void setPayloadOff(const char* payloadOff)
    {
        _payloadOff = payloadOff;
    }

    void setPayloadOn(const char* payloadOn)
    {
        _payloadOn = payloadOn;
    }

private:
    const char* _currentTemperatureTemplate = NULL;
    const char* _currentTemperatureTopic = NULL;
    float _minTemp;
    float _maxTemp;
    const char* _modeCommandTemplate = NULL;
    const char* _modeCommandTopic = NULL;
    const char* _modeStateTemplate = NULL;
    const char* _modeStateTopic = NULL;
    const char* _temperatureCommandTemplate = NULL;
    const char* _temperatureCommandTopic = NULL;
    const char* _temperatureStateTemplate = NULL;
    const char* _temperatureStateTopic = NULL;
    const char** _modes = NULL;
    uint8_t _modesSize = 0;
    const char* _payloadOff = NULL;
    const char* _payloadOn = NULL;
};
