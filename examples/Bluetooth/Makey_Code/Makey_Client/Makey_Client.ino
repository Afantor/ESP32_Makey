/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include <Makey32.h>
#include <String.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

int n;
int ssidLength = 14;
int thisPage = 0;
const int pageSize = 5;
bool on = false;
bool leftLocked = false;
bool rightLocked = false;
bool menuLocked = false;

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
BLEScanResults foundDevices;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    menuLocked = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    LCD_Clear();
    Makey.Lcd.setTextSize(2);
    Makey.Lcd.setCursor(0, 110);
    Makey.Lcd.printf("Start Connect...");
    delay(2000);
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");
    LCD_Clear();
    Makey.Lcd.setTextSize(2);
    Makey.Lcd.setCursor(0, 110);
    Makey.Lcd.printf("Connected OK!");
    delay(2000);

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    menuLocked = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void LCD_Clear() {
  Makey.Lcd.fillScreen(BLACK);
  Makey.Lcd.setCursor(0, 0);
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.setTextColor(YELLOW);
}

void DrawMenu(){
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.setTextColor(BLUE);
  
  if (on == true)
  {
    Makey.Lcd.setCursor(80, 228);
    Makey.Lcd.printf("RESCON");
    if (thisPage != 0)
    {
      Makey.Lcd.setCursor(20, 228);
      Makey.Lcd.printf("<");
      leftLocked = false;
    }
    else
    {
      leftLocked = true;
    }
    if (thisPage < ((n - 1) / pageSize))
    {
      Makey.Lcd.setCursor(210, 228);
      Makey.Lcd.printf(">");
      rightLocked = false;
    }
    else
    {
      rightLocked = true;
    }
  }
  else
  {
    Makey.Lcd.setCursor(110, 228);
    Makey.Lcd.printf("SCAN");
  }
}

void Show(int nav = 0) // -1 top, 1 bottom
{
  if (nav == -1)
  {
    if ((on == true) && (leftLocked == false))
    {
      thisPage--;
      if (thisPage < 0) thisPage = 0;
      Show();
    }
  }
  else if (nav == 1)
  {
    if ((on == true) && (rightLocked == false))
    {
      if ((thisPage) <= (n / pageSize)) thisPage++;
      Show();
    }
  }
  else
  {
    LCD_Clear();
    Makey.Lcd.setCursor(64, 14);
    Makey.Lcd.print("TOTAL: ");
    Makey.Lcd.print(n);
    Makey.Lcd.setTextSize(1);
    Makey.Lcd.setCursor(0, 30);
    for (int i = (thisPage * pageSize); i < ((thisPage * pageSize) + pageSize); i++)
    {
      if (i >= n) break;
      BLEAdvertisedDevice d = foundDevices.getDevice(i);
      if(d.getRSSI() > -80)
      {
        Makey.Lcd.print(i + 1);
        if(d.haveName()){
          String blename = d.getName().c_str();
          String bleadd =  d.getAddress().toString().c_str();
          Makey.Lcd.print(") Nm:" + blename + ".Add:" + bleadd + " ("  + d.getRSSI() + ");\n");          
        }
        else{
          String bleadd =  d.getAddress().toString().c_str();
          Makey.Lcd.print(") Nm:.Add:" + bleadd + " ("  + d.getRSSI() + ");\n");
        }          
      }

    }
    DrawMenu();
  }
}

void Search() {
  on = true;
  LCD_Clear();
  Makey.Lcd.setTextSize(2);
  Makey.Lcd.setCursor(0, 100);
  Makey.Lcd.printf("Please, wait.");
  Makey.Lcd.setCursor(0, 130);
  Makey.Lcd.printf("Searching...");
  Show();
}

void setup() {
  Makey.begin();
  Serial.println("Starting Arduino BLE Client application...");
  Serial.println("BLE Scanning...");
  Makey.Lcd.setTextSize(2);
  Makey.Lcd.setCursor(0, 110);
  Makey.Lcd.printf("BLE Scanner...");
  delay(1000);
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  foundDevices = pBLEScan->start(5, false);
  Serial.print("Devices found: ");
  n = foundDevices.getCount();
  Serial.println(n);
  Serial.println("Scan done!");
  LCD_Clear();
  Makey.Lcd.setCursor(0, 110);
  Makey.Lcd.printf("Scan done!");  
  Search();
  delay(3000);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
} // End of setup.


