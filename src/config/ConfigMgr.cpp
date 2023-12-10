#include <EEPROM.h>
#include <WiFi.h>
#include "../defines.h"
#include "../utils/Utils.h"
#include "ConfigMgr.h"

bool ConfigMgr::load()
{
    uint16_t checksum = 0;
    Config config = {};
    EEPROM.begin(sizeof(config) + sizeof(checksum));
    EEPROM.get(0, config);
    EEPROM.get(sizeof(config), checksum);
    EEPROM.end();

    uint16_t configChecksum = calculateChecksum(config);

    if (checksum == configChecksum)
    {
        _config = config;
        return true;
    }

    snprintf(_config.wifiAPSSID, WIFI_SSID_LEN, "Vsratostat_%s", getMacAddress());
    snprintf(_config.mqttStateTopic, MQTT_TOPIC_LEN, "vsratostat_/%s/state", getChipID());
    snprintf(_config.mqttCommandTopic, MQTT_TOPIC_LEN, "vsratostat_/%s/set", getChipID());
    snprintf(_config.mqttHADiscoveryPrefix, MQTT_TOPIC_LEN, "homeassistant");

    return false;
}

bool ConfigMgr::store()
{
    uint16_t checksum = calculateChecksum(_config);
    EEPROM.begin(sizeof(_config) + sizeof(checksum));
    EEPROM.put(0, _config);
    EEPROM.put(sizeof(_config), checksum);

    bool result = EEPROM.commit();
    EEPROM.end();

    return result;
}

uint16_t ConfigMgr::calculateChecksum(Config &config)
{
    uint8_t *buf = (uint8_t *)&config;
	uint16_t crc = 0xffff, poly = 0xa001;
	uint16_t i = 0;
	uint16_t len = sizeof(config) - 2;

	for (i = 0; i < len; i++)
	{
		crc ^= buf[i];
		for (uint8_t j = 0; j < 8; j++)
		{
			if (crc & 0x01)
			{
				crc >>= 1;
				crc ^= poly;
			}
			else
				crc >>= 1;
		}
	}

	return crc;
}