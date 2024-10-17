#include "Sensor.h"
#include "../../defines.h"
#include "utils/Json.h"

std::string Sensor::marshalJSON()
{
    return buildJson([this](JsonObject entity) {
        buildBaseField(entity);

        if (_stateTopic) {
            entity[F("state_topic")] = _stateTopic;
        }

        if (_valueTemplate) {
            entity[F("value_template")] = _valueTemplate;
        }

        if (_unitOfMeasurement) {
            entity[F("unit_of_measurement")] = _unitOfMeasurement;
        }
    });
}