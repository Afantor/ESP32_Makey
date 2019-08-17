/*
Name:   Config.h
Version:  1.0.0
Created:  6/13/2019 12:03:48 AM
Author:   Afantor
Github:   https://github.com/Afantor/Afantor-Cubic32.git
Copyright (c) 2019 Afantor
*/

#pragma once

#ifndef _CONFIG_H_
#define _CONFIG_H_
// Screen LCD_TFT
#define TFT_BL_PIN    		27
#define TFT_MISO_PIN  		-1
#define TFT_MOSI_PIN  		23
#define TFT_SCL_PIN   		18
#define TFT_CS_PIN    		15    // Chip select control pin
#define TFT_DC_PIN    		2   // Data Command control pin
#define TFT_RST_PIN   		4    // Reset pin (could connect to RST pin)
//#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

// SD card
#define TFCARD_CS_PIN 		5
#define TFCARD_MOSI_PIN 	23
#define TFCARD_CLK_PIN 		18
#define TFCARD_MISO_PIN 	19


// Buttons
#define BTN_L 				1
#define BTN_R 				2
#define BTN_MODE 			3
#define BTN_SET 			4
#define BTN_START 			5

#define BUTTON_L 			1
#define BUTTON_R 			2
#define BUTTON_MODE			3
#define BUTTON_SET 			4
#define BUTTON_START		5


#define BUTTON_L_PIN 		12
#define BUTTON_R_PIN 		14
#define BUTTON_MODE_PIN 	16
#define BUTTON_START_PIN 	13
#define BUTTON_SET_PIN 		0

//Joystick XY and Potentiometer
#define JOY_LX_PIN 		33
#define JOY_LY_PIN 		32
#define JOY_RX_PIN 		35
#define JOY_RY_PIN 		34
#define POT_L_PIN 		39
#define POT_R_PIN 		36

//LED Pin
#define LED_1_PIN		25
#define LED_2_PIN		26
// UART
#define USE_SERIAL Serial

#define FALSE  (0)
#define TRUE   (1)

#endif /* SETTINGS_C */
