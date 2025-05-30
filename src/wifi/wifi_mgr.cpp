#include <WiFi.h>
#include <esp_log.h>
#include "wifi_mgr.h"

void WiFiMgr::init()
{
    ESP_LOGI("wifi", "init start");

    if (!_config.isAPMode) {
        ESP_LOGI("wifi", "run in client mode");

        WiFi.mode(WIFI_STA);
        WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) {
            if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
                _isConnected = true;
                ESP_LOGI("wifi", "connected");
            } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
                _isConnected = false;
                ESP_LOGI("wifi", "disconnected");
            }

            for (auto fn : _connectCallbacks) {
                fn(_isConnected);
            }
        });
        
        WiFi.begin(_config.wifiSSID, _config.wifiPassword);
    } else {
        ESP_LOGI("wifi", "run in access point mode");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(_config.wifiAPSSID, _config.wifiAPHasPassword ? _config.wifiAPPassword : NULL);
    }

    ESP_LOGI("wifi", "init complete");
}
