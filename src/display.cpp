// Programmets huvudfil
// Prata med nod.cpp för att få information

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

// Arrays of strings for random selection
const char* batteryOptions[] = {" 90%", " 75%", " 60%", " 45%"};
const char* lastOptions[] = {" 1000 kg", " 750 kg", " 500 kg", " 250 kg"};
const char* lastTypOptions[] = {" *", " ", " *", " "};
const int arraySize = 4; // Size of the arrays

void setup() {
    // initialize with the I2C addr 0x3C (for the 128x32)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(0,1);

    randomSeed(analogRead(0));
}

String getRandomText(const char* options[], int size) {
    int randomIndex = random(size); 
    return options[randomIndex]; 
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

    //Static information
    display.setCursor(32, 30);
    display.print("Battery:");
    display.setCursor(32, 40);
    display.print("Last:");
    display.setCursor(32, 50);
    display.print("Lasttyp:");

    // Moving horizontally
    display.setCursor(73, 30);
    display.println(" 90%");
    display.setCursor(55, 40);
    display.println(" 1000kg");
    display.setCursor(75, 50);
    display.println(" * ");

    //Moving horizontally and getting a random selected status
   /* display.setCursor(73, 30);
    display.println(getRandomText(batteryOptions, arraySize));
    display.setCursor(55, 40);
    display.println(getRandomText(lastOptions, arraySize));
    display.setCursor(75, 50);
    display.println(getRandomText(lastTypOptions, arraySize));*/

     // Display other lines at the updated x-position
   /* display.setCursor(otherLinesXPos, 30);
    display.println("Battery: 90%");
    display.setCursor(otherLinesXPos + 10, 40);
    display.println("Last: 1000 kg ");
    display.setCursor(otherLinesXPos + 10, 50);
    display.println("Last typ: * ");*/

    display.display();

    delay(100);
}

/*
Pseudo code to use button A and B

initialize buttonA as input
initialize buttonB as input

loop:
    if buttonA is pressed:
        execute ActionA
    if buttonB is pressed:
        execute ActionB
*/

/*
In real code

const int buttonAPin = [GPIO_PIN_NUMBER_FOR_A]; // Replace [GPIO_PIN_NUMBER_FOR_A] with the actual GPIO pin number
const int buttonBPin = [GPIO_PIN_NUMBER_FOR_B]; // Replace [GPIO_PIN_NUMBER_FOR_B] with the actual GPIO pin number

void setup() {
    pinMode(buttonAPin, INPUT_PULLUP); // Use INPUT if you have an external pull-up resistor
    pinMode(buttonBPin, INPUT_PULLUP); // Use INPUT if you have an external pull-up resistor
}

void loop() {
    int buttonAState = digitalRead(buttonAPin);
    int buttonBState = digitalRead(buttonBPin);

    if (buttonAState == LOW) { // Assumes active LOW (button pressed gives LOW)
        // Button A is pressed
        // Action for button A
    }

    if (buttonBState == LOW) { // Assumes active LOW
        // Button B is pressed
        // Action for button B
    }
}
*/

