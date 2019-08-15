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
  Makey.Lcd.setCursor(65, 10);
  Makey.Lcd.println("Button example");
  Makey.Lcd.setCursor(3, 35);
  Makey.Lcd.println("Press button START for 700ms");
  Makey.Lcd.println("to clear screen.");
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
  } else if (Makey.BtnMODE.wasReleased()) {
    Makey.Lcd.print('MODE');
  }else if (Makey.BtnSTART.wasReleased()) {
    Makey.Lcd.print('START');
  }else if (Makey.BtnSET.wasReleased()) {
    Makey.Lcd.print('SET');
  } else if (Makey.BtnSET.wasReleasefor(700)) {
    Makey.Lcd.clear(BLACK);
    Makey.Lcd.setCursor(0, 0);
  }
}
