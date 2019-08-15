#include <Makey32.h>
#include "WiFi.h"
#include <String.h>

int n;
int ssidLength = 14;
int thisPage = 0;
const int pageSize = 9;
bool on = false;
bool leftLocked = false;
bool rightLocked = false;

void setup()
{
  Makey.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Makey.Lcd.setTextSize(2);
  Makey.Lcd.setCursor(0, 110);
  Makey.Lcd.printf("WiFi scanner");
  delay(1000);
  DrawMenu();
}

void LCD_Clear() {
  Makey.Lcd.fillScreen(BLACK);
  Makey.Lcd.setCursor(0, 0);
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.setTextColor(TFT_YELLOW);
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
      Makey.Lcd.print(i + 1);
      String ssid = (WiFi.SSID(i).length() > ssidLength) ? (WiFi.SSID(i).substring(0, ssidLength) + "...") : WiFi.SSID(i);
      Makey.Lcd.print(") " + ssid + " (" + WiFi.RSSI(i) + ");\n");
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
  n = WiFi.scanNetworks();
  Show();
}

void loop()
{
  // if (Makey.BtnA.wasPressed()) Show(-1);
  // if (Makey.BtnB.wasPressed()) Search();
  // if (Makey.BtnC.wasPressed()) Show(1);
  // Makey.update();
  Search();
  delay(20000);
}