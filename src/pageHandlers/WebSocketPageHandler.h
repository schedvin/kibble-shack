#pragma once
#include <Arduino.h>
#include "../BasePageHandler.h"

class WebSocketPageHandler : public BasePageHandler {
private:
    AsyncWebSocket _ws;

    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, 
                   AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            Serial.printf("[WS] Client #%u connected on Port 80\n", client->id());
            client->text("{\"status\":\"connected\"}");
        }
    }

public:
    explicit WebSocketPageHandler(const char* endpoint = "/ws") : _ws(endpoint) {}

    void registerRoutes(AsyncWebServer& server) override {
        _server = &server;
        _ws.onEvent([this](AsyncWebSocket* s, AsyncWebSocketClient* c, AwsEventType t, void* a, uint8_t* d, size_t l) {
            this->onWsEvent(s, c, t, a, d, l);
        });
        _server->addHandler(&_ws);
        Serial.println("[WebSocket] Registered on endpoint /ws (Port 80)");
    }

    void broadcast(const String& message) {
        _ws.textAll(message);
    }
};