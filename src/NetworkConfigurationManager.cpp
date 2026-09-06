#include <string>
#include <WiFi.h>
#include "NetworkConfigurationManager.h"

NetworkConfigurationManager::NetworkConfigurationManager(
    const std::string hostname,
    const std::string ssid,
    const std::string password) : hostname(hostname), ssid(ssid), password(password), lastReconnectAttempt(0) {}

bool NetworkConfigurationManager::begin()
{

    WiFi.setHostname(this->hostname.c_str());

    if (WiFi.begin(this->ssid.c_str(), this->password.c_str()))
    {
        // Start mDNS Responder
        if (MDNS.begin(this->hostname.c_str()))
        {
            delay(100);
            MDNS.addService("http", "tcp", 80);
            Serial.printf("[mDNS] Responder active at http://%s.local\n", this->hostname);
        }
        else
        {
            Serial.println("[mDNS] Failed to start mDNS responder.");
        }
    }

    return true;
}

void NetworkConfigurationManager::loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        unsigned long now = millis();
        // Retry connection every 10 seconds without blocking main loop
        if (now - this->lastReconnectAttempt >= 10000)
        {
            this->lastReconnectAttempt = now;
            Serial.println("[Network] Connection lost. Attempting auto-reconnect...");
            WiFi.reconnect();
        }
    }
}