#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>
// #include "display.hpp"

// OLED inställningar
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RESET -1 // Reset pin
#define buttonAPin 19
#define buttonBPin 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET);
String weight = "0";
String battery = "0";
String loadType = "0";

// Arrays of strings for random selection
const char *batteryOptions[] = {
    " 10%", " 20%", " 30%", " 40%", " 50%",
    " 60%", " 70%", " 80%", " 90%", " 100%"};
const char *weightOptions[] = {
    " 10ton", " 9ton", " 8ton", " 7ton", " 6ton",
    " 5ton", " 4ton", " 3ton", " 2ton", " 1ton"};
const char *loadTypeOptions[] = {
    " *", "", " *", "", " *",
    "", " *", "", " *", ""};

const int arraySize = 10;

String getRandom(const char *options[], int size)
{
    int randomIndex = random(size);
    return options[randomIndex];
}

void setupDIS()
{
    // initialize with the I2C addr 0x3C (for the 128x32)

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    // display.setTextColor(WHITE);
    display.setTextColor(WHITE, BLACK);
    display.setTextSize(0, 1);
    // battery = getRandom(batteryOptions, arraySize);
    // weight = getRandom(weightOptions, arraySize);
    // loadType = getRandom(loadTypeOptions, arraySize);

    pinMode(buttonAPin, INPUT_PULLUP);
    pinMode(buttonBPin, INPUT_PULLUP);

    // randomSeed(analogRead(0));

    // setupCommunication();
}

void displayClear()
{
    display.clearDisplay();
}

void displayLooping(int id)
{
    display.clearDisplay();
    display.setCursor(32, 20);
    display.printf("Nod: %d", id);

    display.setCursor(32, 30);
    display.print("Battery:");
    display.setCursor(73, 30);
    display.println(battery);

    display.setCursor(32, 40);
    display.print("Last:");
    display.setCursor(55, 40);
    display.println(weight);

    display.setCursor(32, 50);
    display.print("Lasttyp:");
    display.setCursor(75, 50);
    display.println(loadType);

    display.display();
}

void clearArea(int x, int y)
{
    display.fillRect(x, y, 128, 9, BLACK);
}

void setID(int id)
{
    clearArea(32, 20);

    String temp = String(id);
    display.setCursor(32, 20);
    display.println("Nod " + temp);
    // display.println(temp);
    display.display();
}

void setBat(float battery)
{
    clearArea(32, 30);

    String temp = String(battery, 1) + "%";
    display.setCursor(32, 30);
    display.print("Bat " + temp);
    /*
    display.setCursor(60, 30);
    display.println(temp);
    */
    display.display();
}

void setWeight(int weight)
{
    clearArea(32, 40);

    String temp = String(weight);
    display.setCursor(32, 40);
    display.print("Last");
    display.setCursor(65, 40);
    display.println(temp);
    display.display();
}

void setLoadType(bool loadType)
{
    clearArea(32, 50);

    // String temp = String(loadType);
    display.setCursor(32, 50);
    display.print("Lasttyp ");
    display.setCursor(80, 50);
    if (loadType)
    {
        display.println("*");
    }
    else
    {
        display.println("-");
    }
    // display.println(temp);
    display.display();
}

void position(int x, int y)
{
    clearArea(32, 40);

    display.setCursor(32, 40);
    display.print("Pos X");
    display.setCursor(62, 40);
    display.print(x);
    display.print("-Y");
    display.println(y);
    display.display();
}

void location(int current_CS_ID)
{
    String temp;
    if (current_CS_ID == 1){
        temp = String("1");
    }
    else if (current_CS_ID == 2){
        temp = String("2");        
    }
    else if (current_CS_ID == 3){
        temp = String("3");        
    }
    else if (current_CS_ID == 4){
        temp = String("4");        
    }
    else {
        temp = String("?");
    }

    clearArea(32, 40);

    display.setCursor(32, 40);
    display.print("Station " + temp);
    display.display();
}

void destination(String dest)
{
    clearArea(32, 50);

    String temp = String(dest);
    display.setCursor(32, 50);
    display.print("Dest ");
    display.setCursor(75, 50);
    display.println(temp);
    display.display();
}

void queuePoints(float point)
{
    clearArea(32, 50);

    int temp = int(point);
    display.setCursor(32, 50);
    display.print("Points ");
    display.setCursor(74, 50);
    display.println(temp);
    display.display();
}

void loading()
{
    clearArea(32, 50);

    display.setCursor(32, 50);
    display.print("Laddar...");
    display.display();
}

/*DisplayController::DisplayController(new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET))
{

}

   void initialize() {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(0, 1);

        pinMode(buttonAPin, INPUT_PULLUP);
        pinMode(buttonBPin, INPUT_PULLUP);

        randomSeed(analogRead(0));


    }

    void updateDisplay() {

        updateCommunication();
        display.clearDisplay();

        // Display "Nod 1" at the static x-position
        display.setCursor(35, 20);
        display.printf("Nod: %d", getID());

        display.setCursor(32, 40);
        display.print("Last:");
        display.setCursor(55, 40);
        display.println(weight);

        display.setCursor(32, 30);
        display.print("Batteri:");
        display.setCursor(73, 30);
        display.println(battery);

        display.setCursor(32, 50);
        display.print("Lasttyp:");
        display.setCursor(75, 50);
        display.println(loadType);

        display.display();

        delay(100);
    }

        Adafruit_SSD1306 display;
        String weight = "0";
        String battery = "0";
        String loadType = "0";

        // Arrays of strings for random selection
        const char* batteryOptions[] = {
            " 10%", " 20%", " 30%", " 40%", " 50%",
            " 60%", " 70%", " 80%", " 90%", " 100%"
        };
        const char* weightOptions[] = {
            " 10ton", " 9ton", " 8ton", " 7ton", " 6ton",
            " 5ton", " 4ton", " 3ton", " 2ton", " 1ton"
        };
        const char* loadTypeOptions[] = {
            " *", "", " *", "", " *",
            "", " *", "", " *", ""
        };
        const int arraySize = 10; // Size of the arrays

        String getRandom(const char* options[], int size) {
            int randomIndex = random(size);
            return options[randomIndex];
        }*/

// Initialize the OLED display using Arduino Wire
// Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT , &Wire, RESET);

// int buttonAState = digitalRead(buttonAPin);
// int buttonBState = digitalRead(buttonBPin);

/*if (buttonAState == LOW) { // Assumes active LOW (button pressed gives LOW)
    // Button A is pressed
    // Action for button A
    display.setCursor(32, 40);
    display.print("Last:");
    display.setCursor(55, 40);
    display.println(weight);
}

if (buttonBState == ESP_LOG_WARN) { // Assumes active LOW
    // Button B is pressed
    // Action for button B
    display.setCursor(32, 30);
    display.print("Battery:");
    display.setCursor(73, 30);
    display.println(battery);
}*/
// Static information

// const int buttonAPin = GPIO_NUM_22;
// const int buttonBPin = GPIO_NUM_21;
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
