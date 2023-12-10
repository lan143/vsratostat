#include <ArduinoJson.h>
#include "StateEntity.h"
#include "defines.h"

const char* StateEntity::marshalJSON()
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    JsonObject entity = jsonDoc.to<JsonObject>();

    entity[F("centralHeatingMode")] = _centralHeatingMode;
    entity[F("centralHeatingSetPoint")] = _centralHeatingSetPoint;
    entity[F("centralHeatingCurrentTemperature")] = _centralHeatingCurrentTemperature;
    entity[F("hotWaterMode")] = _hotWaterMode;
    entity[F("hotWaterSetPoint")] = _hotWaterSetPoint;
    entity[F("hotWaterCurrentTemperature")] = _hotWaterCurrentTemperature;
    entity[F("isHotWaterActive")] = _isHotWaterActive ? trueStr : falseStr;
    entity[F("isFlameActive")] = _isFlameActive ? trueStr : falseStr;
    entity[F("isFault")] = _isFault ? trueStr : falseStr;
    entity[F("modulation")] = _modulation;

    serializeJson(jsonDoc, _payload, JSON_DYNAMIC_MSG_BUFFER);

    return _payload;
}