#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include "../../defines.h"
#include "Base.h"

class Button : Base
{
public:
    Button(
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

    void setCommandTopic(const char* commandTopic)
    {
        _commandTopic = commandTopic;
    }

    void setCommandTemplate(const char* commandTemplate)
    {
        _commandTemplate = commandTemplate;
    }

    void setPayloadPress(const char* payloadPress)
    {
        _payloadPress = payloadPress;
    }

private:
    const char* _deviceClass = NULL;
    const char* _commandTopic = NULL;
    const char* _commandTemplate = NULL;
    const char* _payloadPress = NULL;
};
