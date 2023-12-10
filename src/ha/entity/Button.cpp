#include "Button.h"

const char* Button::marshalJSON()
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    JsonObject entity = jsonDoc.to<JsonObject>();
    buildBaseField(&entity);

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

    serializeJson(jsonDoc, _payload, JSON_DYNAMIC_MSG_BUFFER);

    return _payload;
}