#include "Globals.h"
#include "BluetoothProvisioningService.h"
#include "ConfigurationManager.h"

#define SERVICE_UUID "1710ccf7-8e1b-4e1d-9070-d359796f0a35"
#define CHARACTERISTIC_UUID "14c7022c-54a8-4259-8097-5ac2e5cc7bf4"
#define IP_ADDRESS_UUID "a1b2c3d4-e5f6-4a5b-8c9d-0e1f2a3b4c5d"

BluetoothProvisioningService::BluetoothProvisioningService() {}

bool BluetoothProvisioningService::startProvisioning()
{
    try
    {
        BLEDevice::init(BLE_ADVERTISING_DEVICE_NAME);
        BLEServer *bleServer = BLEDevice::createServer();
        BLEService *bleService = bleServer->createService(SERVICE_UUID);

        BLECharacteristic *pCredsChar = bleService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_WRITE);
        pCredsChar->setCallbacks(new BluetoothCharacteristicCallbacks());

        BLECharacteristic *ipAddressCharacteristic = bleService->createCharacteristic(
            IP_ADDRESS_UUID,
            BLECharacteristic::PROPERTY_NOTIFY);
        ipAddressCharacteristic->addDescriptor(new BLE2902());

        bleService->start();

        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        BLEDevice::startAdvertising();

        return true;
    }
    catch (const std::exception &e)
    {
        Serial.print("Exception caught in provisioning: ");
        Serial.println(e.what());
        return false;
    }
    catch (...)
    {
        Serial.println("Unknown exception caught in provisioning!");
        return false;
    }
}