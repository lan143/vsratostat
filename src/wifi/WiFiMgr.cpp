#include <WiFi.h>
#include "WiFiMgr.h"

void WiFiMgr::init()
{
    Config* config = _configMgr->getConfig();

    if (!config->isAPMode) {
        WiFi.mode(WIFI_STA);
        WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
            this->onEvent(event, info);
        }, SYSTEM_EVENT_STA_CONNECTED);
        WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
            this->onEvent(event, info);
        }, SYSTEM_EVENT_STA_DISCONNECTED);

        WiFi.begin(config->wifiSSID, config->wifiPassword);
    } else {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(config->wifiAPSSID, config->wifiAPHasPassword ? config->wifiAPPassword : NULL);
    }
}

void WiFiMgr::onEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch (event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            _isConnected = true;
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            _isConnected = false;
            break;
    }
}