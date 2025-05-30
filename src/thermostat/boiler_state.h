#pragma once

class BoilerState
{
public:
    bool isCentralHeatingOn() { return _centralHeatingIsOn; }
    float getCentralHeatingSetPoint() { return _centralHeatingSetPoint; }
    float getCentralHeatingCurrentTemperature() { return _centralHeatingCurrentTemperature; }
    bool isHotWaterOn() { return _hotWaterIsOn; }
    float getHotWaterSetPoint() { return _hotWaterSetPoint; }
    float getHotWaterCurrentTemperature() { return _hotWaterCurrentTemperature; }
    bool isHotWaterActive() { return _hotWaterActive; }
    bool isFault() { return _isFault; }
    bool isFlameOn() { return _isFlameOn; }
    float getModulation() { return _modulation; }

    void setCentralHeatingOn(bool val) { _centralHeatingIsOn = val; }
    void setCentralHeatingSetPoint(float val) { _centralHeatingSetPoint = val; }
    void setCentralHeatingCurrentTemperature(float val) { _centralHeatingCurrentTemperature = val; }
    void setHotWaterOn(bool val) { _hotWaterIsOn = val; }
    void setHotWaterSetPoint(float val) { _hotWaterSetPoint = val; }
    void setHotWaterCurrentTemperature(float val) { _hotWaterCurrentTemperature = val; }
    void setHotWaterActive(bool val) { _hotWaterActive = val; }
    void setFault(bool val) { _isFault = val; }
    void setFlameOn(bool val) { _isFlameOn = val; }
    void setModulation(float val) { _modulation = val; }

    bool operator==(BoilerState& other);
    bool operator!=(BoilerState& other)
    {
        return !(*this == other);
    }

private:
    bool _centralHeatingIsOn = false;
    float _centralHeatingSetPoint = 0;
    float _centralHeatingCurrentTemperature = 0;
    bool _hotWaterIsOn = false;
    float _hotWaterSetPoint = 0;
    float _hotWaterCurrentTemperature = 0;
    bool _hotWaterActive = false;
    bool _isFault = false;
    bool _isFlameOn = false;
    float _modulation = 0;
};