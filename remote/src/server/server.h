#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <esp_now.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <Preferences.h>

#include "../shared.h"
#include "../water-level/water-level.h"

#define AP_SSID "ESP32_Remote"
#define AP_PASSWORD "00000001"

extern float pipeDepthMM;
extern Preferences preferences;
extern ESPNowData myData;
extern WaterLevel waterLevel;

class RemoteServer
{
public:
    void init();
    void loop();

private:
    static void handleRoot();
    static void handleCalibrate();
    static void handleStatus();
    static WebServer _server;
};

#endif