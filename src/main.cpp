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

ConfigMgr configMgr;
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

void checkHeap() {
    if (!heap_caps_check_integrity_all(true)) {
        Serial.println("Heap integrity check failed before checking largest free block!");
    } else {
        Serial.println("Heap is correct");
    }

    size_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    Serial.print("Largest free block: ");
    Serial.println(largestBlock);
}

void setup() {
    randomSeed(micros());

    Serial.begin(SERIAL_SPEED);
    SPIFFS.begin(true);

    configMgr.load();

    openTherm.begin(handleInterrupt);

    wifiMgr.init();
    mqtt.init();
    boiler.init();
    handler.init();
    stateProducer.init(configMgr.getConfig()->mqttStateTopic);
    commandConsumer.init(configMgr.getConfig()->mqttCommandTopic);
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
