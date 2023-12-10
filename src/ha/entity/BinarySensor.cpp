#include "BinarySensor.h"

const char* BinarySensor::marshalJSON()
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    JsonObject entity = jsonDoc.to<JsonObject>();
    buildBaseField(&entity);

    if (_deviceClass) {
        entity[F("device_class")] = _deviceClass;
    }

    if (_stateTopic) {
        entity[F("state_topic")] = _stateTopic;
    }

    if (_valueTemplate) {
        entity[F("value_template")] = _valueTemplate;
    }

    if (_payloadOn) {
        entity[F("payload_on")] = _payloadOn;
    }

    if (_payloadOff) {
        entity[F("payload_off")] = _payloadOff;
    }

    serializeJson(jsonDoc, _payload, JSON_DYNAMIC_MSG_BUFFER);

    return _payload;
}
