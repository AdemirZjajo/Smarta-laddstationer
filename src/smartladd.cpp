// Programmets huvudfil

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED inställningar 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RESET    -1  // Reset pin 

// Initialize the OLED display using Arduino Wire
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT , &Wire, RESET);

int nodXPos = 35; // X-position for "Nod 1"
int otherLinesXPos = SCREEN_WIDTH; // Initial x-position for other lines, starting from the right


void setup() {
    // initialize with the I2C addr 0x3C (for the 128x32)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(0,1);
}

void loop() {
    display.clearDisplay();

    // Display "Nod 1" at the static x-position
    display.setCursor(nodXPos, 20);
    display.println("Nod 1");

    otherLinesXPos -= 2;
    if (otherLinesXPos < - SCREEN_WIDTH) {
    otherLinesXPos = SCREEN_WIDTH;
    }

     // Display other lines at the updated x-position
    display.setCursor(otherLinesXPos, 30);
    display.println("Battery: 90%");
    display.setCursor(otherLinesXPos + 10, 40);
    display.println("Last: 1000 kg ");
    display.setCursor(otherLinesXPos + 10, 50);
    display.println("Last typ: * ");

    display.display();

    delay(100);
}