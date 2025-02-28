#include "server.h"

WebServer RemoteServer::_server(80);

void RemoteServer::init()
{

    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    _server.on("/", HTTP_GET, RemoteServer::handleRoot);
    _server.on("/calibrate", HTTP_GET, RemoteServer::handleCalibrate);
    _server.on("/status", HTTP_GET, RemoteServer::handleStatus);

    _server.serveStatic("/", SPIFFS, "/");

    _server.begin();
}

void RemoteServer::handleRoot()
{
    File file = SPIFFS.open("/index.html", "r");
    if (!file)
    {
        RemoteServer::_server.send(500, "text/plain", "Failed to open file");
        return;
    }

    String html = file.readString();
    file.close();

    RemoteServer::_server.send(200, "text/html", html);
}

void RemoteServer::handleCalibrate()
{
    float emptyPipeHeight = waterLevel.getDistanceMM();
    if (emptyPipeHeight > 0)
    {
        pipeDepthMM = emptyPipeHeight;
        preferences.putFloat("tankDepth", pipeDepthMM);
    }
    RemoteServer::_server.sendHeader("Location", "/", true);
    RemoteServer::_server.send(302, "text/plain", "Calibrating...");
}

void RemoteServer::handleStatus()
{
    String json = "{";
    json += "\"pipe_depth\": " + String(pipeDepthMM) + ",";
    json += "\"water_level\": " + String(myData.water_level) + ",";
    json += "\"esp_main_active\": ";
    json += espNow.espMainIsActive;

    json += "}";

    _server.send(200, "application/json", json);
}

void RemoteServer::loop()
{
    _server.handleClient();
}