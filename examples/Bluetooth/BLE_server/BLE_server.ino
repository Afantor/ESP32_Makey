/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara


要创建BLE服务器，代码应遵循以下步骤：

1.创建BLE服务器。在这种情况下，ESP32充当BLE服务器。
2.创建BLE服务。
3.在服务上创建BLE特征。
4.在特征上创建BLE描述符。
5.启动服务。
6.开始做广告，以便其他设备可以找到它。

本例程可以使用两块ESP32，一块做扫描，一块做服务器。
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // Create the BLE Device
  BLEDevice::init("Long name works now");
  //将BLE设备设置为服务器
  BLEServer *pServer = BLEDevice::createServer();
  //使用之前定义的UUID为BLE服务器创建服务
  BLEService *pService = pServer->createService(SERVICE_UUID);
  //为该服务设置特征,并且需要将特征的属性作为参数传递。在这种情况下，它是：READ和WRITE
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  //创建特征后，可以使用setValue()方法设置其值
  pCharacteristic->setValue("Hello World says Neil");
  //启动服务和广告，以便其他BLE设备可以扫描并找到此BLE设备
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}