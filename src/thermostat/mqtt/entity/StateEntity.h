#pragma once

#include <Arduino.h>
#include "../../../defines.h"

const char trueStr[] = "true";
const char falseStr[] = "false";

class StateEntity
{
public:
    StateEntity(
        const char* centralHeatingMode,
        float centralHeatingSetPoint,
        float centralHeatingCurrentTemperature,
        const char* hotWaterMode,
        float hotWaterSetPoint,
        float hotWaterCurrentTemperature,
        bool isHotWaterActive,
        bool isFlameActive,
        bool isFault,
        float modulation
    ) : _centralHeatingMode(centralHeatingMode), _centralHeatingSetPoint(centralHeatingSetPoint),
        _centralHeatingCurrentTemperature(centralHeatingCurrentTemperature),
        _hotWaterMode(hotWaterMode),
        _hotWaterSetPoint(hotWaterSetPoint),
        _hotWaterCurrentTemperature(hotWaterCurrentTemperature),
        _isHotWaterActive(isHotWaterActive),
        _isFlameActive(isFlameActive),
        _isFault(isFault), _modulation(modulation) {}

    const char* marshalJSON();

private:
    const char* _centralHeatingMode;
    float _centralHeatingSetPoint;
    float _centralHeatingCurrentTemperature;

    const char* _hotWaterMode;
    float _hotWaterSetPoint;
    float _hotWaterCurrentTemperature;

    bool _isHotWaterActive;
    bool _isFlameActive;
    bool _isFault;

    float _modulation;

    char _payload[JSON_DYNAMIC_MSG_BUFFER] = {0};
};