/************************************************************************

  Makey32 I2C Scanner

  The Makey32 has a connector for I2C devices. 
  This program scans the addresses 1-127 continuosly and shows 
  the devices found on the TFT.

  The Makey32 Core has four internal I2C devices at address 0x68 and 0x75.
  
  If they do not appear on the TFT it could mean you made a short cut on 
  the I2C bus.

  October 2019, Afantor

************************************************************************/
#include <Makey32.h>

void setup() 
{
  Makey.begin();
  Makey.Lcd.begin();
  Makey.Lcd.fillScreen( BLACK );
  Makey.Lcd.setCursor(0, 14);
  Makey.Lcd.setTextColor(YELLOW);  
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.println("Makey32 I2C Tester");
    
  Wire.begin();

  delay(3000);
  Makey.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void loop() 
{
  int address;
  int error;
  Makey.Lcd.setCursor(0, 30);
  Makey.Lcd.println("scanning Address [HEX]");
  
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if(error==0)
    {
      Makey.Lcd.println("");
      Makey.Lcd.print(" 0x");
      Makey.Lcd.print(address,HEX);Makey.Lcd.println(" ");
    }
    else Makey.Lcd.print(".");

    delay(10);
  }

  if(textColor==YELLOW) textColor=GREEN;
  else textColor=YELLOW;
  Makey.Lcd.setTextColor(textColor,BLACK);  
}
