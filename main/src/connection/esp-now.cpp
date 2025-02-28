#include "esp-now.h"

static long lastReceivedTime = 0; // Track last received time

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len);

void ESPNow::init()
{
    // WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        debugger.println("ESP-NOW Init Failed");
    }

    if (esp_now_register_recv_cb(onReceive) != ESP_OK)
    {
        debugger.println("ESP-NOW Register Callback Failed");
    }
}

void ESPNow::loop()
{
    // static long lastReceivedTime = 0; // Track last received time

    remoteIsActive = (millis() - lastReceivedTime) < 5000;
}

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    // receiving = true;
    lastReceivedTime = millis();

    memcpy(&myData, incomingData, sizeof(myData));

    if (flowing && velocityCalculator.compute(myData.water_level) >= warningThreshold)
    {
        timeTaken = millis() - startedFlowing;
        flowing = false;
    }
}