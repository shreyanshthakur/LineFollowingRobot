#include <Arduino.h>

const int LED_PIN = 2;

unsigned long lastToggle = 0;
bool ledState = false;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    if (millis() - lastToggle >= 1000) {
        lastToggle = millis();

        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}