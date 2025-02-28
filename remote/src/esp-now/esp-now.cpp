#include "esp-now.h"

const uint8_t espMainMac[] = {0x34, 0x5F, 0x45, 0xA8, 0x81, 0xA0};
esp_now_peer_info_t peerInfo;
bool sendSuccess = false;

void ESPNow::init()
{
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    esp_now_register_send_cb(ESPNow::sendCallback);
    memcpy(peerInfo.peer_addr, espMainMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
}

void ESPNow::send(const uint8_t *data)
{
    esp_now_send(espMainMac, data, sizeof(data));
    espMainIsActive = sendSuccess;
}

void ESPNow::sendCallback(const uint8_t *mac, esp_now_send_status_t status)
{
    bool success = status == ESP_NOW_SEND_SUCCESS;
    sendSuccess = success;

    if (success)
    {
        Serial.println("Send Success");
    }
    else
    {
        Serial.println("Send Fail");
    }
}