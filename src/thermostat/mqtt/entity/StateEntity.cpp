#include <ArduinoJson.h>
#include "StateEntity.h"
#include "defines.h"
#include "utils/Json.h"

std::string StateEntity::marshalJSON()
{
    std::string payload = buildJson([this](JsonObject entity) {
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
    });

    return payload;
}