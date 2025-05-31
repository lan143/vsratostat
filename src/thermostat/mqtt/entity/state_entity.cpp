#include <Json.h>
#include "defines.h"
#include "state_entity.h"

std::string StateEntity::marshalJSON()
{
    std::string payload = EDUtils::buildJson([this](JsonObject entity) {
        switch (_centralHeatingMode) {
            case EDHA::MODE_OFF:
                entity[F("centralHeatingMode")] = "off";
                break;
            case EDHA::MODE_HEAT:
                entity[F("centralHeatingMode")] = "heat";
                break;
        }
        
        entity[F("centralHeatingState")] = _isFlameActive ? "heating" : "idle";
        entity[F("centralHeatingSetPoint")] = _centralHeatingSetPoint;
        entity[F("centralHeatingCurrentTemperature")] = _centralHeatingCurrentTemperature;

        entity[F("hotWaterMode")] = EDHA::mapMode(_hotWaterMode);
        entity[F("hotWaterSetPoint")] = _hotWaterSetPoint;
        entity[F("hotWaterCurrentTemperature")] = _hotWaterCurrentTemperature;
        entity[F("isHotWaterActive")] = _isHotWaterActive ? trueStr : falseStr;
        entity[F("isFlameActive")] = _isFlameActive ? trueStr : falseStr;
        entity[F("isFault")] = _isFault ? trueStr : falseStr;
        entity[F("modulation")] = _modulation;
    });

    return payload;
}
