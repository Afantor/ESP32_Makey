#include <Makey32.h>

void setup() {
  Makey.begin();

  // Display QRCode
  Makey.Lcd.qrcode("http://www.afantor.cc");
  // Makey.Lcd.qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
}

void loop() {
}
