#pragma once

#define SERIAL_SPEED 115200

#define OPENTERM_IN_PIN 33
#define OPENTERM_OUT_PIN 32
#define RESET_RELAY_PIN 26

const char deviceName[] = "Vsratostat";
const char deviceModel[] = "esp32 prototype";
const char deviceManufacturer[] = "lan143";
const char deviceHWVersion[] = "0.1.0";
const char deviceFWVersion[] = "0.1.0";

#define CENTRAL_HEATING_MIN_TEMP 30
#define CENTRAL_HEATING_MAX_TEMP 80

#define WATER_HEATING_MIN_TEMP 35
#define WATER_HEATING_MAX_TEMP 60
