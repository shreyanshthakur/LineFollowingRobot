#include<Arduino.h>

#define LEFT_SENSOR_PIN 18
#define RIGHT_SENSOR_PIN 19

#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  int leftValue = digitalRead(LEFT_SENSOR_PIN);
  int rightValue = digitalRead(RIGHT_SENSOR_PIN);

  if (leftValue == LOW && rightValue == LOW) {
    moveForward();
  } else if (leftValue == LOW) {
    turnLeft();
  } else if (rightValue == LOW) {
    turnRight();
  } else {
    stopMoving();
  }

  delay(100);
}

