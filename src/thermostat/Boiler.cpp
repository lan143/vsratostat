#include <Arduino.h>
#include <Utils.h>
#include <enum/modes.h>
#include <enum/device_class.h>
#include "config.h"
#include "defines.h"
#include "Boiler.h"

Boiler::Boiler(
    EDConfig::ConfigMgr<Config>* configMgr,
    OpenTherm* openTherm,
    StateProducer* stateProducer
) : _configMgr(configMgr), _openTherm(openTherm), _stateProducer(stateProducer)
{
    _resetInProgress = false;
    _lastSendStateTime = 0;
    _lastSyncOpenTermTime = 0;
    _lastResetErrorsTime = 0;
}

void Boiler::init(EDHA::DiscoveryMgr* discoveryMgr, EDHA::Device* device)
{
    pinMode(RESET_RELAY_PIN, OUTPUT);
    digitalWrite(RESET_RELAY_PIN, HIGH);

    Config& config = _configMgr->getConfig();
    _current.setCentralHeatingOn(config.isCentralHeatingEnabled);
    _current.setCentralHeatingSetPoint(config.centralHeatingSetPoint);
    _current.setHotWaterOn(config.isHotWaterEnabled);
    _current.setHotWaterSetPoint(config.hotWaterSetPoint);

    const char* chipID = EDUtils::getChipID();

    std::list<EDHA::Mode> climateModes;
    climateModes.push_back(EDHA::MODE_OFF);
    climateModes.push_back(EDHA::MODE_HEAT);

    discoveryMgr->addClimate(
        device,
        "Climate",
        "climate",
        EDUtils::formatString("%s_boiler_vsratostat", chipID)
    )
        ->setCurrentTemperatureTemplate("{{ value_json.centralHeatingCurrentTemperature }}")
        ->setCurrentTemperatureTopic(config.mqttStateTopic)
        ->setMinTemp(CENTRAL_HEATING_MIN_TEMP)
        ->setMaxTemp(CENTRAL_HEATING_MAX_TEMP)
        ->setModeCommandTemplate("{\"centralHeatingMode\": \"{{ value }}\"}")
        ->setModeCommandTopic(config.mqttCommandTopic)
        ->setModeStateTemplate("{{ value_json.centralHeatingMode }}")
        ->setModeStateTopic(config.mqttStateTopic)
        ->setTemperatureCommandTemplate("{\"centralHeatingSetPoint\": {{ value }}}")
        ->setTemperatureCommandTopic(config.mqttCommandTopic)
        ->setTemperatureStateTemplate("{{ value_json.centralHeatingSetPoint }}")
        ->setTemperatureStateTopic(config.mqttStateTopic)
        ->setModes(climateModes)
        ->setPayloadOff("false")
        ->setPayloadOn("true")
        ->setActionTopic(config.mqttStateTopic)
        ->setActionTemplate("{{ value_json.centralHeatingState }}");

    std::list<EDHA::Mode> hotWaterModes;
    hotWaterModes.push_back(EDHA::MODE_OFF);
    hotWaterModes.push_back(EDHA::MODE_GAS);

    discoveryMgr->addWaterHeater(
        device,
        "Hot water",
        "hot_water",
        EDUtils::formatString("%s_hot_water_vsratostat", chipID)
    )
        ->setModeCommandTemplate("{\"hotWaterMode\": \"{{ value }}\" }")
        ->setModeCommandTopic(config.mqttCommandTopic)
        ->setModeStateTemplate("{{ value_json.hotWaterMode }}")
        ->setModeStateTopic(config.mqttStateTopic)
        ->setCurrentTemperatureTemplate("{{ value_json.hotWaterCurrentTemperature }}")
        ->setCurrentTemperatureTopic(config.mqttStateTopic)
        ->setMinTemp(WATER_HEATING_MIN_TEMP)
        ->setMaxTemp(WATER_HEATING_MAX_TEMP)
        ->setTemperatureCommandTemplate("{\"hotWaterSetPoint\": {{ value }} }")
        ->setTemperatureCommandTopic(config.mqttCommandTopic)
        ->setModes(hotWaterModes)
        ->setInitial(_current.getHotWaterSetPoint())
        ->setPrecision(1.0f);

    discoveryMgr->addSensor(
        device,
        "Modulation",
        "modulation",
        EDUtils::formatString("%s_modulation_vsratostat", chipID)
    )
        ->setStateTopic(config.mqttStateTopic)
        ->setValueTemplate("{{ value_json.modulation }}")
        ->setUnitOfMeasurement("%");

    discoveryMgr->addBinarySensor(
        device,
        "Is hot water active",
        "hot_water_sensor",
        EDUtils::formatString("%s_hot_water_sensor_vsratostat", chipID)
    )
        ->setStateTopic(config.mqttStateTopic)
        ->setValueTemplate("{{ value_json.isHotWaterActive }}")
        ->setPayloadOn("true")
        ->setPayloadOff("false");

    discoveryMgr->addBinarySensor(
        device,
        "Is flame",
        "flame_sensor",
        EDUtils::formatString("%s_flame_sensor_vsratostat", chipID)
    )
        ->setDeviceClass(EDHA::deviceClassBinarySensorHeat)
        ->setStateTopic(config.mqttStateTopic)
        ->setValueTemplate("{{ value_json.isFlameActive }}")
        ->setPayloadOn("true")
        ->setPayloadOff("false");

    discoveryMgr->addBinarySensor(
        device,
        "Is fault",
        "fault_sensor",
        EDUtils::formatString("%s_fault_sensor_vsratostat", chipID)
    )
        ->setDeviceClass(EDHA::deviceClassBinarySensorProblem)
        ->setStateTopic(config.mqttStateTopic)
        ->setValueTemplate("{{ value_json.isFault }}")
        ->setPayloadOn("true")
        ->setPayloadOff("false");

    discoveryMgr->addButton(
        device,
        "Reset errors",
        "reset_button",
        EDUtils::formatString("%s_reset_errors_vsratostat", chipID)
    )
        ->setDeviceClass(EDHA::deviceClassButtonRestart)
        ->setCommandTopic(config.mqttCommandTopic)
        ->setCommandTemplate("{\"reset\": {{ value }} }")
        ->setPayloadPress("true");
}

