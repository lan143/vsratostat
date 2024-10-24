#include <Arduino.h>
#include <esp_log.h>
#include "CommandConsumer.h"
#include "../entity/CommandEntity.h"

void CommandConsumer::consume(std::string payload)
{
    ESP_LOGD("command_consumer", "handle");

    CommandEntity command;
    if (!command.unmarshalJSON(payload.c_str())) {
        ESP_LOGE("command_consumer", "cant unmarshal command");
        
        return;
    }

    if (command.hasCentralHeatingMode()) {
        _boiler->setCentralHeatingOn(strcmp(command.getCentralHeatingMode(), modeOff) != 0);
    }

    if (command.hasCentralHeatingSetPoint()) {
        _boiler->setCentralHeatingSetPoint(command.getCentralHeatingSetPoint());
    }

    if (command.hasReset()) {
        _boiler->resetErrors();
    }

    if (command.hasHotWaterMode()) {
        _boiler->setHotWaterOn(strcmp(command.getHotWaterMode(), modeOff) != 0);
    }

    if (command.hasHotWaterSetPoint()) {
        _boiler->setHotWaterSetPoint(command.getHotWaterSetPoint());
    }
}