// ControlPageHandler.h
#pragma once

#include "../BasePageHandler.h"
#include "WebSocketPageHandler.h"

class ControlPageHandler : public BasePageHandler {
private:
    WebSocketPageHandler* _webSocketHandler;

    /// void handle<name>(AsyncWebServerRequest *request);
 

public:
    explicit ControlPageHandler(WebSocketPageHandler* webSocketHandler);
    void registerRoutes(AsyncWebServer& server) override;
};