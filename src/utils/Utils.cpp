#include "Utils.h"

char _macStr[19] = {0};

std::string getMacAddress()
{
    char macAddress[18] = {0};
    uint8_t chipId[6];
    esp_read_mac(chipId, ESP_MAC_WIFI_STA);
    snprintf(
        macAddress,
        18,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        chipId[0],
        chipId[1],
        chipId[2],
        chipId[3],
        chipId[4],
        chipId[5]
    );

    return std::string(macAddress);
}

const char* getChipID()
{
    if (strlen(_macStr) > 0) {
        return _macStr;
    }

    uint64_t mac = ESP.getEfuseMac();
    snprintf(_macStr, 19, "0x%llx", mac);

    return _macStr;
}