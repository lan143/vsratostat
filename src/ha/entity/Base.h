#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Device.h"
#include "../../defines.h"
#include "../enum/DeviceClass.h"
#include "../enum/EntityCategory.h"

class Base
{
public:
    Base(
        const char* name,
        Device* device,
        EntityCategory* entityCategory,
        const char* objectID,
        const char* uniqueID,
        bool forceUpdate
    ) : _name(name), _device(device), _entityCategory(entityCategory), _objectID(objectID),
        _uniqueID(uniqueID), _forceUpdate(forceUpdate) {}

protected:
    void buildBaseField(JsonObject* entity);

private:
    const char* _name = NULL;
    Device* _device = NULL;
    EntityCategory* _entityCategory = NULL;
    const char* _objectID = NULL;
    const char* _uniqueID = NULL;
    bool _forceUpdate = NULL;
};
