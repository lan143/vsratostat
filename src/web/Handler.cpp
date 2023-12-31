#include <ArduinoJSON.h>
#include <FS.h>
#include <SPIFFS.h>
#include "../defines.h"
#include "Handler.h"
#include "utils/String.h"

void Handler::init()
{
    _server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _server->serveStatic("/jquery-3.6.0.min.js", SPIFFS, "/jquery-3.6.0.min.js");
    _server->serveStatic("/bootstrap.min.js", SPIFFS, "/bootstrap.min.js");

    _server->on("/api/wifi/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
        /*AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument json(200);

        auto networks = this->_wifiService->getWifiNetworks();

        for (auto i = networks.begin(); i != networks.end(); i++) {
            json.add(*i);
        }

        serializeJson(json, *response);

        request->send(response);*/
    });

    _server->on("/api/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument json(JSON_DYNAMIC_MSG_BUFFER);
        Config* config = _configMgr->getConfig();
        json["wifiSSID"] = config->wifiSSID;
        json["wifiPassword"] = config->wifiPassword;
        json["mqttHost"] = config->mqttHost;
        json["mqttPort"] = config->mqttPort;
        json["mqttLogin"] = config->mqttLogin;
        json["mqttPassword"] = config->mqttPassword;
        json["mqttIsHADiscovery"] = config->mqttIsHADiscovery;
        json["mqttHADiscoveryPrefix"] = config->mqttHADiscoveryPrefix;
        json["mqttCommandTopic"] = config->mqttCommandTopic;
        json["mqttStateTopic"] = config->mqttStateTopic;

        serializeJson(json, *response);

        request->send(response);
    });

    _server->on("/api/boiler/stats", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/html");
        BoilerState &state = _boiler->getState();

        response->printf(
            "Central heating current temperature: %f<br>Hot water current temperature: %f<br>Hot water active: %s<br>Modulation: %f<br>Is flame on: %s<br>Is fault: %s",
            state.getCentralHeatingCurrentTemperature(),
            state.getHotWaterCurrentTemperature(),
            state.isHotWaterActive() ? "yes" : "no",
            state.getModulation(),
            state.isFlameOn() ? "yes" : "no",
            state.isFault() ? "yes" : "no"
        );

        request->send(response);
    });

    _server->on("/api/settings/wifi", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (!request->hasParam("wifiSSID", true) || !request->hasParam("wifiPassword", true)) {
            request->send(422, "application/json", "{\"message\": \"not present wifiSSID or wifiPassword in request\"}");
            return;
        }

        AsyncWebParameter* wifiSSID = request->getParam("wifiSSID", true);
        AsyncWebParameter* wifiPassword = request->getParam("wifiPassword", true);

        if (wifiSSID->value().length() > WIFI_SSID_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"WiFi SSID lenght more 32 symbols\"}");
            return;
        }

        if (wifiPassword->value().length() > WIFI_PWD_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"WiFi password lenght more 63 symbols\"}");
            return;
        }

        Config* config = _configMgr->getConfig();
        strcpy(config->wifiSSID, wifiSSID->value().c_str());
        strcpy(config->wifiPassword, wifiPassword->value().c_str());
        config->isAPMode = false;

        _configMgr->store();

        request->send(200, "application/json", "{}");
    });

    _server->on("/api/settings/mqtt", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (
            !request->hasParam("host", true)
            || !request->hasParam("port", true)
            || !request->hasParam("login", true)
            || !request->hasParam("password", true)
            || !request->hasParam("haDiscoveryPrefix", true)
        ) {
            request->send(422, "application/json", "{\"message\": \"not present mqtt params in request\"}");
            return;
        }

        Config* config = _configMgr->getConfig();
        AsyncWebParameter* host = request->getParam("host", true);
        AsyncWebParameter* port = request->getParam("port", true);
        AsyncWebParameter* login = request->getParam("login", true);
        AsyncWebParameter* password = request->getParam("password", true);
        AsyncWebParameter* haDiscoveryPrefix = request->getParam("haDiscoveryPrefix", true);

        if (host->value().length() == 0 || host->value().length() > HOST_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"MQTT host lenght invalid\"}");
            return;
        }

        int mqttPort;
        if (str2int(&mqttPort, port->value().c_str(), 10) != STR2INT_SUCCESS) {
            request->send(422, "application/json", "{\"message\": \"Incorrect port\"}");
            return;
        }

        if (login->value().length() > MQTT_LOGIN_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"MQTT login length invalid\"}");
            return;
        }

        if (password->value().length() > MQTT_PASSWORD_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"MQTT password length invalid\"}");
            return;
        }

        if (haDiscoveryPrefix->value().length() == 0) {
            request->send(422, "application/json", "{\"message\": \"You must specify mqtt topic name\"}");
            return;
        }

        if (haDiscoveryPrefix->value().length() > MQTT_TOPIC_LEN-1) {
            request->send(422, "application/json", "{\"message\": \"HA discovery prefix length invalid\"}");
            return;
        }

        strcpy(config->mqttHost, host->value().c_str());
        config->mqttPort = (uint16_t)mqttPort;
        strcpy(config->mqttLogin, login->value().c_str());
        strcpy(config->mqttPassword, password->value().c_str());
        strcpy(config->mqttHADiscoveryPrefix, haDiscoveryPrefix->value().c_str());

        _configMgr->store();

        request->send(200, "application/json", "{}");
    });

    _server->on("/api/settings", HTTP_POST, [this](AsyncWebServerRequest *request) {
    });

    _server->on("/api/wifi/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        /*DynamicJsonDocument json(200);
        json["status"] = _wifiService->getWifiStatus();

        serializeJson(json, *response);*/
        response->print("{\"status\": \"unimplemented\"}");
        request->send(response);
    });

    _server->on("/api/reboot", HTTP_POST, [this](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{}");
        delay(200);
        ESP.restart();
    });

    _server->begin();
}