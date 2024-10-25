#include "defines.h"
#include "Base.h"

void Base::buildBaseField(JsonObject entity)
{
    if (_name) {
        (entity)[F("name")] = _name;
    }

    JsonObject device = entity.createNestedObject("device");
    _device->buildBaseField(device);

    (entity)[F("entity_category")] = _entityCategory;
    (entity)[F("object_id")] = _objectID;
    (entity)[F("unique_id")] = _uniqueID;
    (entity)[F("force_update")] = _forceUpdate;
}
