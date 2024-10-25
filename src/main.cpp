#include <Arduino.h>
#include <SPIFFS.h>
#include <OpenTherm.h>
#include <ConfigMgr.h>
#include <esp_log.h>

#include "defines.h"
#include "config/Config.h"
#include "ha/DiscoveryMgr.h"
#include "mqtt/MQTT.h"
#include "thermostat/Boiler.h"
#include "thermostat/mqtt/consumer/CommandConsumer.h"
#include "thermostat/mqtt/producer/StateProducer.h"
#include "web/Handler.h"
#include "wifi/WiFiMgr.h"

EDConfig::ConfigMgr<Config> configMgr;
MQTT mqtt(configMgr.getConfig());
DiscoveryMgr haDiscoveryMgr(configMgr.getConfig(), &mqtt);
OpenTherm openTherm(OPENTERM_IN_PIN, OPENTERM_OUT_PIN);
WiFiMgr wifiMgr(configMgr.getConfig());
StateProducer stateProducer(&mqtt);
Boiler boiler(&configMgr, &openTherm, &stateProducer);
CommandConsumer commandConsumer(&boiler);
Handler handler(&configMgr, &boiler, &wifiMgr);

void ICACHE_RAM_ATTR handleInterrupt()
{
    openTherm.handleInterrupt();
}

void setup() {
    randomSeed(micros());

    Serial.begin(SERIAL_SPEED);

    ESP_LOGI("setup", "Vsratostat");
    ESP_LOGI("setup", "start");

    SPIFFS.begin(true);

    configMgr.setDefault([](Config& config) {
        snprintf(config.wifiAPSSID, WIFI_SSID_LEN, "Vsratostat_%s", EDUtils::getMacAddress().c_str());
        snprintf(config.mqttStateTopic, MQTT_TOPIC_LEN, "vsratostat/%s/state", EDUtils::getChipID());
        snprintf(config.mqttCommandTopic, MQTT_TOPIC_LEN, "vsratostat/%s/set", EDUtils::getChipID());
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

    ESP_LOGI("setup", "complete");
}

void loop()
{
    mqtt.loop();
    boiler.loop();
    haDiscoveryMgr.loop();
    delay(10);
}
