#include "velocity.h"

float Velocity::compute(float waterLevel)
{
    // Convert mm to meters
    float wl = waterLevel / 1000.0f;
    float pipeDiameter = pipeDia / 1000.0f;

    if (wl <= 0 || pipeDiameter <= 0)
    {
        return 0.0f; // No flow
    }
    if (wl > pipeDiameter)
    {
        wl = pipeDiameter; // Cap water level
    }

    // Ratio of water level to pipe diameter
    float ratio = wl / pipeDiameter;
    ratio = fmax(0.0f, fmin(1.0f, ratio)); // Clamp between 0 and 1

    // Calculate theta (radians)
    float theta = 2.0 * acos(1.0 - 2.0 * ratio);

    // Cross-sectional flow area (m²)
    float area = (pow(pipeDiameter, 2.0) / 4.0) * ((theta - sin(theta)) / 2.0);
    area = fmax(area, 0.0001f); // Prevent near-zero area

    // Wetted perimeter (m)
    float perimeter = pipeDiameter * (theta / 2.0);
    if (perimeter <= 0)
        return 0.0f; // Prevent division by zero

    // Hydraulic radius (m)
    float r = area / perimeter;
    if (r <= 0)
        return 0.0f; // Prevent negative values

    // Manning’s equation for velocity (m/s)
    float velocity = (1.0f / rough) * pow(r, 2.0f / 3.0f) * pow(slope, 0.5f);

    return velocity; // Velocity in meters per second (m/s)
}

// float Velocity::compute(float waterLevel)
// {
//     float wl = waterLevel / 1000;
//     float pipeDiameter = pipeDia / 1000;

//     if (wl <= 0 || pipeDiameter <= 0)
//     {
//         return 0.0f; // No flow
//     }
//     if (wl > pipeDiameter)
//     {
//         wl = pipeDiameter; // Cap water level
//     }

//     float ratio = wl / pipeDiameter;
//     if (ratio > 1.0)
//         ratio = 1.0; // Avoid invalid values
//     if (ratio < 0.0)
//         ratio = 0.0; // Avoid negative values
//     float theta = 2.0 * acos(1.0 - 2.0 * ratio);

//     float area = (pow(pipeDiameter, 2.0) / 4.0) * ((theta - sin(theta)) / 2.0);
//     if (area < 0.0001)
//         area = 0.0001; // Prevent near-zero area

//     float perimeter = pipeDiameter * (theta / 2.0);

//     if (perimeter <= 0)
//         return 0.0f; // Prevent division by zero
//     float r = area / perimeter;
//     if (r < 0)
//         return 0.0f; // Prevent negative power operation

//     float a = 1 / rough;
//     float b = pow(r, 2.0 / 3.0);
//     float c = pow(slope, 0.5);

//     return a * b * c; // Velocity in m/s
// }

// float Velocity::compute(float waterLevel)
// {
//     if (waterLevel <= 0 || pipeDia <= 0)
//     {
//         return 0.0f; // No flow
//     }
//     if (waterLevel > pipeDia)
//     {
//         waterLevel = pipeDia; // Cap water level
//     }

//     float ratio = waterLevel / pipeDia;
//     if (ratio > 1.0)
//         ratio = 1.0; // Avoid invalid values
//     if (ratio < 0.0)
//         ratio = 0.0; // Avoid negative values

//     // Calculate theta (radians)
//     float theta = 2.0 * acos(1.0 - 2.0 * ratio);

//     // Cross-sectional area of flow (in mm²)
//     float area = (pow(pipeDia, 2.0) / 4.0) * ((theta - sin(theta)) / 2.0);
//     if (area < 0.0001)
//         area = 0.0001; // Prevent near-zero area

//     // Wetted perimeter (in mm)
//     float perimeter = pipeDia * (theta / 2.0);

//     if (perimeter <= 0)
//         return 0.0f; // Prevent division by zero

//     // Hydraulic radius (in mm)
//     float r = area / perimeter;

//     if (r < 0)
//         return 0.0f; // Prevent negative power operation

//     // Compute velocity in mm/s using the updated Manning’s formula
//     float a = 10000 / rough; // Corrected scaling factor for mm
//     float b = pow(r, 2.0 / 3.0);
//     float c = pow(slope, 0.5);

//     float velocity_mm_s = a * b * c; // Velocity in mm/s

//     // Convert to m/s by dividing by 1000
//     return velocity_mm_s / 1000.0f; // Velocity in m/s
// }
