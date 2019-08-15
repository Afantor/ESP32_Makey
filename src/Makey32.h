// Copyright (c) Afantor. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2019, Afantor
 * \class Makey32
 * \brief   Makey32 library.
 * @file    Makey32.h
 * @author  Afantor
 * @version V0.3.0
 * @date    2019/01/01
 * @brief   Header for Makey32.cpp module
 *
 * \par Description
 * This file is a drive for Makey32 Board.
 *
 * \par Method List:
 *
 *  System:
        Cubic.begin();
        Cubic.update();

    LCD:
        Cubic.Lcd.setBrightness(uint8_t brightness);
        Cubic.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        Cubic.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        Cubic.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        Cubic.Lcd.fillScreen(uint16_t color);
        Cubic.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        Cubic.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        Cubic.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        Cubic.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        Cubic.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        Cubic.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        Cubic.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        Cubic.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        Cubic.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        Cubic.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        Cubic.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        Cubic.Lcd.setCursor(uint16_t x0, uint16_t y0);
        Cubic.Lcd.setTextColor(uint16_t color);
        Cubic.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        Cubic.Lcd.setTextSize(uint8_t size);
        Cubic.Lcd.setTextWrap(boolean w);
        Cubic.Lcd.printf();
        Cubic.Lcd.print();
        Cubic.Lcd.println();
        Cubic.Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        Cubic.Lcd.drawRightString(const char *string, int dX, int poY, int font);
        Cubic.Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        Cubic.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        Cubic.Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

    Button:
        Cubic.BtnA/B/C.read();
        Cubic.BtnA/B/C.isPressed();
        Cubic.BtnA/B/C.isReleased();
        Cubic.BtnA/B/C.wasPressed();
        Cubic.BtnA/B/C.wasReleased();
        Cubic.BtnA/B/C.wasreleasedFor()
        Cubic.BtnA/B/C.pressedFor(uint32_t ms);
        Cubic.BtnA/B/C.releasedFor(uint32_t ms);
        Cubic.BtnR.lastChange();
        Cubic.BtnR.wasPressed();
        Cubic.BtnR.wasReleased();

 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Afantor Lan         2019/01/01        0.0.1          Rebuild the new.
 * Afantor Lan         2018/05/20        0.3.0          Add Touch API
 * </pre>
 *
 */

#ifndef _MAKEY32_H_
#define _MAKEY32_H_
  
  #if defined(ESP32)

    #define LCD_INSDE

    #include "gitTagVersion.h"
    #include <Arduino.h>
    #include <Wire.h>
    #include <SPI.h>
    #include <Ticker.h>
    #include <FS.h>
    #include <SD.h>
    #include <BLEDevice.h>
    #include <BLEServer.h>
    #include <BLEUtils.h>
    #include <BLE2902.h>
    #include <BLEScan.h>
    #include <BLEAdvertisedDevice.h>

    #include "LCDDisplay.h"
    #include "Free_Fonts.h"
    #include "utility/Config.h"
    #include "utility/Button.h"
    #include "utility/I2CBus.h"
    #include "utility/BMI160.h"

    class Makey32
    {
      public:
        Makey32();
        void begin(bool LCDEnable = true, bool SerialEnable = true, bool I2CEnable = false, bool SDEnable = false);
        void update();

        // Button API
        #define DEBOUNCE_MS 10
        Button BtnL     = Button(BUTTON_L_PIN, true, DEBOUNCE_MS);
        Button BtnR     = Button(BUTTON_R_PIN, true, DEBOUNCE_MS);
        Button BtnMode  = Button(BUTTON_MODE_PIN, true, DEBOUNCE_MS);
        Button BtnStart = Button(BUTTON_START_PIN, true, DEBOUNCE_MS);
        Button BtnSet   = Button(BUTTON_SET_PIN, true, DEBOUNCE_MS);
          
        // I2C
        I2CBus I2C = I2CBus();

        // LCD
        #ifdef LCD_INSDE
        LCDDisplay Lcd = LCDDisplay();
        #endif          
        /**
        * Function has been move to Power class.(for compatibility)
        * This name will be removed in a future release.
        */
        // void setPowerBoostKeepOn(bool en) __attribute__((deprecated));
        // void setWakeupButton(uint8_t button) __attribute__((deprecated));
        // void powerOFF() __attribute__((deprecated));
        
      private:
          bool isInited;
    };
    
    extern Makey32 Makey;
    #define mk Makey
    #define lcd Lcd
  #else
    #error “This library only supports boards with ESP32 processor.”
  #endif
#endif
