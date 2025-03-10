#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <esp_now.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <Preferences.h>

#include "../utils/structs.h"
#include "../sensors/hc-sr04.h"
#include "../connection/esp-now.h"

#define AP_SSID "ESP32_Main"
#define AP_PASSWORD "00000000"

extern float pipeDepth;
extern Preferences preferences;
extern ESPNowData myData;
extern HCSR04 hcsr04;
extern ESPNow espNow;
extern float velocity;
extern float slope;
extern float rough;
extern float pipeDia;

class RemoteServer
{
public:
    void init();
    void loop();

private:
    static void handleRoot();
    static void handleCalibrate();
    static void handleStatus();
    static void handleSetInitialValues();
    static void handleSetNumbers();
    static void handleSetWarningThreshold();
    static void handleSetCriticalThreshold();
    static void handleSetSlope();
    static void handleSetRough();
    static void handleSetPipeDia();
    static WebServer server;
};

#endif