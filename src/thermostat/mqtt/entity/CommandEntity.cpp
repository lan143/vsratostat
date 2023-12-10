#include <ArduinoJson.h>

#include "defines.h"
#include "CommandEntity.h"

bool CommandEntity::unmarshalJSON(const char* data)
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    auto error = deserializeJson(jsonDoc, data);
    if (error) {
        return false;
    }

    JsonObject root = jsonDoc.as<JsonObject>();

    if (root.containsKey(F("centralHeatingMode"))) {
        _hasCentralHeatingMode = true;
        _centralHeatingMode = root[F("centralHeatingMode")].as<const char*>();
    }

    if (root.containsKey(F("centralHeatingSetPoint"))) {
        _hasCentralHeatingSetPoint = true;
        _centralHeatingSetPoint = root[F("centralHeatingSetPoint")].as<float>();
    }

    if (root.containsKey(F("reset"))) {
        _hasReset = true;
        _reset = root[F("reset")].as<bool>();
    }

    if (root.containsKey(F("hotWaterMode"))) {
        _hasHotWaterMode = true;
        _hotWaterMode = root[F("hotWaterMode")].as<const char*>();
    }

    if (root.containsKey(F("hotWaterSetPoint"))) {
        _hasHotWaterSetPoint = true;
        _hotWaterSetPoint = root[F("hotWaterSetPoint")].as<float>();
    }

    return true;
}