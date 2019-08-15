#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <driver/adc.h>

BLECharacteristic *pCharacteristic;


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() 
{
	Serial.begin(115200);
	Serial.println("Starting BLE work!");

	BLEDevice::init("ESP32Server");
	BLEServer *pServer = BLEDevice::createServer();
	BLEService *pService = pServer->createService(SERVICE_UUID);
	pCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	pService->start();
	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->addServiceUUID(pService->getUUID());
	pAdvertising->start();
}

int count = 0;

// this loop just publishes the value of count and then 
// increments it. You should see a stream of messages at 
// one second intervals.

void loop() 
{
	Serial.print("Sending: ");
	Serial.println(count);
	char buffer[80];
	dtostrf(count, 1, 2, buffer);
	pCharacteristic->setValue(buffer);
	pCharacteristic->notify(); // send notification of change
	count++;
	delay(1000);
}