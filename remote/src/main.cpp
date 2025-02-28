#include <Arduino.h>
#include <Preferences.h>

#include "server/server.h"
#include "water-level/water-level.h"
#include "esp-now/esp-now.h"

const char *apSSID = "ESP32_Remote";
const char *apPassword = "00000001";

RemoteServer server;
WaterLevel waterLevel;
ESPNow espNow;

Preferences preferences;
TaskHandle_t sensorTaskHandle;

float pipeDepthMM = 90.0;

ESPNowData myData;

// Task running on Core 1 (for sensors and ESP-NOW)
void sensorTask(void *parameter)
{
  while (true)
  {
    float distance = waterLevel.getDistanceMM();
    if (distance > 0 && distance < pipeDepthMM)
    {
      myData.water_level = pipeDepthMM - distance;
    }
    else
    {
      myData.water_level = 0;
    }

    Serial.print("Water Level: ");
    Serial.println(myData.water_level);
    espNow.send((uint8_t *)&myData);

    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 5 seconds
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);

  preferences.begin("settings", false);
  pipeDepthMM = preferences.getFloat("tankDepth", 90.0);

  server.init();
  waterLevel.init();
  espNow.init();

  xTaskCreatePinnedToCore(
      sensorTask,        // Task function
      "SensorTask",      // Name
      4096,              // Stack size
      NULL,              // Parameters
      1,                 // Priority
      &sensorTaskHandle, // Task handle
      1                  // Run on Core 1
  );
}

void loop()
{
  server.loop();
}
