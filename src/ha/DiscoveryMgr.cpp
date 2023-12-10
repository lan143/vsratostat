#include <list>
#include "../defines.h"
#include "../utils/Utils.h"
#include "DiscoveryMgr.h"
#include "enum/EntityCategory.h"

void DiscoveryMgr::init()
{
    Config* config = _configMgr->getConfig();
    const char* macAddress = getMacAddress();
    const char* chipID = getChipID();

    const char* identifiers[1];
    identifiers[0] = macAddress;

    Device device;
    device.setHWVersion(hwVersion);
    device.setSWVersion(fwVersion);
    device.setModel(model);
    device.setName(name);
    device.setIdentifiers(identifiers, 1);
    device.setManufacturer(manufacturer);

    buildClimateConfig(config, &device, chipID);
    buildHotWaterConfig(config, &device, chipID);
    buildModulationSensorConfig(config, &device, chipID);
    buildHotWaterSensorConfig(config, &device, chipID);
    buildFlameSensorConfig(config, &device, chipID);
    buildFaultSensorConfig(config, &device, chipID);
    buildResetButtonConfig(config, &device, chipID);

    Serial.printf("_climateConfigJSON: %s\n", _climateConfigJSON);
    Serial.printf("_hotWaterConfigJSON: %s\n", _hotWaterConfigJSON);
    Serial.printf("_modulationSensorConfigJSON: %s\n", _modulationSensorConfigJSON);
    Serial.printf("_hotWaterSensorConfigJSON: %s\n", _hotWaterSensorConfigJSON);
    Serial.printf("_flameSensorConfigJSON: %s\n", _flameSensorConfigJSON);
    Serial.printf("_faultSensorConfigJSON: %s\n", _faultSensorConfigJSON);
    Serial.printf("_resetButtonConfigJSON: %s\n", _resetButtonConfigJSON);
}

void DiscoveryMgr::loop()
{
    if (!_isSend && (_lastSendTime + 1000) < millis()) {
        sendDiscovery();
        _lastSendTime = millis();
    }
}

void DiscoveryMgr::sendDiscovery()
{
    if (
        !_mqtt->publish(_climateTopic, _climateConfigJSON, true)
        || !_mqtt->publish(_hotWaterTopic, _hotWaterConfigJSON, true)
        || !_mqtt->publish(_modulationSensorTopic, _modulationSensorConfigJSON, true)
        || !_mqtt->publish(_hotWaterSensorTopic, _hotWaterSensorConfigJSON, true)
        || !_mqtt->publish(_flameSensorTopic, _flameSensorConfigJSON, true)
        || !_mqtt->publish(_faultSensorTopic, _faultSensorConfigJSON, true)
        || !_mqtt->publish(_resetButtonTopic, _resetButtonConfigJSON, true)
    ) {
        return;
    }

    _isSend = true;
}

void DiscoveryMgr::updateTopicName(char* topic, const char* prefix, const char* type, const char* uniqueID, const char* name)
{
    snprintf(topic, TOPIC_SIZE, "%s/%s/%s/%s/config", prefix, type, uniqueID, name);
}

void DiscoveryMgr::buildClimateConfig(Config* config, Device* device, const char* chipID)
{
    const char* climateModes[2];
    climateModes[0] = &modeOff[0];
    climateModes[1] = &modeHeat[0];

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_boiler_vsratostat", chipID);

    updateTopicName(
        _climateTopic,
        config->mqttHADiscoveryPrefix,
        climateName,
        chipID,
        boilerName
    );
    Climate climateConfig(
            &boilerLabel[0],
            device,
            entityCategoryConfig,
            &boilerName[0],
            &uniqueID[0],
            true
    );
    climateConfig.setCurrentTemperatureTemplate(climateCurrentTemperatureTemplate);
    climateConfig.setCurrentTemperatureTopic(config->mqttStateTopic);
    climateConfig.setMinTemp(CENTRAL_HEATING_MIN_TEMP);
    climateConfig.setMaxTemp(CENTRAL_HEATING_MAX_TEMP);
    climateConfig.setModeCommandTemplate(climateModeCommandTemple);
    climateConfig.setModeCommandTopic(config->mqttCommandTopic);
    climateConfig.setModeStateTemplate(climateModeStateTemplate);
    climateConfig.setModeStateTopic(config->mqttStateTopic);
    climateConfig.setTemperatureCommandTemplate(climateTemperatureCommandTemplate);
    climateConfig.setTemperatureCommandTopic(config->mqttCommandTopic);
    climateConfig.setTemperatureStateTemplate(climateTemperatureStateTemplate);
    climateConfig.setTemperatureStateTopic(config->mqttStateTopic);
    climateConfig.setModes(climateModes, 2);
    climateConfig.setPayloadOff(climatePayloadOff);
    climateConfig.setPayloadOn(climatePayloadOn);

    strcpy(_climateConfigJSON, climateConfig.marshalJSON());
}

