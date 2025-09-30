// ****************************************************************************
// Title        : Simple Debounce Example (Interrupt)
// File Name    : 'DebounceSimpleInterrupt.ino'
// Target MCU   : Espressif ESP32 (Doit DevKit Version 1)
// Description  : Demonstrates button debouncing using hardware timer interrupt
//                for precise timing
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

// Globals
// ****************************************************************************
bool stateLed = false;                          // LED toggle state
bool stateHeartbeat = LOW;                      // Heartbeat LED state
unsigned long timeBlink = 0;                    // Last heartbeat time

// Hardware timer
hw_timer_t *timer = NULL;                       // Timer pointer

// Setup button debouncing
const bool logicLevel = HIGH;                   // Active HIGH logic
Debounce16 button(PIN_BUTTON, logicLevel);      // Instantiate debounce object

// Timer Interrupt Service Routine
// ****************************************************************************
void IRAM_ATTR onTimer()
{
    button.update();                            // Update at precise 1ms intervals
}

// Setup Code
// ****************************************************************************
void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Debounce16 Library - Simple Example (Interrupt)");
    Serial.println("================================================");
    Serial.println("- Press button to toggle LED");
    Serial.println("- Using hardware timer for precise timing");
    Serial.println("================================================");
    Serial.println();

    pinMode(PIN_LED, OUTPUT);                   // Configure LED as output
    digitalWrite(PIN_LED, LOW);                 // Initialize LED to OFF

    pinMode(LED_BUILTIN, OUTPUT);               // Configure built-in LED

    // Configure timer interrupt
    timer = timerBegin(0, 80, true);           // Timer 0, prescaler 80 (1MHz)
    timerAttachInterrupt(timer, &onTimer, true); // Attach ISR
    timerAlarmWrite(timer, 1000, true);        // 1000µs = 1ms, auto-reload
    timerAlarmEnable(timer);                   // Enable timer

    Serial.println("Timer interrupt configured for 1ms updates");
}

// Main Program
// ****************************************************************************
void loop()
{
    unsigned long currentMillis = millis();

    // No debounce update needed here - handled by ISR

    // Check for button press event
    if (button.isPressed())
    {
        stateLed = !stateLed;                   // Toggle LED state
        digitalWrite(PIN_LED, stateLed);        // Update physical LED

        Serial.print("Button pressed - LED is now: ");
        Serial.println(stateLed ? "ON" : "OFF");
    }

    // Heartbeat indicator
    if (currentMillis - timeBlink >= INTERVAL_BLINK)
    {
        timeBlink = currentMillis;
        stateHeartbeat = !stateHeartbeat;
        digitalWrite(LED_BUILTIN, stateHeartbeat);
    }
}
