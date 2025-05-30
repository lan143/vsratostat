#pragma once

#include <OpenTherm.h>
#include <ConfigMgr.h>
#include <discovery.h>
#include <ready.h>

#include "config.h"
#include "thermostat/mqtt/producer/state_producer.h"
#include "boiler_state.h"

class Boiler : public EDHealthCheck::Ready
{
public:
    Boiler(
        EDConfig::ConfigMgr<Config>* configMgr,
        OpenTherm* openTherm,
        StateProducer* stateProducer
    );
    void init(EDHA::DiscoveryMgr* discoveryMgr, EDHA::Device* device);
    void loop();

    void setCentralHeatingOn(bool isOn);
    void setCentralHeatingSetPoint(float setPoint);
    void setHotWaterOn(bool isOn);
    void setHotWaterSetPoint(float setPoint);
    void resetErrors();

    BoilerState &getState() { return _current; }

    EDHealthCheck::ReadyResult ready();

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
    int openThermFaultCount = 0;

    StateProducer* _stateProducer;

    bool _resetInProgress = false;

    unsigned long _lastSendStateTime = 0;
    unsigned long _lastSyncOpenTermTime = 0;
    unsigned long _lastResetErrorsTime = 0;
};
