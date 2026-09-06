// ControlPageHandler.cpp
#include "ControlPageHandler.h"

ControlPageHandler::ControlPageHandler(WebSocketPageHandler* webSocketHandler) 
    :   _webSocketHandler(webSocketHandler)
{
}

void ControlPageHandler::registerRoutes(AsyncWebServer& server) {
    _server = &server;

    ///_server->on("/api/path", HTTP_GET, [this](AsyncWebServerRequest *request) { handle<name>(request); });
}
