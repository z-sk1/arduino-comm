#include <Servo.h>

const int yellowPin = 11;
const int greenPin = 10;

const int num_servo = 1;

Servo servo[num_servo];
int servoPins[num_servo] = {9};

int servoPos;
const int joyX = A0;
const int joyY = A1;
const int joyBtn = 5;

unsigned long lastMoveTime = 0;
int servoStep = 5;  // how many degrees to move each step
int servoDir = 1;

const int btnPin = 8;
int btnState = 0;
bool lastStableBtn = HIGH;
bool lastReading = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; 

bool servoJoystickActive = true;

void setup() {
  Serial.begin(9600);

  servoPos = 90;
  
  for (int i = 0; i < num_servo; i++) {
    servo[i].attach(servoPins[i]);
    servo[i].write(servoPos);
  }

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyBtn, INPUT_PULLUP);

  pinMode(btnPin, INPUT_PULLUP);

  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, HIGH);
}

void loop() {
  int reading = digitalRead(btnPin);

  // If reading changed, reset debounce timer
  if (reading != lastReading) {
      lastDebounceTime = millis();
  }
  lastReading = reading;

  // Check if stable for 50ms
  if ((millis() - lastDebounceTime) > debounceDelay) {

      // Has the stable state changed?
      if (reading != lastStableBtn) {
          lastStableBtn = reading;

          // Only toggle when button becomes PRESSED (LOW)
          if (reading == LOW) {
              servoJoystickActive = !servoJoystickActive;
          }
      }
  }

  if (servoJoystickActive) {
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
  } else {
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  }

  if (servoJoystickActive) {
    int button = digitalRead(joyBtn);

    if (button == LOW) {
      servoSpinNonBlocking();
    } else {
      long totalX = 0;
      long totalY = 0;

      const int samples = 5;
      const int center = 512;
      const int deadzone = 200;
      for (int i = 0; i < samples; i++) {
        totalX += analogRead(joyX);
        totalY += analogRead(joyY);
      }

      int avgX = totalX / samples;
      int avgY = totalY / samples;
      if (abs(avgY - center) < deadzone) avgY = center;

      int posX = map(avgX, 0, 1023, 0, 180);
      int posY = map(avgY, 0, 1023, 0, 180);

      servo[0].write(posX);
    }
  } else {
    servoSpinNonBlocking();
  }

}

void servoSpinNonBlocking() {
    unsigned long currentTime = millis();
    if (currentTime - lastMoveTime >= 25) { // move every 50ms
        lastMoveTime = currentTime;

        servoPos += servoStep * servoDir;

        if (servoPos >= 180) servoDir = -1; // reverse
        if (servoPos <= 0)   servoDir = 1;  // reverse

        for (int i = 0; i < num_servo; i++) {
          servo[i].write(servoPos);
        }
    }
}