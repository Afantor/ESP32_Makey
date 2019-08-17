#include <Makey32.h>

void setup(void) {

  // Set all chip selects high to avoid bus contention during initialisation of each peripheral
  // digitalWrite(32, HIGH); // Touch controller chip select (if used)
  digitalWrite(15, HIGH); // TFT screen chip select
  digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)

  Makey.begin();

  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  Serial.println("initialisation done.");

  Makey.Lcd.setRotation(1);  // portrait
  Makey.Lcd.fillScreen(random(0xFFFF));

  //Draw the jpeg file "p2.jpg" from TF(SD) card
  // The image is 300 x 300 pixels so we do some sums to position image in the middle of the screen!
  // Doing this by reading the image width and height from the jpeg info is left as an exercise!
  int x = (Makey.Lcd.width()  - 240) / 2 - 1;
  int y = (Makey.Lcd.height() - 240) / 2 - 1;
  
  // Makey.Lcd.drawJpgFile(SD, "/m5_logo.jpg");
  Makey.Lcd.drawJpgFile(SD, "/m5_logo.jpg", x, y);// This draws a jpeg pulled off the SD Card
  // Makey.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y, uint16_t maxWidth, uint16_t maxHeight, uint16_t offX, uint16_t offY, jpeg_div_t scale);
}

void loop() {

}
