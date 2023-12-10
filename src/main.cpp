#include <Arduino.h>
#include <SPIFFS.h>
#include <OpenTherm.h>

#include "defines.h"
#include "config/ConfigMgr.h"
#include "ha/DiscoveryMgr.h"
#include "mqtt/MQTT.h"
#include "thermostat/Boiler.h"
#include "thermostat/mqtt/consumer/CommandConsumer.h"
#include "thermostat/mqtt/producer/StateProducer.h"
#include "web/Handler.h"
#include "wifi/WiFiMgr.h"

ConfigMgr* configMgr;
MQTT* mqtt;
DiscoveryMgr* haDiscoveryMgr;
OpenTherm openTherm(OPENTERM_IN_PIN, OPENTERM_OUT_PIN);
WiFiMgr* wifiMgr;
StateProducer* stateProducer;
Boiler* boiler;
CommandConsumer* commandConsumer;
Handler* handler;

void ICACHE_RAM_ATTR handleInterrupt()
{
	openTherm.handleInterrupt();
}

void setup()
{
    configMgr = new ConfigMgr();
    mqtt = new MQTT(configMgr);
    haDiscoveryMgr = new DiscoveryMgr(configMgr, mqtt);
    wifiMgr = new WiFiMgr(configMgr);
    stateProducer = new StateProducer(mqtt);
    boiler = new Boiler(configMgr, &openTherm, stateProducer);
    commandConsumer = new CommandConsumer(boiler);
    handler = new Handler(configMgr, boiler);

    Serial.begin(SERIAL_SPEED);
    SPIFFS.begin(true);

    configMgr->load();

    openTherm.begin(handleInterrupt);

    wifiMgr->init();

    mqtt->init();

    boiler->init();

    handler->init();

    stateProducer->init(configMgr->getConfig()->mqttStateTopic);

    commandConsumer->init(configMgr->getConfig()->mqttCommandTopic);
    mqtt->subscribe(commandConsumer);

    haDiscoveryMgr->init();
}

void loop()
{
    mqtt->loop();
    boiler->loop();
    haDiscoveryMgr->loop();
}
