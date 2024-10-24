#pragma once

#include <OpenTherm.h>
#include <ConfigMgr.h>

#include "config/Config.h"
#include "mqtt/Producer/StateProducer.h"
#include "BoilerState.h"
#include "BoilerConstant.h"

class Boiler
{
public:
    Boiler(
        EDConfig::ConfigMgr<Config>* configMgr,
        OpenTherm* openTherm,
        StateProducer* stateProducer
    );
    void init();
    void loop();

    void setCentralHeatingOn(bool isOn);
    void setCentralHeatingSetPoint(float setPoint);
    void setHotWaterOn(bool isOn);
    void setHotWaterSetPoint(float setPoint);
    void resetErrors();

    BoilerState &getState() { return _current; }

private:
    void hvacInit();
    void sensorsInit();
    void waterHeaterInit();
    void buttonsInit();

private:
    BoilerState _current;
    BoilerState _prev;

private:
    EDConfig::ConfigMgr<Config>* _configMgr;
    OpenTherm* _openTherm;
    StateProducer* _stateProducer;

    bool _resetInProgress = false;

    unsigned long _lastSendStateTime = 0;
    unsigned long _lastSyncOpenTermTime = 0;
    unsigned long _lastResetErrorsTime = 0;
};
