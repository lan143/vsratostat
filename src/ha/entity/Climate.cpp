#include "../../defines.h"
#include "Climate.h"

const char* Climate::marshalJSON()
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    JsonObject entity = jsonDoc.to<JsonObject>();
    buildBaseField(&entity);

    if (_currentTemperatureTemplate) {
        entity[F("current_temperature_template")] = _currentTemperatureTemplate;
    }

    if (_currentTemperatureTopic) {
        entity[F("current_temperature_topic")] = _currentTemperatureTopic;
    }

    if (_minTemp > 0) {
        entity[F("min_temp")] = _minTemp;
    }

    if (_maxTemp > 0) {
        entity[F("max_temp")] = _maxTemp;
    }

    if (_modeCommandTemplate) {
        entity[F("mode_command_template")] = _modeCommandTemplate;
    }

    if (_modeCommandTopic) {
        entity[F("mode_command_topic")] = _modeCommandTopic;
    }

    if (_modeStateTemplate) {
        entity[F("mode_state_template")] = _modeStateTemplate;
    }

    if (_modeStateTopic) {
        entity[F("mode_state_topic")] = _modeStateTopic;
    }

    if (_temperatureCommandTemplate) {
        entity[F("temperature_command_template")] = _temperatureCommandTemplate;
    }

    if (_temperatureCommandTopic) {
        entity[F("temperature_command_topic")] = _temperatureCommandTopic;
    }

    if (_temperatureStateTemplate) {
        entity[F("temperature_state_template")] = _temperatureStateTemplate;
    }

    if (_temperatureStateTopic) {
        entity[F("temperature_state_topic")] = _temperatureStateTopic;
    }

    if (_modesSize > 0) {
        JsonArray modes = entity.createNestedArray(F("modes"));
        for (uint8_t i = 0; i < _modesSize; i++) {
            modes[i] = _modes[i];
        }
    }

    if (_payloadOff) {
        entity[F("payload_off")] = _payloadOff;
    }

    if (_payloadOn) {
        entity[F("payload_on")] = _payloadOn;
    }

    serializeJson(jsonDoc, _payload, JSON_DYNAMIC_MSG_BUFFER);

    return _payload;
}