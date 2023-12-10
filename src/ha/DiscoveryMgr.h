#pragma once

#include <Arduino.h>
#include "../config/ConfigMgr.h"
#include "../mqtt/MQTT.h"
#include "../thermostat/BoilerConstant.h"
#include "entity/BinarySensor.h"
#include "entity/Button.h"
#include "entity/Climate.h"
#include "entity/Device.h"
#include "entity/Sensor.h"
#include "entity/WaterHeater.h"

const char boilerLabel[] = "Boiler";
const char hotWaterLabel[] = "Hot water";
const char modulationLabel[] = "Modulation";
const char hotWaterActiveLabel[] = "Is hot water active";
const char flameSensorLabel[] = "Is flame";
const char faultSensorLabel[] = "Is fault";
const char resetButtonLabel[] = "Reset errors";

const char climateName[] = "climate";
const char boilerName[] = "boiler";
const char waterHeaterName[] = "water_heater";
const char hotWaterName[] = "hot_water";
const char sensorName[] = "sensor";
const char modulationSensorName[] = "modulation_sensor";
const char binarySensorName[] = "binary_sensor";
const char hotWaterSensorName[] = "hot_water_sensor";
const char flameSensorName[] = "flame_sensor";
const char faultSensorName[] = "fault_sensor";
const char buttonName[] = "button";
const char resetButtonName[] = "reset_button";

const char climateCurrentTemperatureTemplate[] = "{{ value_json.centralHeatingCurrentTemperature }}";
const char climateModeCommandTemple[] = "{\"centralHeatingMode\": \"{{ value }}\"}";
const char climateModeStateTemplate[] = "{{ value_json.centralHeatingMode }}";
const char climateTemperatureCommandTemplate[] = "{\"centralHeatingSetPoint\": {{ value }}}";
const char climateTemperatureStateTemplate[] = "{{ value_json.centralHeatingSetPoint }}";
const char climatePayloadOff[] = "false";
const char climatePayloadOn[] = "true";

const char hotWaterModeCommandTemplate[] = "{\"hotWaterMode\": \"{{ value }}\" }";
const char hotWaterModeStateTemplate[] = "{{ value_json.hotWaterMode }}";
const char hotWaterCurrentTemperatureTemplate[] = "{{ value_json.hotWaterCurrentTemperature }}";
const char hotWaterTemperatureCommandTemplate[] = "{\"hotWaterSetPoint\": {{ value }} }";

const char modulationSensorValueTemplate[] = "{{ value_json.modulation }}";

const char hotWaterSensorValueTemplate[] = "{{ value_json.isHotWaterActive }}";

const char flameSensorValueTemplate[] = "{{ value_json.isFlameActive }}";
const char faultSensorValueTemplate[] = "{{ value_json.isFault }}";

const char resetButtonCommandTemplate[] = "{\"reset\": {{ value }} }";

const char percentSymbol[] = "%";

#define TOPIC_SIZE 200

class DiscoveryMgr
{
public:
    DiscoveryMgr(
        ConfigMgr* configMgr,
        MQTT* mqtt
    ) : _configMgr(configMgr), _mqtt(mqtt)
    {
        _isSend = false;
        _lastSendTime = 0;
    }
    void init();
    void loop();

private:
    void sendDiscovery();
    void updateTopicName(char* topic, const char* prefix, const char* type, const char* uniqueID, const char* name);

    void buildClimateConfig(Config* config, Device* device, const char* chipID);
    void buildHotWaterConfig(Config* config, Device* device, const char* chipID);
    void buildModulationSensorConfig(Config* config, Device* device, const char* chipID);
    void buildHotWaterSensorConfig(Config* config, Device* device, const char* chipID);
    void buildFlameSensorConfig(Config* config, Device* device, const char* chipID);
    void buildFaultSensorConfig(Config* config, Device* device, const char* chipID);
    void buildResetButtonConfig(Config* config, Device* device, const char* chipID);

private:
    char _climateTopic[TOPIC_SIZE] = {0};
    char _hotWaterTopic[TOPIC_SIZE] = {0};
    char _modulationSensorTopic[TOPIC_SIZE] = {0};
    char _hotWaterSensorTopic[TOPIC_SIZE] = {0};
    char _flameSensorTopic[TOPIC_SIZE] = {0};
    char _faultSensorTopic[TOPIC_SIZE] = {0};
    char _resetButtonTopic[TOPIC_SIZE] = {0};

    char _climateConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _hotWaterConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _modulationSensorConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _hotWaterSensorConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _flameSensorConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _faultSensorConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};
    char _resetButtonConfigJSON[JSON_DYNAMIC_MSG_BUFFER] = {0};

private:
    ConfigMgr* _configMgr;
    MQTT* _mqtt;

private:
    bool _isSend;
    uint64_t _lastSendTime;
};
