#include "../../defines.h"
#include "Device.h"

void Device::buildBaseField(JsonObject entity)
{
    if (_configurationURL) {
        (entity)[F("configuration_url")] = _configurationURL;
    }

    if (_connectionsSize > 0) {
        JsonArray connections = entity.createNestedArray(F("connections"));
        for (uint8_t i = 0; i < _connectionsSize; i++) {
            connections[i] = _connections[i];
        }
    }

    if (_hwVersion) {
        (entity)[F("hw_version")] = _hwVersion;
    }

    if (_identifiers.size() > 0) {
        JsonArray identifiers = entity.createNestedArray(F("identifiers"));
        int i = 0;
        for (auto identifier : _identifiers) {
            identifiers[i] = identifier;
            i++;
        }
    }

    if (_manufacturer) {
        (entity)[F("manufacturer")] = _manufacturer;
    }

    if (_model) {
        (entity)[F("model")] = _model;
    }

    if (_name) {
        (entity)[F("name")] = _name;
    }

    if (_suggestedArea) {
        (entity)[F("suggested_area")] = _suggestedArea;
    }

    if (_swVersion) {
        (entity)[F("sw_version")] = _swVersion;
    }

    if (_viaDevice) {
        (entity)[F("via_device")] = _viaDevice;
    }
}