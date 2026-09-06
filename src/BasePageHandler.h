#pragma once
#include <Arduino.h>
#include "Networking.h"

class BasePageHandler {
protected:
    AsyncWebServer* _server = nullptr;

    // void sendJsonError(int code, const char* message) {
    //     if (_server) {
    //         String payload = "{\"error\":\"" + String(message) + "\"}";
    //         _server->send(code, "application/json", payload);
    //     }
    // }

    // void sendJsonResponse(int code, const String& jsonPayload) {
    //     if (_server) {
    //         _server->send(code, "application/json", jsonPayload);
    //     }
    // }

public:
    virtual ~BasePageHandler() = default;
    virtual void registerRoutes(AsyncWebServer& server) = 0;
};