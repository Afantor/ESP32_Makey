#include <Makey32.h>

void setup() {
  Makey.begin();
  Makey.Power.begin();
  Makey.Lcd.setBrightness(200);

  Makey.Power.setWakeupButton(BUTTON_A_PIN);
  if(!Makey.Power.canControl()) {
    Makey.Lcd.printf("IP5306 is not i2c version\n");
  }
}

void loop() {

  Makey.update();
  bool c=Makey.Power.isResetbyPowerSW();
  bool d=Makey.Power.isResetbyDeepsleep();
 
  Makey.Lcd.printf("<<Sleep test>>\n");
  Makey.Lcd.printf("power-on triggered at:%s%s\n\n",c?"POWER-SW":"",d?"DeepSleep-end":"");
  
  Makey.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call Makey.Power.setPowerBoostKeepOn(false)
  Makey.Power.lightSleep(SLEEP_SEC(5));
  
  Makey.Lcd.printf("Go lightSleep (press buttonA wake up)\n");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call Makey.Power.setPowerBoostKeepOn(false)
  Makey.Power.lightSleep(0);
  
  Makey.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call Makey.Power.setPowerBoostKeepOn(false)
  Makey.Power.deepSleep(0);
  
}
