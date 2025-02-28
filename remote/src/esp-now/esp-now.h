#ifndef ESP_NOW_H
#define ESP_NOW_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

class ESPNow
{
public:
    void init();
    void send(const uint8_t *data);
    bool espMainIsActive;

private:
    static void sendCallback(const uint8_t *mac, esp_now_send_status_t status);
};

#endif