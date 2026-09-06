#include <Arduino.h>
#include <LittleFS.h>

#include "ApiHost.h"
#include "BluetoothProvisioningService.h"
#include "ConfigurationManager.h"
#include "NetworkConfigurationManager.h"

#include "pageHandlers/ControlPageHandler.h"
#include "pageHandlers/StatusPageHandler.h"
#include "pageHandlers/WebSocketPageHandler.h"

ConfigurationManager config("network");
BluetoothProvisioningService *bleService = nullptr;
NetworkConfigurationManager *netConfig = nullptr;
ApiHost apiHost(80);

WebSocketPageHandler webSocketHandler;
StatusPageHandler statusHandler;
ControlPageHandler controlHandler(&webSocketHandler);

void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin(true))
    {
        Serial.println("An error occurred while mounting LittleFS");
        return;
    }

    config.begin();

    if (config.hasKeys({"hostname", "ssid", "password"}))
    {
        const std::string hostname = config.get("hostname", "catfeeder");
        const std::string ssid = config.get("ssid", "");
        const std::string password = config.get("password", "");

        netConfig = new NetworkConfigurationManager(hostname.c_str(), ssid.c_str(), password.c_str());

        if (netConfig->begin())
        {
            apiHost.addHandler(&webSocketHandler);
            apiHost.addHandler(&statusHandler);
            apiHost.addHandler(&controlHandler);
            apiHost.begin();
        }
    } else {

        bleService = new BluetoothProvisioningService();
        if(!bleService->startProvisioning()) {
            Serial.println("[EROR] Failed to start advertising");
        }
    }

    config.end();
}

void loop()
{
    if (netConfig != nullptr) {
    netConfig->loop();
    apiHost.handle();
    } else {
        Serial.println("Network no initialized, waiting for BLE provisioning");
    }

    delay(1000);

    // if (false) {
    //     webSocketHandler.broadcast("{\"event\":\"sound_detected\"}");
    // }
}