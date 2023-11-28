#pragma once
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <string>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RESET -1 // Reset pin
#define buttonAPin 19
#define buttonBPin 18

// Declaration of global variables and functions
extern Adafruit_SSD1306 display;
extern String weight;
extern String battery;
extern String loadType;

// Array sizes
const int arraySize = 10;

// Function declarations
void setupCommunication();
String getRandom(const char *options[], int size);

void setupDIS();
void displayLooping(int id);

void displayClear();

void setID(int id);
void setBat(float battery);
void setWeight(int weight);
void setLoadType(bool loadType);
void queuePoints(float points);
void loading();
void destination(String dest);

void clearArea();
void position(int x, int y);

#endif