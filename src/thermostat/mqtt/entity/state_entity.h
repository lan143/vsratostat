#pragma once

#include <Arduino.h>
#include <enum/modes.h>
#include "defines.h"

const char trueStr[] = "true";
const char falseStr[] = "false";

class StateEntity
{
public:
    StateEntity(
        EDHA::Mode centralHeatingMode,
        float centralHeatingSetPoint,
        float centralHeatingCurrentTemperature,
        EDHA::Mode hotWaterMode,
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

    std::string marshalJSON();

private:
    EDHA::Mode _centralHeatingMode;
    float _centralHeatingSetPoint;
    float _centralHeatingCurrentTemperature;

    EDHA::Mode _hotWaterMode;
    float _hotWaterSetPoint;
    float _hotWaterCurrentTemperature;

    bool _isHotWaterActive;
    bool _isFlameActive;
    bool _isFault;

    float _modulation;
};
