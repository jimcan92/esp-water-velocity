#include "velocity.h"

float Velocity::compute(float waterLevel)
{

    if (waterLevel <= 0 || PIPE_DIAMETER <= 0)
    {
        return 0.0f; // No flow
    }
    if (waterLevel > PIPE_DIAMETER)
    {
        waterLevel = PIPE_DIAMETER; // Cap water level
    }

    float ratio = waterLevel / PIPE_DIAMETER;
    if (ratio > 1.0)
        ratio = 1.0; // Avoid invalid values
    if (ratio < 0.0)
        ratio = 0.0; // Avoid negative values
    float theta = 2.0 * acos(1.0 - 2.0 * ratio);

    float area = (pow(PIPE_DIAMETER, 2.0) / 4.0) * ((theta - sin(theta)) / 2.0);
    if (area < 0.0001)
        area = 0.0001; // Prevent near-zero area

    float perimeter = PIPE_DIAMETER * (theta / 2.0);

    if (perimeter <= 0)
        return 0.0f; // Prevent division by zero
    float r = area / perimeter;
    if (r < 0)
        return 0.0f; // Prevent negative power operation

    float a = 1 / ROUGH_COEF;
    float b = pow(r, 2.0 / 3.0);
    float c = pow(slope, 0.5);

    return a * b * c; // Velocity in m/s
}