// This is the Arduino main loop function.
void loop() {
  Makey.update();
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    // String newValue = "Time since boot: " + String(millis()/1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // // Set the characteristic's value to be the array of bytes that is actually a string.
    // pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());

   
    // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
    if (Makey.BtnL.wasReleased()) {
      Makey.Lcd.drawNumber(1, 50, 30);
      String newValue = "BTN_L Pressed.";
      Serial.println("BTN_L Pressed.");
      pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
      delay(100);
    } else if (Makey.BtnR.wasReleased()) {
      Makey.Lcd.drawNumber(1, 170, 30);
      String newValue = "BTN_R Pressed.";
      Serial.println("BTN_R Pressed.");
      pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
      delay(100);
    } else if (Makey.BtnMode.wasReleased()) {
      Makey.Lcd.drawNumber(1, 50, 118);
      String newValue = "BTN_Mode Pressed.";
      Serial.println("BTN_Mode Pressed.");
      pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
      delay(100);
    }else if (Makey.BtnStart.wasReleased()) {
      Makey.Lcd.drawNumber(1, 210, 118);
      String newValue = "BTN_Start Pressed.";
      Serial.println("BTN_Start Pressed.");
      pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
      delay(100);
    }else if (Makey.BtnSet.wasReleased()) {
      Makey.Lcd.drawNumber(1, 130, 118);
      String newValue = "BTN_Set Pressed.";
      Serial.println("BTN_Set Pressed.");
      pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
      delay(100);
    } else if (Makey.BtnSet.wasReleasefor(700)) {
      Makey.Lcd.clear(BLACK);
      Makey.Lcd.setCursor(0, 0);
    }
    else{
      Makey.Lcd.drawNumber(0, 50, 30);Makey.Lcd.drawNumber(0, 170, 30);
      Makey.Lcd.drawNumber(0, 50, 118);Makey.Lcd.drawNumber(0, 130, 118);Makey.Lcd.drawNumber(0, 210, 118);
      delay(20);
    }
    Makey.Lcd.drawNumber(analogRead(JOY_LX_PIN), 50, 52);
    Makey.Lcd.drawNumber(analogRead(JOY_LY_PIN), 50, 74);
    Makey.Lcd.drawNumber(analogRead(JOY_RX_PIN), 170, 52);
    Makey.Lcd.drawNumber(analogRead(JOY_RY_PIN), 170, 74);
    Makey.Lcd.drawNumber(analogRead(POT_L_PIN), 50, 96);
    Makey.Lcd.drawNumber(analogRead(POT_R_PIN), 170, 96);
  }else if(doScan){
    String Devname = myDevice->getName().c_str();
    LCD_Clear();
    Makey.Lcd.setTextSize(1);
    Makey.Lcd.setCursor(0, 14);
    Makey.Lcd.setTextColor(RED);
    Makey.Lcd.println(Devname + ": Disconnect");
    Makey.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
    delay(1000);
  }
  if(menuLocked){
    String Devname = myDevice->getName().c_str();
    LCD_Clear();
    Makey.Lcd.setTextSize(1);
    Makey.Lcd.setCursor(0, 14);
    Makey.Lcd.setTextColor(YELLOW);
    Makey.Lcd.println(Devname + ": Connected");
    Makey.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    Makey.Lcd.drawString("B_L:", 0, 30);Makey.Lcd.drawString("B_R:", 120, 30);
    Makey.Lcd.drawString("JLX:", 0, 52);Makey.Lcd.drawString("JRX:", 120, 52);
    Makey.Lcd.drawString("JLY:", 0, 74);Makey.Lcd.drawString("JRY:", 120, 74);
    Makey.Lcd.drawString("P_L:", 0, 96);Makey.Lcd.drawString("P_L:", 120, 96);
    Makey.Lcd.drawString("B_M:", 0, 118);Makey.Lcd.drawString("B_R:", 80, 118);Makey.Lcd.drawString("B_S:", 160, 118);
    menuLocked = false;
  }
  // delay(1000); // Delay a second between loops.
} // End of loop
