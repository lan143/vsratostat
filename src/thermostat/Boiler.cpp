#include <Arduino.h>

#include "defines.h"
#include "Boiler.h"
#include "utils/Utils.h"

Boiler::Boiler(
    ConfigMgr* configMgr,
    OpenTherm* openTherm,
    StateProducer* stateProducer
) : _configMgr(configMgr), _openTherm(openTherm), _stateProducer(stateProducer)
{
    _resetInProgress = false;
    _lastSendStateTime = 0;
    _lastSyncOpenTermTime = 0;
    _lastResetErrorsTime = 0;
}

void Boiler::init()
{
    pinMode(RESET_RELAY_PIN, OUTPUT);
    digitalWrite(RESET_RELAY_PIN, HIGH);

    Config* config = _configMgr->getConfig();
    _current.setCentralHeatingOn(config->isCentralHeatingEnabled);
    _current.setCentralHeatingSetPoint(config->centralHeatingSetPoint);
    _current.setHotWaterOn(config->isHotWaterEnabled);
    _current.setHotWaterSetPoint(config->hotWaterSetPoint);
}

void Boiler::loop()
{
    if ((_lastSendStateTime + 1000) < millis()) {
        if (_current != _prev) {
            StateEntity state(
                _current.isCentralHeatingOn() ? &modeHeat[0] : &modeOff[0],
                _current.getCentralHeatingSetPoint(),
                _current.getCentralHeatingCurrentTemperature(),
                _current.isHotWaterOn() ? &modeGAS[0] : &modeOff[0],
                _current.getHotWaterSetPoint(),
                _current.getHotWaterCurrentTemperature(),
                _current.isHotWaterActive(),
                _current.isFlameOn(),
                _current.isFault(),
                _current.getModulation()
            );
            _stateProducer->publish(&state);
            _prev = _current;
        }

        _lastSendStateTime = millis();
    }

    if ((_lastSyncOpenTermTime + 2000) < millis()) {
        unsigned long request = _openTherm->buildSetBoilerStatusRequest(
            _current.isCentralHeatingOn(),
            _current.isHotWaterOn()
        );
        unsigned long response = _openTherm->sendRequest(request);

        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setHotWaterActive(_openTherm->isHotWaterActive(response));
            _current.setFault(_openTherm->isFault(response));
            _current.setFlameOn(_openTherm->isFlameOn(response));
        }

        _openTherm->setBoilerTemperature(_current.getCentralHeatingSetPoint());
        if (_openTherm->getLastResponseStatus() != OpenThermResponseStatus::SUCCESS) {
            _current.setCentralHeatingSetPoint(_prev.getCentralHeatingSetPoint());
        }

        _openTherm->setDHWSetpoint(_current.getHotWaterSetPoint());
        if (_openTherm->getLastResponseStatus() != OpenThermResponseStatus::SUCCESS) {
            _current.setHotWaterSetPoint(_prev.getHotWaterSetPoint());
        }

        float centralHeatingCurrentTemperature = _openTherm->getBoilerTemperature();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setCentralHeatingCurrentTemperature(centralHeatingCurrentTemperature);
        }

        float hotWaterCurrentTemperature = _openTherm->getDHWTemperature();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            if (!_current.isHotWaterActive()) {
                hotWaterCurrentTemperature = WATER_HEATING_MIN_TEMP;
            }

            _current.setHotWaterCurrentTemperature(hotWaterCurrentTemperature);
        }

        float modulation = _openTherm->getModulation();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setModulation(modulation);
        }

        _lastSyncOpenTermTime = millis();
    }

    if (_resetInProgress && (_lastResetErrorsTime + 500) < millis()) {
        digitalWrite(RESET_RELAY_PIN, HIGH);
        _resetInProgress = false;
        _lastResetErrorsTime = millis();
    }
}

void Boiler::setCentralHeatingOn(bool isOn)
{
    _current.setCentralHeatingOn(isOn);

    Config* config = _configMgr->getConfig();
    config->isCentralHeatingEnabled = isOn;
    _configMgr->store();
}

void Boiler::setCentralHeatingSetPoint(float setPoint)
{
    _current.setCentralHeatingSetPoint(setPoint);

    Config* config = _configMgr->getConfig();
    config->centralHeatingSetPoint = setPoint;
    _configMgr->store();
}

void Boiler::setHotWaterOn(bool isOn)
{
    _current.setHotWaterOn(isOn);

    Config* config = _configMgr->getConfig();
    config->isHotWaterEnabled = isOn;
    _configMgr->store();
}

void Boiler::setHotWaterSetPoint(float setPoint)
{
    _current.setHotWaterSetPoint(setPoint);

    Config* config = _configMgr->getConfig();
    config->hotWaterSetPoint = setPoint;
    _configMgr->store();
}

void Boiler::resetErrors()
{
    if (_resetInProgress) {
        return;
    }

    _resetInProgress = true;
    digitalWrite(RESET_RELAY_PIN, LOW);
    _lastResetErrorsTime = millis();
}
