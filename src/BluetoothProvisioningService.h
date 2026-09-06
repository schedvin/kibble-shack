#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> // Required for BLE Notifications



class BluetoothCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      
      if (rxValue.length() > 0) {
        size_t delimiterIndex = rxValue.find(";;;");
        if (delimiterIndex != std::string::npos) {
          // newSSID = String(rxValue.substr(0, delimiterIndex).c_str());
          // newPassword = String(rxValue.substr(delimiterIndex + 3).c_str());
          
          Serial.println("\n[BLE] Received new WiFi Credentials");
        }
      }
    }
};



class BluetoothProvisioningService {
private:
public:
    BluetoothProvisioningService();
    bool startProvisioning();
};
