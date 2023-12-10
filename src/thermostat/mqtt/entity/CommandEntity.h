#pragma once

#include <Arduino.h>

class CommandEntity
{
public:
    bool unmarshalJSON(const char* data);

    bool hasCentralHeatingMode() { return _hasCentralHeatingMode; }
    const char* getCentralHeatingMode() { return _centralHeatingMode; }

    bool hasCentralHeatingSetPoint() { return _hasCentralHeatingSetPoint; }
    float getCentralHeatingSetPoint() { return _centralHeatingSetPoint; }

    bool hasReset() { return _hasReset; }
    bool isReset() { return _reset; }

    bool hasHotWaterMode() { return _hasHotWaterMode; }
    const char* getHotWaterMode() { return _hotWaterMode; }

    bool hasHotWaterSetPoint() { return _hasHotWaterSetPoint; }
    float getHotWaterSetPoint() { return _hotWaterSetPoint; }

private:
    bool _hasCentralHeatingMode = false;
    const char* _centralHeatingMode;

    bool _hasCentralHeatingSetPoint = false;
    float _centralHeatingSetPoint;

    bool _hasReset = false;
    bool _reset;

    bool _hasHotWaterMode = false;
    const char* _hotWaterMode;

    bool _hasHotWaterSetPoint = false;
    float _hotWaterSetPoint;
};
