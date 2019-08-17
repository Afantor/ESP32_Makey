/*
    Name:       button.ino
    Created:    2019/5/21 14:06:15
    Author:     Afantor
*/

#include <Makey32.h>
// The setup() function runs once each time the micro-controller starts
void setup() {
  // init lcd, serial, but don't init sd card
  Makey.begin();
  Makey.Lcd.clear(BLACK);
  Makey.Lcd.setTextColor(YELLOW);
  Makey.Lcd.setTextSize(2);
  Makey.Lcd.setCursor(0, 24);
  Makey.Lcd.println("Button Test");
  Makey.Lcd.setCursor(0, 60);
  Makey.Lcd.println("Press BTN Set");
  Makey.Lcd.println("for 700ms to");
  Makey.Lcd.println("clear screen.");
  Makey.Lcd.setTextColor(RED);
}

// Add the main program code into the continuous loop() function
void loop() {
  Makey.update();
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (Makey.BtnL.wasReleased()) {
    Makey.Lcd.print('L');
  } else if (Makey.BtnR.wasReleased()) {
    Makey.Lcd.print('R');
  } else if (Makey.BtnMode.wasReleased()) {
    Makey.Lcd.print('M');
  }else if (Makey.BtnStart.wasReleased()) {
    Makey.Lcd.print('S');
  }else if (Makey.BtnSet.wasReleased()) {
    Makey.Lcd.print('T');
  } else if (Makey.BtnSet.wasReleasefor(700)) {
    Makey.Lcd.clear(BLACK);
    Makey.Lcd.setCursor(0, 24);
  }
}
