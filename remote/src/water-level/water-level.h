#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

#include <Arduino.h>
#include <esp_now.h>

#include "../shared.h"
#include "../esp-now/esp-now.h"

#define TRIG_PIN 5
#define ECHO_PIN 18

extern float pipeDepthMM;
extern ESPNowData myData;

extern ESPNow espNow;

class WaterLevel
{
public:
    void init();
    float getDistanceMM();
};

#endif