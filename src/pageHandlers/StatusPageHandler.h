// StatusPageHandler.h
#pragma once

#include "../BasePageHandler.h"

class StatusPageHandler : public BasePageHandler {
private:
    void handleGetStatus(AsyncWebServerRequest *request);;
    void handleGetMetrics(AsyncWebServerRequest *request);;

public:
    void registerRoutes(AsyncWebServer& server) override;
};