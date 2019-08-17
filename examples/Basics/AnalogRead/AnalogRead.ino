/*
    Name:       AnalogRead.ino
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
  Makey.Lcd.println("Analog Test");
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.setCursor(0, 60);
  Makey.Lcd.println("Joy_LX--P32");
  Makey.Lcd.println("Joy_LY--P33");
  Makey.Lcd.println("Joy_RX--P35");
  Makey.Lcd.println("Joy_RY--P34");
  Makey.Lcd.println("Pot_L --P39");
  Makey.Lcd.println("Pot_R --P36");
  delay(1000);
  Makey.Lcd.setTextSize(1);
  Makey.Lcd.clear(BLACK);
  Makey.Lcd.setCursor(0, 24);
  Makey.Lcd.println("LX: ");
  Makey.Lcd.println("LY: ");
  Makey.Lcd.println("RX: ");
  Makey.Lcd.println("RY: ");
  Makey.Lcd.println("PL: ");
  Makey.Lcd.println("PR: ");
  Makey.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
}

// Add the main program code into the continuous loop() function
void loop() {
  // Makey.update();
  // Makey.Lcd.setCursor(50, 24);Makey.Lcd.println(analogRead(JOY_LX_PIN));
  // Makey.Lcd.setCursor(50, 46);Makey.Lcd.println(analogRead(JOY_LY_PIN));
  // Makey.Lcd.setCursor(50, 68);Makey.Lcd.println(analogRead(JOY_RX_PIN));
  // Makey.Lcd.setCursor(50, 90);Makey.Lcd.println(analogRead(JOY_RY_PIN));
  // Makey.Lcd.setCursor(50, 112);Makey.Lcd.println(analogRead(POT_L_PIN));
  // Makey.Lcd.setCursor(50, 134);Makey.Lcd.println(analogRead(POT_R_PIN));
  Makey.Lcd.drawNumber(analogRead(JOY_LX_PIN), 50, 12);
  Makey.Lcd.drawNumber(analogRead(JOY_LY_PIN), 50, 34);
  Makey.Lcd.drawNumber(analogRead(JOY_RX_PIN), 50, 56);
  Makey.Lcd.drawNumber(analogRead(JOY_RY_PIN), 50, 78);
  Makey.Lcd.drawNumber(analogRead(POT_L_PIN), 50, 100);
  Makey.Lcd.drawNumber(analogRead(POT_R_PIN), 50, 122);
  delay(50);
}