void DiscoveryMgr::buildHotWaterConfig(Config* config, Device* device, const char* chipID)
{
    const char* hotWaterModes[2];
    hotWaterModes[0] = &modeOff[0];
    hotWaterModes[1] = &modeGAS[0];

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_hot_water_vsratostat", chipID);

    updateTopicName(&_hotWaterTopic[0], config->mqttHADiscoveryPrefix, waterHeaterName, chipID, hotWaterName);
    WaterHeater hotWaterConfig(
            hotWaterLabel,
            device,
            entityCategoryConfig,
            hotWaterName,
            uniqueID,
            true
    );
    hotWaterConfig.setModeCommandTemplate(hotWaterModeCommandTemplate);
    hotWaterConfig.setModeCommandTopic(config->mqttCommandTopic);
    hotWaterConfig.setModeStateTemplate(hotWaterModeStateTemplate);
    hotWaterConfig.setModeStateTopic(config->mqttStateTopic);
    hotWaterConfig.setCurrentTemperatureTemplate(hotWaterCurrentTemperatureTemplate);
    hotWaterConfig.setCurrentTemperatureTopic(config->mqttStateTopic);
    hotWaterConfig.setMinTemp(WATER_HEATING_MIN_TEMP);
    hotWaterConfig.setMaxTemp(WATER_HEATING_MAX_TEMP);
    hotWaterConfig.setTemperatureCommandTemplate(hotWaterTemperatureCommandTemplate);
    hotWaterConfig.setTemperatureCommandTopic(config->mqttCommandTopic);
    hotWaterConfig.setModes(hotWaterModes, 2);

    strcpy(_hotWaterConfigJSON, hotWaterConfig.marshalJSON());
}

void DiscoveryMgr::buildModulationSensorConfig(Config* config, Device* device, const char* chipID)
{
    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_modulation_vsratostat", chipID);

    updateTopicName(&_modulationSensorTopic[0], config->mqttHADiscoveryPrefix, &sensorName[0], chipID, &modulationSensorName[0]);
    Sensor modulationSensorConfig(
            &modulationLabel[0],
            device,
            entityCategoryDiagnostic,
            &modulationSensorName[0],
            &uniqueID[0],
            true
    );
    modulationSensorConfig.setStateTopic(config->mqttStateTopic);
    modulationSensorConfig.setValueTemplate(modulationSensorValueTemplate);
    modulationSensorConfig.setUnitOfMeasurement(percentSymbol);

    strcpy(_modulationSensorConfigJSON, modulationSensorConfig.marshalJSON());
}

void DiscoveryMgr::buildHotWaterSensorConfig(Config* config, Device* device, const char* chipID)
{
    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_hot_water_sensor_vsratostat", chipID);

    updateTopicName(_hotWaterSensorTopic, config->mqttHADiscoveryPrefix, binarySensorName, chipID, hotWaterSensorName);
    BinarySensor hotWaterSensorConfig(
            hotWaterActiveLabel,
            device,
            entityCategoryDiagnostic,
            hotWaterSensorName,
            uniqueID,
            true
    );
    hotWaterSensorConfig.setStateTopic(config->mqttStateTopic);
    hotWaterSensorConfig.setValueTemplate(hotWaterSensorValueTemplate);
    hotWaterSensorConfig.setPayloadOn(climatePayloadOn);
    hotWaterSensorConfig.setPayloadOff(climatePayloadOff);

    strcpy(_hotWaterSensorConfigJSON, hotWaterSensorConfig.marshalJSON());
}

void DiscoveryMgr::buildFlameSensorConfig(Config* config, Device* device, const char* chipID)
{
    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_flame_sensor_vsratostat", chipID);

    updateTopicName(_flameSensorTopic, config->mqttHADiscoveryPrefix, binarySensorName, chipID, flameSensorName);
    BinarySensor flameSensorConfig(
            flameSensorLabel,
            device,
            entityCategoryDiagnostic,
            flameSensorName,
            uniqueID,
            true
    );
    flameSensorConfig.setDeviceClass(deviceClassBinarySensorHeat);
    flameSensorConfig.setStateTopic(config->mqttStateTopic);
    flameSensorConfig.setValueTemplate(flameSensorValueTemplate);
    flameSensorConfig.setPayloadOn(climatePayloadOn);
    flameSensorConfig.setPayloadOff(climatePayloadOff);

    strcpy(_flameSensorConfigJSON, flameSensorConfig.marshalJSON());
}

void DiscoveryMgr::buildFaultSensorConfig(Config* config, Device* device, const char* chipID)
{
    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_fault_sensor_vsratostat", chipID);

    updateTopicName(&_faultSensorTopic[0], config->mqttHADiscoveryPrefix, &binarySensorName[0], chipID, &faultSensorName[0]);
    BinarySensor faultSensorConfig(
            &faultSensorLabel[0],
            device,
            entityCategoryDiagnostic,
            &faultSensorName[0],
            &uniqueID[0],
            true
    );
    faultSensorConfig.setDeviceClass(deviceClassBinarySensorProblem);
    faultSensorConfig.setStateTopic(config->mqttStateTopic);
    faultSensorConfig.setValueTemplate(faultSensorValueTemplate);
    faultSensorConfig.setPayloadOn(climatePayloadOn);
    faultSensorConfig.setPayloadOff(climatePayloadOff);

    strcpy(_faultSensorConfigJSON, faultSensorConfig.marshalJSON());
}

void DiscoveryMgr::buildResetButtonConfig(Config* config, Device* device, const char* chipID)
{
    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_reset_errors_vsratostat", chipID);

    updateTopicName(&_resetButtonTopic[0], config->mqttHADiscoveryPrefix, &buttonName[0], chipID, &resetButtonName[0]);
    Button resetButtonConfig(
            &resetButtonLabel[0],
            device,
            entityCategoryDiagnostic,
            &resetButtonName[0],
            uniqueID,
            true
    );
    resetButtonConfig.setDeviceClass(deviceClassButtonRestart);
    resetButtonConfig.setCommandTopic(config->mqttCommandTopic);
    resetButtonConfig.setCommandTemplate(resetButtonCommandTemplate);
    resetButtonConfig.setPayloadPress(climatePayloadOn);

    strcpy(_resetButtonConfigJSON, resetButtonConfig.marshalJSON());
}
