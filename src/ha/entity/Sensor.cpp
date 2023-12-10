#include "Sensor.h"
#include "../../defines.h"

const char* Sensor::marshalJSON()
{
    DynamicJsonDocument jsonDoc(JSON_DYNAMIC_MSG_BUFFER);
    JsonObject entity = jsonDoc.to<JsonObject>();
    buildBaseField(&entity);

    if (_stateTopic) {
        entity[F("state_topic")] = _stateTopic;
    }

    if (_valueTemplate) {
        entity[F("value_template")] = _valueTemplate;
    }

    if (_unitOfMeasurement) {
        entity[F("unit_of_measurement")] = _unitOfMeasurement;
    }

    serializeJson(jsonDoc, _payload, JSON_DYNAMIC_MSG_BUFFER);

    return _payload;
}