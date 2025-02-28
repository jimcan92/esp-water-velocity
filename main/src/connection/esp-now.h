#ifndef ESP_NOW_H
#define ESP_NOW_H

#include <Arduino.h>
// #include <WiFi.h>
#include <esp_now.h>

#include "../display/debug.h"
#include "utils/structs.h"
#include "calculation/velocity.h"

extern Debug debugger;
extern ESPNowData myData;
extern bool waterArrived;
extern bool flowing;
extern unsigned long startedFlowing;
extern unsigned long timeTaken;
extern Velocity velocityCalculator;
extern float warningThreshold;

class ESPNow
{
public:
    void init();
    bool remoteIsActive;
    void loop();
};

#endif