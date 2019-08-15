#include <Makey32.h>
#include <String.h>

int n;
int ssidLength = 14;
int thisPage = 0;
const int pageSize = 5;
bool on = false;
bool leftLocked = false;
bool rightLocked = false;

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
BLEScanResults foundDevices;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Makey.begin();
  Serial.println("BLE Scanning...");
  Makey.Lcd.setTextSize(2);
  Makey.Lcd.setCursor(0, 110);
  Makey.Lcd.printf("BLE Scanner...");
  delay(1000);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  DrawMenu();
}

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
    Makey.Lcd.printf("RESCAN");
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

void loop() {
  // put your main code here, to run repeatedly:
  foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  n = foundDevices.getCount();
  Serial.println(n);
  Serial.println("Scan done!");
  Search();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
  delay(5000);
}