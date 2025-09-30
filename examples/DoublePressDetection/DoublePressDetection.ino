// ****************************************************************************
// Title        : Double Press Detection Example (Polling)
// File Name    : 'DoublePressDetection.ino'
// Target MCU   : Espressif ESP32 (Doit DevKit Version 1)
// Description  : Demonstrates double-press detection with visual feedback
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
const uint8_t PIN_LED_SINGLE = 15;              // Single press LED
const uint8_t PIN_LED_DOUBLE = 16;              // Double press LED
const uint16_t INTERVAL_BLINK = 1000;           // Heartbeat interval
const uint16_t INTERVAL_DEBOUNCE = 1;           // Debounce update interval
const uint16_t WINDOW_DOUBLE_PRESS = 300;       // 300ms double-press window
const uint16_t DURATION_LED_FEEDBACK = 500;     // LED feedback duration

// Globals
// ****************************************************************************
bool stateHeartbeat = LOW;                      // Heartbeat LED state
unsigned long timeBlink = 0;                    // Last heartbeat time
unsigned long timeDebounce = 0;                 // Last debounce update time
unsigned long timeLedSingle = 0;                // Single press LED timer
unsigned long timeLedDouble = 0;                // Double press LED timer
bool flagLedSingleActive = false;               // Single LED active flag
bool flagLedDoubleActive = false;               // Double LED active flag

// Setup button debouncing
const bool logicLevel = HIGH;                   // Active HIGH logic
Debounce16 button(PIN_BUTTON, logicLevel);      // Instantiate debounce object

// Setup Code
// ****************************************************************************
void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Debounce16 Library - Double Press Detection");
    Serial.println("============================================");
    Serial.println("- Single press = LED 1 (green)");
    Serial.println("- Double press = LED 2 (red)");
    Serial.println("- Double-press window: 300ms");
    Serial.println("============================================");
    Serial.println();

    pinMode(PIN_LED_SINGLE, OUTPUT);            // Configure single press LED
    pinMode(PIN_LED_DOUBLE, OUTPUT);            // Configure double press LED
    pinMode(LED_BUILTIN, OUTPUT);               // Configure heartbeat LED

    digitalWrite(PIN_LED_SINGLE, LOW);          // Initialize LEDs OFF
    digitalWrite(PIN_LED_DOUBLE, LOW);

    // Enable and configure double-press detection
    button.enableDoublePressDetection(true);
    button.setDoublePressWindow(WINDOW_DOUBLE_PRESS);

    Serial.println("Double-press detection enabled");
}

// Main Program
// ****************************************************************************
void loop()
{
    unsigned long currentMillis = millis();

    // Update debounce state
    if (currentMillis - timeDebounce >= INTERVAL_DEBOUNCE)
    {
        timeDebounce = currentMillis;
        button.update();                        // Update button state
    }

    // Check for double-press event
    if (button.isDoublePressed())
    {
        digitalWrite(PIN_LED_DOUBLE, HIGH);     // Turn on double-press LED
        timeLedDouble = currentMillis;          // Start LED timer
        flagLedDoubleActive = true;

        Serial.println(">>> DOUBLE PRESS detected! <<<");
    }

    // Check for single press (after double-press window expires)
    uint8_t clickCount = button.getClickCount();
    if (clickCount == 1)
    {
        digitalWrite(PIN_LED_SINGLE, HIGH);     // Turn on single-press LED
        timeLedSingle = currentMillis;          // Start LED timer
        flagLedSingleActive = true;

        Serial.println("Single press detected");
    }

    // Turn off single-press LED after feedback duration
    if (flagLedSingleActive &&
        (currentMillis - timeLedSingle >= DURATION_LED_FEEDBACK))
    {
        digitalWrite(PIN_LED_SINGLE, LOW);
        flagLedSingleActive = false;
    }

    // Turn off double-press LED after feedback duration
    if (flagLedDoubleActive &&
        (currentMillis - timeLedDouble >= DURATION_LED_FEEDBACK))
    {
        digitalWrite(PIN_LED_DOUBLE, LOW);
        flagLedDoubleActive = false;
    }

    // Heartbeat indicator
    if (currentMillis - timeBlink >= INTERVAL_BLINK)
    {
        timeBlink = currentMillis;
        stateHeartbeat = !stateHeartbeat;
        digitalWrite(LED_BUILTIN, stateHeartbeat);
    }
}
