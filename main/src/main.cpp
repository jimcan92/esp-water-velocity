#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include "display/lcd.h"
#include "sensors/hc-sr04.h"
#include "display/debug.h"
#include "utils/structs.h"
#include "connection/esp-now.h"
#include "connection/gsm.h"
#include "server/server.h"
#include "calculation/velocity.h"

Preferences preferences;

Debug debugger;
LCD lcd;
HCSR04 hcsr04;
ESPNow espNow;
GSM gsm;
RemoteServer server;
Velocity velocityCalculator;

ESPNowData myData;

float pipeDia = 76.6;
float rough = 0.07;
float slope = 0.0666667; // 1/15
float pipeDepth = 70;    // mm
float waterLevel = 0.0;
float velocity = 0.0;   // m/s
int velocityStatus = 0; // 0-Normal, 1-Warning, 2-Critical
float warningThreshold = 10.0;
float criticalThreshold = 20.0;
String phoneNumbers = "+639950281057,+639924367264";

bool flowing = false;
unsigned long startedFlowing = 0;
unsigned long timeTaken = 0;

void setup()
{
  debugger.init();
  lcd.init();

  WiFi.mode(WIFI_AP_STA);

  // 3️⃣ Load Preferences (Config Data)
  preferences.begin("settings", false);
  pipeDepth = preferences.getFloat("pipeDepth", 90.0);
  phoneNumbers = preferences.getString("numbers", "+639950281057,+639924367264");
  warningThreshold = preferences.getFloat("warning", 10.0);
  criticalThreshold = preferences.getFloat("critical", 20.0);
  slope = preferences.getFloat("slope", 0.0666667);
  rough = preferences.getFloat("rough", 0.07);
  pipeDia = preferences.getFloat("pipe_dia", 76.6);

  hcsr04.init();
  espNow.init();
  gsm.init();
  server.init();
}

void loop()
{
  server.loop();
  espNow.loop();
}