void Boiler::loop()
{
    if ((_lastSendStateTime + 1000) < millis()) {
        if (_current != _prev) {
            StateEntity state(
                _current.isCentralHeatingOn() ? EDHA::MODE_HEAT : EDHA::MODE_OFF,
                _current.getCentralHeatingSetPoint(),
                _current.getCentralHeatingCurrentTemperature(),
                _current.isHotWaterOn() ? EDHA::MODE_GAS : EDHA::MODE_OFF,
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
        bool openThermIsFault = false;
        unsigned long request = _openTherm->buildSetBoilerStatusRequest(
            _current.isCentralHeatingOn(),
            _current.isHotWaterOn()
        );
        unsigned long response = _openTherm->sendRequest(request);

        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setHotWaterActive(_openTherm->isHotWaterActive(response));
            _current.setFault(_openTherm->isFault(response));
            _current.setFlameOn(_openTherm->isFlameOn(response));
        } else {
            openThermIsFault = true;
        }

        _openTherm->setBoilerTemperature(_current.getCentralHeatingSetPoint());
        if (_openTherm->getLastResponseStatus() != OpenThermResponseStatus::SUCCESS) {
            _current.setCentralHeatingSetPoint(_prev.getCentralHeatingSetPoint());
            openThermIsFault = true;
        }

        _openTherm->setDHWSetpoint(_current.getHotWaterSetPoint());
        if (_openTherm->getLastResponseStatus() != OpenThermResponseStatus::SUCCESS) {
            _current.setHotWaterSetPoint(_prev.getHotWaterSetPoint());
            openThermIsFault = true;
        }
 
        float centralHeatingCurrentTemperature = _openTherm->getBoilerTemperature();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setCentralHeatingCurrentTemperature(centralHeatingCurrentTemperature);
        } else {
            openThermIsFault = true;
        }

        float hotWaterCurrentTemperature = _openTherm->getDHWTemperature();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            if (!_current.isHotWaterActive()) {
                hotWaterCurrentTemperature = 0;
            }

            _current.setHotWaterCurrentTemperature(hotWaterCurrentTemperature);
        } else {
            openThermIsFault = true;
        }

        float modulation = _openTherm->getModulation();
        if (_openTherm->getLastResponseStatus() == OpenThermResponseStatus::SUCCESS) {
            _current.setModulation(modulation);
        } else {
            openThermIsFault = true;
        }

        if (openThermIsFault) {
            openThermFaultCount++;
        } else {
            openThermFaultCount = 0;
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

    Config& config = _configMgr->getConfig();
    config.isCentralHeatingEnabled = isOn;
    _configMgr->store();
}

void Boiler::setCentralHeatingSetPoint(float setPoint)
{
    _current.setCentralHeatingSetPoint(setPoint);

    Config& config = _configMgr->getConfig();
    config.centralHeatingSetPoint = setPoint;
    _configMgr->store();
}

void Boiler::setHotWaterOn(bool isOn)
{
    _current.setHotWaterOn(isOn);

    Config& config = _configMgr->getConfig();
    config.isHotWaterEnabled = isOn;
    _configMgr->store();
}

void Boiler::setHotWaterSetPoint(float setPoint)
{
    _current.setHotWaterSetPoint(setPoint);

    Config& config = _configMgr->getConfig();
    config.hotWaterSetPoint = setPoint;
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

EDHealthCheck::ReadyResult Boiler::ready()
{
    bool ready = true;
    std::string message = "";
    if (getState().isFault()) {
        ready = false;
        message = "boiler is in fault state";
    } else if (openThermFaultCount >= 5) {
        ready = false;
        message = "can't connect to boiler over OpenTherm";
    }

    return EDHealthCheck::ReadyResult(ready, message);
}