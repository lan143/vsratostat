#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include "../../defines.h"
#include "Base.h"

class BinarySensor : Base
{
public:
    BinarySensor(
        const char* name,
        Device* device,
        EntityCategory* entityCategory,
        const char* objectID,
        const char* uniqueID,
        bool forceUpdate
    ) : Base(name, device, entityCategory, objectID, uniqueID, forceUpdate) {}

    std::string marshalJSON();

    void setDeviceClass(const char* deviceClass)
    {
        _deviceClass = deviceClass;
    }

    void setStateTopic(const char* stateTopic)
    {
        _stateTopic = stateTopic;
    }

    void setValueTemplate(const char* valueTemplate)
    {
        _valueTemplate = valueTemplate;
    }

    void setPayloadOn(const char* payloadOn)
    {
        _payloadOn = payloadOn;
    }

    void setPayloadOff(const char* payloadOff)
    {
        _payloadOff = payloadOff;
    }

private:
    const char* _deviceClass = NULL;
    const char* _stateTopic = NULL;
    const char* _valueTemplate = NULL;
    const char* _payloadOn = NULL;
    const char* _payloadOff = NULL;
};
