#include <Arduino.h>
#include <SPIFFS.h>
#include <OpenTherm.h>
#include <ConfigMgr.h>

#include "defines.h"
#include "config/ConfigEntity.h"
#include "ha/DiscoveryMgr.h"
#include "mqtt/MQTT.h"
#include "thermostat/Boiler.h"
#include "thermostat/mqtt/consumer/CommandConsumer.h"
#include "thermostat/mqtt/producer/StateProducer.h"
#include "web/Handler.h"
#include "wifi/WiFiMgr.h"

Config::ConfigMgr<ConfigEntity> configMgr;
MQTT mqtt(&configMgr);
DiscoveryMgr haDiscoveryMgr(&configMgr, &mqtt);
OpenTherm openTherm(OPENTERM_IN_PIN, OPENTERM_OUT_PIN);
WiFiMgr wifiMgr(&configMgr);
StateProducer stateProducer(&mqtt);
Boiler boiler(&configMgr, &openTherm, &stateProducer);
CommandConsumer commandConsumer(&boiler);
Handler handler(&configMgr, &boiler);

void ICACHE_RAM_ATTR handleInterrupt()
{
    openTherm.handleInterrupt();
}

void setup() {
    randomSeed(micros());

    Serial.begin(SERIAL_SPEED);
    SPIFFS.begin(true);

    configMgr.setDefault([](ConfigEntity& config) {
        snprintf(config.wifiAPSSID, WIFI_SSID_LEN, "Vsratostat_%s", getMacAddress());
        snprintf(config.mqttStateTopic, MQTT_TOPIC_LEN, "vsratostat/%s/state", getChipID());
        snprintf(config.mqttCommandTopic, MQTT_TOPIC_LEN, "vsratostat/%s/set", getChipID());
        snprintf(config.mqttHADiscoveryPrefix, MQTT_TOPIC_LEN, "homeassistant");
    });
    configMgr.load();

    openTherm.begin(handleInterrupt);

    wifiMgr.init();
    mqtt.init();
    boiler.init();
    handler.init();
    stateProducer.init(configMgr.getConfig().mqttStateTopic);
    commandConsumer.init(configMgr.getConfig().mqttCommandTopic);
    mqtt.subscribe(&commandConsumer);
    haDiscoveryMgr.init();
}

void loop()
{
    mqtt.loop();
    boiler.loop();
    haDiscoveryMgr.loop();
    delay(10);
}
