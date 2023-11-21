#ifndef DISPLAY_INFO_HPP
#define DISPLAY_INFO_HPP

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RESET -1 // Reset pin

extern Adafruit_SSD1306 display;

extern String weight;
extern String battery;
extern String loadType;

// Arrays of strings for random selection
extern const char* batteryOptions[];
extern const char* weightOptions[];
extern const char* loadTypeOptions[];
extern const int arraySize;

String getRandom(const char* options[], int size);

#endif 