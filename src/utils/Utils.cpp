#include "Utils.h"

char _macStr[19] = {0};
char _macAddress[18] = {0};

const char* getMacAddress()
{
    if (strlen(_macAddress) > 0) {
        return _macAddress;
    }

    uint8_t chipId[6];
    esp_efuse_read_mac(chipId);
    snprintf(
        _macAddress,
        18,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        chipId[0],
        chipId[1],
        chipId[2],
        chipId[3],
        chipId[4],
        chipId[5]
    );

    return _macAddress;
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