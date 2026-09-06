#pragma once
#include <string>
#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

class NetworkConfigurationManager {
private:
    const std::string hostname;
    const std::string ssid;
    const std::string password;

    unsigned long lastReconnectAttempt;

public:
    NetworkConfigurationManager(const std::string hostname, const std::string ssid, const std::string password);
    bool begin();
    void loop();
};