#include "server.h"

WebServer RemoteServer::server(80);

void RemoteServer::init()
{
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    debugger.println("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    server.on("/", HTTP_GET, handleRoot);
    server.on("/calibrate", HTTP_GET, handleCalibrate);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/set_numbers", HTTP_POST, handleSetNumbers);
    server.on("/set_slope", HTTP_POST, handleSetSlope);
    server.on("/set_warning_threshold", HTTP_POST, handleSetWarningThreshold);
    server.on("/set_critical_threshold", HTTP_POST, handleSetCriticalThreshold);
    server.on("/initial_values", HTTP_GET, handleSetInitialValues);

    server.serveStatic("/", SPIFFS, "/");

    server.begin();
}

void RemoteServer::handleRoot()
{
    File file = SPIFFS.open("/index.html", "r");
    if (!file)
    {
        debugger.println("Failed to open");
        RemoteServer::server.send(500, "text/plain", "Failed to open file");
        return;
    }

    String html = file.readString();
    file.close();

    RemoteServer::server.send(200, "text/html", html);
}

void RemoteServer::handleCalibrate()
{
    float emptyPipeHeight = hcsr04.getDistance();
    if (emptyPipeHeight > 0)
    {
        pipeDepth = emptyPipeHeight;
        preferences.putFloat("pipeDepth", pipeDepth);
    }
    RemoteServer::server.sendHeader("Location", "/", true);
    RemoteServer::server.send(302, "text/plain", "Calibrating...");
}

void RemoteServer::handleStatus()
{
    String json = "{";
    json += "\"pipe_depth\": " + String(pipeDepth) + ",";
    json += "\"water_level\": " + String(hcsr04.getWaterLevel()) + ",";
    json += "\"velocity\": " + String(velocity) + ",";
    json += "\"remote_water_level\": " + String(myData.water_level) + ",";
    json += "\"time\": " + String(timeTaken / 1000) + ",";
    json += "\"esp_remote_active\": \"" + String(espNow.remoteIsActive ? "Active" : "Inactive") + "\",";
    json += "\"flowing\": \"" + String(flowing ? "True" : "False") + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

void RemoteServer::handleSetInitialValues()
{
    String json = "{";
    json += "\"numbers\": \"" + String(phoneNumbers) + "\",";
    json += "\"warning_threshold\": \"" + String(warningThreshold) + "\",";
    json += "\"critical_threshold\": \"" + String(criticalThreshold) + "\",";
    json += "\"slope\": \"" + String(slope, 7) + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

void RemoteServer::handleSetNumbers()
{
    if (server.hasArg("numbers"))
    {
        phoneNumbers = server.arg("numbers");
        preferences.putString("numbers", phoneNumbers);
        server.send(200, "text/plain", "Numbers updated successfully");
    }
    else
    {
        server.send(400, "text/plain", "Missing 'numbers' parameter");
    }
}

void RemoteServer::handleSetSlope()
{
    if (server.hasArg("slope"))
    {
        slope = server.arg("slope").toFloat();
        preferences.putFloat("slope", slope);
        server.send(200, "text/plain", "Slope updated successfully");
    }
    else
    {
        server.send(400, "text/plain", "Missing 'numbers' parameter");
    }
}

void RemoteServer::handleSetWarningThreshold()
{
    if (server.hasArg("warning_threshold"))
    {
        warningThreshold = server.arg("warning_threshold").toFloat();
        preferences.putFloat("warning", warningThreshold);
        server.send(200, "text/plain", "Warning threshold updated successfully");
    }
    else
    {
        server.send(400, "text/plain", "Missing 'warning_threshold' parameter");
    }
}

void RemoteServer::handleSetCriticalThreshold()
{
    if (server.hasArg("critical_threshold"))
    {
        criticalThreshold = server.arg("critical_threshold").toFloat();
        preferences.putFloat("critical", criticalThreshold);
        server.send(200, "text/plain", "Critical threshold updated successfully");
    }
    else
    {
        server.send(400, "text/plain", "Missing 'critical_threshold' parameter");
    }
}

void RemoteServer::loop()
{
    server.handleClient();
}