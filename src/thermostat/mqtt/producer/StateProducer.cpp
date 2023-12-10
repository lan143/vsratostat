#include "StateProducer.h"

void StateProducer::publish(StateEntity* state)
{
    const char* payload = state->marshalJSON();
    Serial.printf("payload: %s\n", payload);
    _mqtt->publish(_topic, payload, true);
}
