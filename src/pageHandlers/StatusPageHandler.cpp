// StatusPageHandler.cpp
#include "StatusPageHandler.h"

void StatusPageHandler::registerRoutes(AsyncWebServer& server) {
    _server = &server;

    _server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request) { handleGetStatus(request); });
    _server->on("/api/metrics", HTTP_GET, [this](AsyncWebServerRequest *request) { handleGetMetrics(request); });
}

void StatusPageHandler::handleGetStatus(AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"uptime_ms\":" + String(millis()) + ",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap());
    json += "}";
    
    // Add "application/json" as the second argument!
    request->send(200, "application/json", json);
}

void StatusPageHandler::handleGetMetrics(AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"cpu_freq_mhz\":" + String(ESP.getCpuFreqMHz()) + ",";
    json += "\"sdk_version\":\"" + String(ESP.getSdkVersion()) + "\"";
    json += "}";
    
    // Add "application/json" as the second argument!
    request->send(200, "application/json", json);
}