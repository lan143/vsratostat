#include <list>
#include "../defines.h"
#include "../utils/Utils.h"
#include "DiscoveryMgr.h"
#include "enum/EntityCategory.h"

void DiscoveryMgr::init()
{
    _isSend = false;
    _lastSendTime = 0;

    _device.setHWVersion(deviceHWVersion);
    _device.setSWVersion(deviceFWVersion);
    _device.setModel(deviceModel);
    _device.setName(deviceName);
    _device.addIdentifier(getMacAddress());
    _device.setManufacturer(deviceManufacturer);
}

void DiscoveryMgr::loop()
{
    if (!_isSend && (_lastSendTime + 10000) < millis()) {
        sendDiscovery();
        _lastSendTime = millis();
    }
}

void DiscoveryMgr::sendDiscovery()
{
    ConfigEntity& config = _configMgr->getConfig();
    if (!config.mqttIsHADiscovery) {
        Serial.println("discovery: skip send, disabled in config");
        _isSend = true;
        return;
    }

    if (!_mqtt->isConnected()) {
        Serial.println("discovery: send failed, mqtt is not connected");
        return;
    }

    if (
        !publishClimateConfig()
        || !publishHotWaterConfig()
        || !publishModulationSensorConfig()
        || !publishHotWaterSensorConfig()
        || !publishFlameSensorConfig()
        || !publishFaultSensorConfig()
        || !publishResetButtonConfig()
    ) {
        Serial.println("discovery: send failed, some errors");

        return;
    }

    Serial.println("discovery: successfuly sent");

    _isSend = true;
}

std::string DiscoveryMgr::buildTopicName(const char* prefix, const char* type, const char* uniqueID, const char* name)
{
    char topic[TOPIC_SIZE] = {0};
    snprintf(topic, TOPIC_SIZE, "%s/%s/%s/%s/config", prefix, type, uniqueID, name);

    return std::string(topic);
}

