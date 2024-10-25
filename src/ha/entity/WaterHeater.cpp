#include <Json.h>
#include "defines.h"
#include "WaterHeater.h"

std::string WaterHeater::marshalJSON()
{
    return EDUtils::buildJson([this](JsonObject entity) {
        buildBaseField(entity);

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

        if (_temperatureCommandTemplate) {
            entity[F("temperature_command_template")] = _temperatureCommandTemplate;
        }

        if (_temperatureCommandTopic) {
            entity[F("temperature_command_topic")] = _temperatureCommandTopic;
        }

        if (_modesSize > 0) {
            JsonArray modes = entity.createNestedArray(F("modes"));
            for (uint8_t i = 0; i < _modesSize; i++) {
                modes[i] = _modes[i];
            }
        }
    });
}
