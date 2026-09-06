// ApiHost.cpp
#include <Arduino.h>
#include <LittleFS.h>
#include "ApiHost.h"

ApiHost::ApiHost(uint16_t port) : _server(port), _port(port) {}

void ApiHost::setupDefaultRoutes() {
    _server.onNotFound([this](AsyncWebServerRequest *request) {
        String msg = "{\"error\":\"Not Found\",\"path\":\"" + request->url() + "\"}";
        request->send(404, "application/json", msg);
    });
}

void ApiHost::addHandler(BasePageHandler* handler) {
    if (handler != nullptr) {
        _handlers.push_back(handler);
        handler->registerRoutes(_server);
    }
}

void ApiHost::begin() {
    _server.serveStatic("/", LittleFS, "/")
      .setDefaultFile("index.html")
      .setCacheControl("max-age=86400");

    setupDefaultRoutes();
    _server.begin();

    Serial.printf("[ApiHost] Server listening on port %d\n", _port);
}

void ApiHost::handle() {
}