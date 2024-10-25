#include <Json.h>
#include "Button.h"

std::string Button::marshalJSON()
{
    return EDUtils::buildJson([this](JsonObject entity) {
        buildBaseField(entity);
        if (_deviceClass) {
            entity[F("device_class")] = _deviceClass;
        }

        if (_commandTopic) {
            entity[F("command_topic")] = _commandTopic;
        }

        if (_commandTemplate) {
            entity[F("command_template")] = _commandTemplate;
        }

        if (_payloadPress) {
            entity[F("payload_press")] = _payloadPress;
        }
    });
}
