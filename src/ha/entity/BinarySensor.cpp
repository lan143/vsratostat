#include "BinarySensor.h"
#include "utils/Json.h"

std::string BinarySensor::marshalJSON()
{
    return buildJson([this](JsonObject entity) {
        buildBaseField(entity);

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
    });
}
