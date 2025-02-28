#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

#include "connection/gsm.h"
#include "calculation/velocity.h"
#include "connection/esp-now.h"

#define TRIG_PIN 5
#define ECHO_PIN 18

#define SAMPLE_INTERVAL_MS 100                                 // Read sensor every 100 ms
#define AVERAGE_INTERVAL_MS 1000                               // Compute average every 1000 ms
#define MAX_SAMPLES (AVERAGE_INTERVAL_MS / SAMPLE_INTERVAL_MS) // Number of readings per averaging period

extern float pipeDepth;
extern float waterLevel;
extern float velocity;
extern int velocityStatus;
extern float warningThreshold;
extern float criticalThreshold;
extern GSM gsm;
extern Velocity velocityCalculator;
extern bool flowing;
extern unsigned long startedFlowing;
extern unsigned long timeTaken;
extern ESPNow espNow;

class HCSR04
{
public:
    void init();
    float getDistance();
    float getWaterLevel();
};
#endif