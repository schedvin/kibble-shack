#pragma once

#include "Networking.h"
#include "BasePageHandler.h"
#include <vector>


class ApiHost {
private:
    AsyncWebServer _server;
    uint16_t _port;
    std::vector<BasePageHandler*> _handlers;

    void setupDefaultRoutes();


public:
    explicit ApiHost(uint16_t port = 80);
    void addHandler(BasePageHandler* handler);
    void begin();
    void handle();

    AsyncWebServer* getServer() { return &_server; }
};