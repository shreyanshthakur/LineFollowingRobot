#include <Arduino.h>

#define LEFT_SENSOR_PIN 18
#define RIGHT_SENSOR_PIN 19

void setup() {
    Serial.begin(115200);
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(RIGHT_SENSOR_PIN, INPUT);
}

void loop() {
    int leftValue = digitalRead(LEFT_SENSOR_PIN);
    int rightValue = digitalRead(RIGHT_SENSOR_PIN);

    Serial.print("L: ");
    Serial.print(leftValue);
    Serial.print(" | R: ");
    Serial.println(rightValue);

    if (leftValue == LOW && rightValue == LOW) {
        Serial.println("Both on line");
    } else if (leftValue == LOW) {
        Serial.println("Left on line");
    } else if (rightValue == LOW) {
        Serial.println("Right on line");
    } else {
        Serial.println("Both off line");
    }

    delay(200);
}