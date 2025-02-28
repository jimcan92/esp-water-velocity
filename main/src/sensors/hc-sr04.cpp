#include "hc-sr04.h"

void hcsr04Task(void *p);

void HCSR04::init()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    xTaskCreatePinnedToCore(
        hcsr04Task,     // Task function
        "HC-SR04 Task", // Name
        4096,           // Stack size
        this,           // Parameters
        1,              // Priority
        NULL,           // Task handle
        1               // Run on Core 1
    );
}

float HCSR04::getDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duration == 0)
        return -1;

    float distance = duration * 0.343 / 2; // Speed of sound in mm/us
    return distance;
}

float HCSR04::getWaterLevel()
{
    float distance = getDistance();

    if (distance <= 0)
        return 0.0;

    float waterLevel = pipeDepth - distance;

    if (waterLevel < 0)
    {
        waterLevel = 0;
    }

    return waterLevel;
}

void hcsr04Task(void *p)
{
    HCSR04 *hcsr04 = (HCSR04 *)p;
    TickType_t lastWakeTime = xTaskGetTickCount();
    float readings[MAX_SAMPLES];
    int index = 0;

    while (true)
    {
        float reading = hcsr04->getWaterLevel();

        if (index < MAX_SAMPLES) // Ensure no buffer overflow
        {
            readings[index] = reading;
            index++;
        }

        if (index >= MAX_SAMPLES)
        {
            // Compute average water level
            float sum = 0;
            for (int i = 0; i < MAX_SAMPLES; i++)
            {
                sum += readings[i];
            }
            waterLevel = sum / MAX_SAMPLES;

            // Compute velocity
            velocity = velocityCalculator.compute(waterLevel);
            int newVelocityStatus = velocityStatus; // Keep the same status by default

            if (velocity >= criticalThreshold)
            {
                newVelocityStatus = 2; // CRITICAL
            }
            else if (velocity >= warningThreshold)
            {
                newVelocityStatus = 1; // WARNING
            }
            else if (velocity <= warningThreshold - 1.0) // Add a small buffer to avoid quick switching
            {
                newVelocityStatus = 0; // NORMAL
            }

            // Send SMS only if the status has changed
            if (velocityStatus != newVelocityStatus)
            {

                switch (newVelocityStatus)
                {
                case 1:
                    debugger.println("🚨 WARNING: Velocity = " + String(velocity) + " m/s");
                    if (espNow.remoteIsActive && velocityStatus < 1 && !flowing)
                    {
                        startedFlowing = millis();
                        flowing = true;
                        timeTaken = 0;
                    }
                    gsm.queueSMSToAll("🚨 WARNING: Velocity is " + String(velocity) + " m/s!");
                    break;

                case 2:
                    debugger.println("🔥 CRITICAL: Velocity = " + String(velocity) + " m/s");
                    gsm.queueSMSToAll("🔥 CRITICAL: Velocity is " + String(velocity) + " m/s!");
                    break;

                case 0:
                    debugger.println("✅ NORMAL: Velocity = " + String(velocity) + " m/s");
                    break;
                }
                velocityStatus = newVelocityStatus; // Update status
            }

            index = 0; // Reset index for new readings
        }

        vTaskDelayUntil(&lastWakeTime, SAMPLE_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}
