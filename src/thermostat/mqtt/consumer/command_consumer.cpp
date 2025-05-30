#include <Arduino.h>
#include <esp_log.h>
#include "command_consumer.h"
#include "thermostat/mqtt/entity/command_entity.h"

void CommandConsumer::consume(std::string payload)
{
    ESP_LOGD("command_consumer", "handle");

    CommandEntity command;
    if (!command.unmarshalJSON(payload.c_str())) {
        ESP_LOGE("command_consumer", "cant unmarshal command");
        
        return;
    }

    if (command.hasCentralHeatingMode()) {
        _boiler->setCentralHeatingOn(strcmp(command.getCentralHeatingMode(), "off") != 0);
    }

    if (command.hasCentralHeatingSetPoint()) {
        _boiler->setCentralHeatingSetPoint(command.getCentralHeatingSetPoint());
    }

    if (command.hasReset()) {
        _boiler->resetErrors();
    }

    if (command.hasHotWaterMode()) {
        _boiler->setHotWaterOn(strcmp(command.getHotWaterMode(), "off") != 0);
    }

    if (command.hasHotWaterSetPoint()) {
        _boiler->setHotWaterSetPoint(command.getHotWaterSetPoint());
    }
}