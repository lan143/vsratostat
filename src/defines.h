#pragma once

#define SERIAL_SPEED 115200

#define OPENTERM_IN_PIN 33
#define OPENTERM_OUT_PIN 32
#define RESET_RELAY_PIN 26

const char name[] = "Vsratostat";
const char model[] = "esp32 prototype";
const char manufacturer[] = "lan143";
const char hwVersion[] = "0.1.0";
const char fwVersion[] = "0.1.0";

#define CENTRAL_HEATING_MIN_TEMP 30
#define CENTRAL_HEATING_MAX_TEMP 80

#define WATER_HEATING_MIN_TEMP 35
#define WATER_HEATING_MAX_TEMP 60

#define JSON_DYNAMIC_MSG_BUFFER 4096
#define JSON_SMALL_STATIC_MSG_BUFFER 512
