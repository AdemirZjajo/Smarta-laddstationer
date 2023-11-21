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

String weight = "0";
String battery = "0";
String loadType = "0";

// Arrays of strings for random selection
const char* batteryOptions[] = {
    " 10%", " 20%", " 30%", " 40%", " 50%",
    " 60%", " 70%", " 80%", " 90%", " 100%"
};
const char* weightOptions[] = {
    " 1000 kg", " 900 kg", " 800 kg", " 700 kg", " 600 kg",
    " 500 kg", " 400 kg", " 300 kg", " 200 kg", " 100 kg" 
};
const char* loadTypeOptions[] = {
    " *", "", " *", "", " *",
    "", " *", "", " *", ""
};
const int arraySize = 10; // Size of the arrays

String getRandom(const char* options[], int size) {
    int randomIndex = random(size); 
    return options[randomIndex]; 
}

void setup() {
    // initialize with the I2C addr 0x3C (for the 128x32)
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(0,1);
    battery = getRandom(batteryOptions, arraySize);
    weight = getRandom(weightOptions, arraySize);
    loadType = getRandom(loadTypeOptions, arraySize);

    randomSeed(analogRead(0));
}



void loop() {
    display.clearDisplay();

    // Display "Nod 1" at the static x-position
    display.setCursor(50, 20);
    display.println("Nod 1");

    //Static information
    display.setCursor(32, 30);
    display.print("Battery:");
    display.setCursor(32, 40);
    display.print("Last:");
    display.setCursor(32, 50);
    display.print("Lasttyp:");

    // Moving horizontally
    display.setCursor(73, 30);
    display.println(battery);
    display.setCursor(55, 40);
    display.println(weight);
    display.setCursor(75, 50);
    display.println(loadType);

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

