#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include "Base.h"

class Sensor : Base
{
public:
    Sensor(
        const char* name,
        Device* device,
        EntityCategory* entityCategory,
        const char* objectID,
        const char* uniqueID,
        bool forceUpdate
    ) : Base(name, device, entityCategory, objectID, uniqueID, forceUpdate) {}
    std::string marshalJSON();

    void setStateTopic(const char* stateTopic)
    {
        _stateTopic = stateTopic;
    }

    void setValueTemplate(const char* valueTemplate)
    {
        _valueTemplate = valueTemplate;
    }

    void setUnitOfMeasurement(const char* unitOfMeasurement)
    {
        _unitOfMeasurement = unitOfMeasurement;
    }

private:
    const char* _stateTopic = NULL;
    const char* _valueTemplate = NULL;
    const char* _unitOfMeasurement = NULL;
};
