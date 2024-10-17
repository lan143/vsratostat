#include "Json.h"

bool parseJson(const char* data, const json_parse_t &f) {
  const size_t free_heap = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  size_t request_size = std::min(free_heap, (size_t) (strlen(data) * 1.5));

  while (true) {
    DynamicJsonDocument json_document(request_size);
    if (json_document.capacity() == 0) {
      return false;
    }

    DeserializationError err = deserializeJson(json_document, data);
        json_document.shrinkToFit();

        JsonObject root = json_document.as<JsonObject>();

        if (err == DeserializationError::Ok) {
            return f(root);
        } else if (err == DeserializationError::NoMemory) {
        if (request_size * 2 >= free_heap) {
            return false;
        }
      request_size *= 2;
      continue;
    } else {
      return false;
    }
  };
  
  return false;
}

std::string buildJson(const json_build_t &f) {
  const size_t free_heap = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  size_t request_size = std::min(free_heap, (size_t) 512);

  while (true) {
    DynamicJsonDocument json_document(request_size);
    if (json_document.capacity() == 0) {
      return "{}";
    }

    JsonObject root = json_document.to<JsonObject>();
    f(root);

    if (json_document.overflowed()) {
      if (request_size == free_heap) {
        return "{}";
      }
      
      request_size = std::min(request_size * 2, free_heap);
      continue;
    }

    json_document.shrinkToFit();
    std::string output;
    serializeJson(json_document, output);

    return output;
  }
}