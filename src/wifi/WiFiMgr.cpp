#include <WiFi.h>
#include "WiFiMgr.h"

void WiFiMgr::init()
{
    Config* config = _configMgr->getConfig();

    if (!config->isAPMode) {
        WiFi.mode(WIFI_STA);
        WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) {
            if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
                _isConnected = true;
                Serial.println("wifi: connected");
            } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
                _isConnected = false;
                Serial.println("wifi: disconnected");
            }
        });
        
        WiFi.begin(config->wifiSSID, config->wifiPassword);
    } else {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(config->wifiAPSSID, config->wifiAPHasPassword ? config->wifiAPPassword : NULL);
    }
}