bool DiscoveryMgr::publishClimateConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    const char* climateModes[2];
    climateModes[0] = &modeOff[0];
    climateModes[1] = &modeHeat[0];

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_boiler_vsratostat", chipID);

    Climate climateConfig(
            &boilerLabel[0],
            &_device,
            entityCategoryConfig,
            &boilerName[0],
            &uniqueID[0],
            true
    );
    climateConfig.setCurrentTemperatureTemplate(climateCurrentTemperatureTemplate);
    climateConfig.setCurrentTemperatureTopic(config.mqttStateTopic);
    climateConfig.setMinTemp(CENTRAL_HEATING_MIN_TEMP);
    climateConfig.setMaxTemp(CENTRAL_HEATING_MAX_TEMP);
    climateConfig.setModeCommandTemplate(climateModeCommandTemple);
    climateConfig.setModeCommandTopic(config.mqttCommandTopic);
    climateConfig.setModeStateTemplate(climateModeStateTemplate);
    climateConfig.setModeStateTopic(config.mqttStateTopic);
    climateConfig.setTemperatureCommandTemplate(climateTemperatureCommandTemplate);
    climateConfig.setTemperatureCommandTopic(config.mqttCommandTopic);
    climateConfig.setTemperatureStateTemplate(climateTemperatureStateTemplate);
    climateConfig.setTemperatureStateTopic(config.mqttStateTopic);
    climateConfig.setModes(climateModes, 2);
    climateConfig.setPayloadOff(climatePayloadOff);
    climateConfig.setPayloadOn(climatePayloadOn);

    std::string climateConfigJSON = climateConfig.marshalJSON();
    std::string topicName = buildTopicName(
        config.mqttHADiscoveryPrefix,
        climateName,
        chipID,
        boilerName
    );

    return _mqtt->publish(topicName.c_str(), climateConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishHotWaterConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    const char* hotWaterModes[2];
    hotWaterModes[0] = &modeOff[0];
    hotWaterModes[1] = &modeGAS[0];

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_hot_water_vsratostat", chipID);

    WaterHeater hotWaterConfig(
            hotWaterLabel,
            &_device,
            entityCategoryConfig,
            hotWaterName,
            uniqueID,
            true
    );
    hotWaterConfig.setModeCommandTemplate(hotWaterModeCommandTemplate);
    hotWaterConfig.setModeCommandTopic(config.mqttCommandTopic);
    hotWaterConfig.setModeStateTemplate(hotWaterModeStateTemplate);
    hotWaterConfig.setModeStateTopic(config.mqttStateTopic);
    hotWaterConfig.setCurrentTemperatureTemplate(hotWaterCurrentTemperatureTemplate);
    hotWaterConfig.setCurrentTemperatureTopic(config.mqttStateTopic);
    hotWaterConfig.setMinTemp(WATER_HEATING_MIN_TEMP);
    hotWaterConfig.setMaxTemp(WATER_HEATING_MAX_TEMP);
    hotWaterConfig.setTemperatureCommandTemplate(hotWaterTemperatureCommandTemplate);
    hotWaterConfig.setTemperatureCommandTopic(config.mqttCommandTopic);
    hotWaterConfig.setModes(hotWaterModes, 2);

    std::string hotWaterConfigJSON = hotWaterConfig.marshalJSON();
    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, waterHeaterName, chipID, hotWaterName);

    return _mqtt->publish(topicName.c_str(), hotWaterConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishModulationSensorConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_modulation_vsratostat", chipID);

    Sensor modulationSensorConfig(
            &modulationLabel[0],
            &_device,
            entityCategoryDiagnostic,
            &modulationSensorName[0],
            &uniqueID[0],
            true
    );
    modulationSensorConfig.setStateTopic(config.mqttStateTopic);
    modulationSensorConfig.setValueTemplate(modulationSensorValueTemplate);
    modulationSensorConfig.setUnitOfMeasurement(percentSymbol);

    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, &sensorName[0], chipID, &modulationSensorName[0]);
    std::string modulationSensorConfigJSON = modulationSensorConfig.marshalJSON();

    return _mqtt->publish(topicName.c_str(), modulationSensorConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishHotWaterSensorConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_hot_water_sensor_vsratostat", chipID);

    BinarySensor hotWaterSensorConfig(
            hotWaterActiveLabel,
            &_device,
            entityCategoryDiagnostic,
            hotWaterSensorName,
            uniqueID,
            true
    );
    hotWaterSensorConfig.setStateTopic(config.mqttStateTopic);
    hotWaterSensorConfig.setValueTemplate(hotWaterSensorValueTemplate);
    hotWaterSensorConfig.setPayloadOn(climatePayloadOn);
    hotWaterSensorConfig.setPayloadOff(climatePayloadOff);

    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, binarySensorName, chipID, hotWaterSensorName);
    std::string hotWaterSensorConfigJSON = hotWaterSensorConfig.marshalJSON();

    return _mqtt->publish(topicName.c_str(), hotWaterSensorConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishFlameSensorConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_flame_sensor_vsratostat", chipID);

    BinarySensor flameSensorConfig(
            flameSensorLabel,
            &_device,
            entityCategoryDiagnostic,
            flameSensorName,
            uniqueID,
            true
    );
    flameSensorConfig.setDeviceClass(deviceClassBinarySensorHeat);
    flameSensorConfig.setStateTopic(config.mqttStateTopic);
    flameSensorConfig.setValueTemplate(flameSensorValueTemplate);
    flameSensorConfig.setPayloadOn(climatePayloadOn);
    flameSensorConfig.setPayloadOff(climatePayloadOff);

    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, binarySensorName, chipID, flameSensorName);
    std::string flameSensorConfigJSON = flameSensorConfig.marshalJSON();

    return _mqtt->publish(topicName.c_str(), flameSensorConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishFaultSensorConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_fault_sensor_vsratostat", chipID);

    BinarySensor faultSensorConfig(
            &faultSensorLabel[0],
            &_device,
            entityCategoryDiagnostic,
            &faultSensorName[0],
            &uniqueID[0],
            true
    );
    faultSensorConfig.setDeviceClass(deviceClassBinarySensorProblem);
    faultSensorConfig.setStateTopic(config.mqttStateTopic);
    faultSensorConfig.setValueTemplate(faultSensorValueTemplate);
    faultSensorConfig.setPayloadOn(climatePayloadOn);
    faultSensorConfig.setPayloadOff(climatePayloadOff);

    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, &binarySensorName[0], chipID, &faultSensorName[0]);
    std::string faultSensorConfigJSON = faultSensorConfig.marshalJSON();

    return _mqtt->publish(topicName.c_str(), faultSensorConfigJSON.c_str(), true);
}

bool DiscoveryMgr::publishResetButtonConfig()
{
    ConfigEntity& config = _configMgr->getConfig();
    const char* chipID = getChipID();

    char uniqueID[50];
    snprintf(uniqueID, 50, "%s_reset_errors_vsratostat", chipID);

    Button resetButtonConfig(
            &resetButtonLabel[0],
            &_device,
            entityCategoryDiagnostic,
            &resetButtonName[0],
            uniqueID,
            true
    );
    resetButtonConfig.setDeviceClass(deviceClassButtonRestart);
    resetButtonConfig.setCommandTopic(config.mqttCommandTopic);
    resetButtonConfig.setCommandTemplate(resetButtonCommandTemplate);
    resetButtonConfig.setPayloadPress(climatePayloadOn);

    std::string topicName = buildTopicName(config.mqttHADiscoveryPrefix, &buttonName[0], chipID, &resetButtonName[0]);
    std::string resetButtonConfigJSON = resetButtonConfig.marshalJSON();

    return _mqtt->publish(topicName.c_str(), resetButtonConfigJSON.c_str(), true);
}
