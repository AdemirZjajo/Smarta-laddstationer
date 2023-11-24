#ifndef DISPLAY_INFO_HPP
#define DISPLAY_INFO_HPP

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RESET -1 // Reset pin
class DisplayController
{
public:
    DisplayController();
    DisplayController(int resetPin);
    void begin();

    // Arrays of strings for random selection
    static char *batteryOptions[];
    static char *weightOptions[];
    static char *loadTypeOptions[];
    static int arraySize;

    String getRandom(const char *options[], int size);
    void initialize();
    void updateDisplay();
    void loopCommunication();
    void clearDisplay();

private:
    Adafruit_SSD1306 display;
    String weight;
    String battery;
    String loadType;
};
#endif