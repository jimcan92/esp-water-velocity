#ifndef VELOCITY_H
#define VELOCITY_H

#include <Arduino.h>

#define ROUGH_COEF 0.07    // Manning roughness coefficient (metric units)
#define PIPE_DIAMETER 76.6 // Pipe diameter in meters

extern float slope;

class Velocity
{
public:
    float compute(float waterLevel);
};

#endif