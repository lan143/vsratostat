#include <Arduino.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <OpenTherm.h>
#include <ConfigMgr.h>
#include <esp_log.h>
#include <discovery.h>
#include <mqtt.h>
#include <healthcheck.h>

#include "defines.h"
#include "config.h"
#include "thermostat/boiler.h"
#include "thermostat/mqtt/consumer/command_consumer.h"
#include "thermostat/mqtt/producer/state_producer.h"
#include "web/handler.h"
#include "wifi/wifi_mgr.h"

EDConfig::ConfigMgr<Config> configMgr(EEPROM_SIZE);
EDHealthCheck::HealthCheck healthCheck;
EDMQTT::MQTT mqtt(configMgr.getConfig().mqtt);
EDHA::DiscoveryMgr discoveryMgr;
OpenTherm openTherm(OPENTERM_IN_PIN, OPENTERM_OUT_PIN);
WiFiMgr wifiMgr(configMgr.getConfig());
StateProducer stateProducer(&mqtt);
Boiler boiler(&configMgr, &openTherm, &stateProducer);
CommandConsumer commandConsumer(&boiler);
Handler handler(&configMgr, &boiler, &wifiMgr, &healthCheck);

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
    wifiMgr.OnConnect([](bool isConnected) {
        if (isConnected) {
            mqtt.connect();
        }
    });
    healthCheck.registerService(&mqtt);

    discoveryMgr.init(
        configMgr.getConfig().mqttHADiscoveryPrefix,
        configMgr.getConfig().mqttIsHADiscovery,
        [](std::string topicName, std::string payload) {
            return mqtt.publish(topicName.c_str(), payload.c_str(), true);
        }
    );

    EDHA::Device* device = discoveryMgr.addDevice();
    device->setHWVersion(deviceHWVersion)
        ->setSWVersion(deviceFWVersion)
        ->setModel(deviceModel)
        ->setName(deviceName)
        ->setManufacturer(deviceManufacturer);

    boiler.init(&discoveryMgr, device);
    healthCheck.registerService(&boiler);

    handler.init();
    stateProducer.init(configMgr.getConfig().mqttStateTopic);

    commandConsumer.init(configMgr.getConfig().mqttCommandTopic);
    mqtt.subscribe(&commandConsumer);

    ArduinoOTA.setPassword("somestrongpassword");
    ArduinoOTA.begin();

    ESP_LOGI("setup", "complete");
}

void loop()
{
    boiler.loop();
    discoveryMgr.loop();
    ArduinoOTA.handle();
    healthCheck.loop();
}
