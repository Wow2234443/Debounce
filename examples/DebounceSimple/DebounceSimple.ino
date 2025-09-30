// ****************************************************************************
// Title        : Simple Debounce Example (Polling)
// File Name    : 'DebounceSimple.ino'
// Target MCU   : Espressif ESP32 (Doit DevKit Version 1)
// Description  : Demonstrates basic button debouncing with toggle functionality
//                using polling-based update method
//
// Revision History:
// When         Who         Description of change
// -----------  ----------- -----------------------
// 30-SEP-2025  Brooks      Initial example
//
// ****************************************************************************

// Include Files
// ****************************************************************************
#include <Debounce16.h>

// Constants
// ****************************************************************************
const uint8_t PIN_BUTTON = 17;                  // Button input pin
const uint8_t PIN_LED = 15;                     // LED output pin
const uint16_t INTERVAL_BLINK = 1000;           // Heartbeat blink interval (ms)
const uint16_t INTERVAL_DEBOUNCE = 1;           // Debounce update interval (ms)

// Globals
// ****************************************************************************
bool stateLed = false;                          // LED toggle state
bool stateHeartbeat = LOW;                      // Heartbeat LED state
unsigned long timeBlink = 0;                    // Last heartbeat time
unsigned long timeDebounce = 0;                 // Last debounce update time

// Setup button debouncing with clear parameter usage
const bool logicLevel = HIGH;                   // Active HIGH logic
Debounce16 button(PIN_BUTTON, logicLevel);      // Instantiate debounce object

// Setup Code
// ****************************************************************************
void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Debounce16 Library - Simple Example (Polling)");
    Serial.println("==============================================");
    Serial.println("- Press button to toggle LED");
    Serial.println("- Built-in LED shows heartbeat");
    Serial.println("==============================================");
    Serial.println();

    pinMode(PIN_LED, OUTPUT);                   // Configure LED as output
    digitalWrite(PIN_LED, LOW);                 // Initialize LED to OFF

    pinMode(LED_BUILTIN, OUTPUT);               // Configure built-in LED
}

// Main Program
// ****************************************************************************
void loop()
{
    unsigned long currentMillis = millis();

    // Update debounce state at regular intervals (polling method)
    if (currentMillis - timeDebounce >= INTERVAL_DEBOUNCE)
    {
        timeDebounce = currentMillis;
        button.update();                        // Update button state every 1ms
    }

    // Check for button press event
    if (button.isPressed())
    {
        stateLed = !stateLed;                   // Toggle LED state
        digitalWrite(PIN_LED, stateLed);        // Update physical LED

        Serial.print("Button pressed - LED is now: ");
        Serial.println(stateLed ? "ON" : "OFF");
    }

    // Heartbeat indicator (shows system is running)
    if (currentMillis - timeBlink >= INTERVAL_BLINK)
    {
        timeBlink = currentMillis;
        stateHeartbeat = !stateHeartbeat;
        digitalWrite(LED_BUILTIN, stateHeartbeat);
    }
}
