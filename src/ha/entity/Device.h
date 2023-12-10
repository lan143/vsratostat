#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>

class Device
{
public:
    void buildBaseField(JsonObject* entity);

    void setConfigurationURL(const char* configurationURL)
    {
        _configurationURL = configurationURL;
    }

    void setConnections(const char** connections, uint8_t size)
    {
        _connections = connections;
        _connectionsSize = size;
    }

    void setHWVersion(const char* hwVersion)
    {
        _hwVersion = hwVersion;
    }

    void setIdentifiers(const char** identifiers, uint8_t size)
    {
        _identifiers = identifiers;
        _identifiersSize = size;
    }

    void setManufacturer(const char* manufacturer)
    {
        _manufacturer = manufacturer;
    }

    void setModel(const char* model)
    {
        _model = model;
    }

    void setName(const char* name)
    {
        _name = name;
    }

    void setSuggestedArea(const char* suggestedArea)
    {
        _suggestedArea = suggestedArea;
    }

    void setSWVersion(const char* swVersion)
    {
        _swVersion = swVersion;
    }

    void setViaDevice(const char* viaDevice)
    {
        _viaDevice = viaDevice;
    }

private:
    const char* _configurationURL = NULL;
    const char** _connections = NULL;
    uint8_t _connectionsSize = 0;
    const char* _hwVersion = NULL;
    const char** _identifiers = NULL;
    uint8_t _identifiersSize = 0;
    const char* _manufacturer = NULL;
    const char* _model = NULL;
    const char* _name = NULL;
    const char* _suggestedArea = NULL;
    const char* _swVersion = NULL;
    const char* _viaDevice = NULL;
};