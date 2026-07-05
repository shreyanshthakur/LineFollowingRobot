#include<Arduino.h>

#define LEFT_SENSOR_PIN 18
#define RIGHT_SENSOR_PIN 19

#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26
#define ENA 27
#define ENB 14

const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8; // 0..255
const int PWM_CHANNEL_A = 0;
const int PWM_CHANNEL_B = 1;
const int MOTOR_SPEED = 150; // 0 = stop, 255 = full speed

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CHANNEL_A, MOTOR_SPEED);
  ledcWrite(PWM_CHANNEL_B, MOTOR_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CHANNEL_A, MOTOR_SPEED / 2);
  ledcWrite(PWM_CHANNEL_B, MOTOR_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(PWM_CHANNEL_A, MOTOR_SPEED);
  ledcWrite(PWM_CHANNEL_B, MOTOR_SPEED / 2);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CHANNEL_A, 0);
  ledcWrite(PWM_CHANNEL_B, 0);
}

void setup() {
    delay(2000);
  Serial.begin(115200);

  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(ENA, PWM_CHANNEL_A);

  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(ENB, PWM_CHANNEL_B);

  ledcWrite(PWM_CHANNEL_A, MOTOR_SPEED);
  ledcWrite(PWM_CHANNEL_B, MOTOR_SPEED);
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

