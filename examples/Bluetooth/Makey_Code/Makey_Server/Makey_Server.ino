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
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
float txValue = 0;
const int readPin = 32; // Use GPIO number. See ESP32 board pinouts
const int LED = 2; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.

//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();

        // Do stuff based on the command received from the app
        if (rxValue.find("A") != -1) { 
          Serial.println("Turning ON!");
          digitalWrite(LED, HIGH);
        }
        else if (rxValue.find("B") != -1) {
          Serial.println("Turning OFF!");
          digitalWrite(LED, LOW);
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.println("Starting BLE work!");
  // Create the BLE Device
  BLEDevice::init("MakeyServer");
  //将BLE设备设置为服务器
  BLEServer *pServer = BLEDevice::createServer();
   pServer->setCallbacks(new MyServerCallbacks());
  //使用之前定义的UUID为BLE服务器创建服务
  BLEService *pService = pServer->createService(SERVICE_UUID);
  //为该服务设置特征,并且需要将特征的属性作为参数传递。在这种情况下，它是：READ和WRITE
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY|
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setReadProperty(true);
  pCharacteristic->setCallbacks(new MyCallbacks());
  //创建特征后，可以使用setValue()方法设置其值
  pCharacteristic->setValue("Hello World!");
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
  if (deviceConnected) {
    // Fabricate some arbitrary junk for now...
//     txValue = analogRead(readPin) / 3.456; // This could be an actual sensor reading!

//     // Let's convert the value to a char array:
//     char txString[8]; // make sure this is big enuffz
//     dtostrf(txValue, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer
    
// //    pCharacteristic->setValue(&txValue, 1); // To send the integer value
// //    pCharacteristic->setValue("Hello!"); // Sending a test message
//     pCharacteristic->setValue(txString);
    
//     pCharacteristic->notify(); // Send the value to the app!
//     Serial.print("*** Sent Value: ");
//     Serial.print(txString);
//     Serial.println(" ***");
    Serial.println("MakeyServer connected.");
    delay(5000);
  }
  else
  {
    delay(1000);
    Serial.println("MakeyServer Disconnected.");
  }
  
}