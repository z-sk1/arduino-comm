#include <Servo.h>
#include <LedControl.h>

#define BLUE_LED_PIN 11
#define GREEN_LED_PIN 10
#define RED_LED_PIN 9
#define YELLOW_LED_PIN 13
#define BUZZ_PIN 12

bool rgbShowActive = false;
bool melodyActive = false;
bool sirenActive = false;
bool megalovaniaActive = false;
bool portalMainThemeActive = false;
bool apertureThemeActive = false;
bool overworldThemeActive = false;
bool undergroundThemeActive = false;
bool servoSpinActive = false;
bool servoJoystickActive = false;
bool lightJoystickActive = false;
bool allLightsOnToggle = false;
bool buzzerJoystickActive = false;
bool ledMatrixLoopActive = false;
bool ledMatrixSmileyActive = false;
bool ledMatrixRandomActive = false;
bool ledMatrixJoystickActive = false;

const int num_servo = 2;

Servo servo[num_servo];
int servoPins[num_servo] = {6, 7};

int servoPos;
int joyX = A0;
int joyY = A1;
int joyBtn = 5;

// pins: DIN, CLK, CS
LedControl lc = LedControl(2, 4, 3, 1);

unsigned long lastMoveTime = 0;
int servoStep = 5;  // how many degrees to move each step
int servoDir = 1;

unsigned long lastBuzzTime = 0;

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

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  pinMode(BUZZ_PIN, OUTPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  randomSeed(analogRead(0));

  Serial.println("Arduino ready!");
}

void loop() {
  // check if any data is available

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    String arg;
    cmd.trim();

    int spaceIndex = cmd.indexOf(' ');
    if (spaceIndex > 0) {
      arg = cmd.substring(spaceIndex + 1);
      cmd = cmd.substring(0, spaceIndex);
    }

    if (cmd == "turnBlueLedOn") {
      digitalWrite(BLUE_LED_PIN, HIGH);
      Serial.println("BLUE LED is on");

    } else if (cmd == "turnBlueLedOff") {
      digitalWrite(BLUE_LED_PIN, LOW);
      Serial.println("BLUE LED is off");

    } else if (cmd == "turnGreenLedOn") {
      digitalWrite(GREEN_LED_PIN, HIGH);
      Serial.println("GREEN LED is on");

    } else if (cmd == "turnGreenLedOff") {
      digitalWrite(GREEN_LED_PIN, LOW);
      Serial.println("GREEN LED is off");

    } else if (cmd == "turnRedLedOn") { 
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("RED LED is on");

    } else if (cmd == "turnRedLedOff") { 
      digitalWrite(RED_LED_PIN, LOW);
      Serial.println("RED LED is off");

    } else if (cmd == "turnYellowLedOn") {
      digitalWrite(YELLOW_LED_PIN, HIGH);
      Serial.println("YELLOW LED is on");

    } else if (cmd == "turnYellowLedOff") {
      digitalWrite(YELLOW_LED_PIN, LOW);
      Serial.println("YELLOW LED is off");

    } else if (cmd == "turnBuzzOn") {
      tone(BUZZ_PIN, 1000);
      Serial.println("Buzz is on");

    } else if (cmd == "turnBuzzOff") {
      noTone(BUZZ_PIN);
      Serial.println("Buzz is on");

    } else if (cmd == "rgbShowOn") {
      rgbShowActive = true;
      Serial.println("RGB SHOW is on");

    } else if (cmd == "rgbShowOff") {
      rgbShowActive = false;
      Serial.println("RGB SHOW is off");
      turnAllLEDsOff();

    } else if (cmd == "melodyOn") {
      melodyActive = true;
      Serial.println("Melody is on");

    } else if (cmd == "melodyOff") {
      melodyActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Melody is off");

    } else if (cmd == "sirenOn") {
      sirenActive = true;
      Serial.println("Siren is on");

    } else if (cmd == "sirenOff") {
      sirenActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Siren is off");

    } else if (cmd == "megalovaniaOn") {
      megalovaniaActive = true;
      Serial.println("Megalovania is on");

    } else if (cmd == "megalovaniaOff") {
      megalovaniaActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Megalovania is off");

    } else if (cmd == "portalMainThemeOn") {
      portalMainThemeActive = true;
      Serial.println("Portal Main Theme on");

    } else if (cmd == "portalMainThemeOff") {
      portalMainThemeActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Portal Main Theme off");

    } else if (cmd == "apertureThemeOn") {
      apertureThemeActive = true;
      Serial.println("Aperture Theme is on");

    } else if (cmd == "apertureThemeOff") {
      apertureThemeActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Aperture Theme is off");

    } else if (cmd == "overworldThemeOn") {
      overworldThemeActive = true;
      Serial.println("Overworld theme is on");

    } else if (cmd == "overworldThemeOff") {
      overworldThemeActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Overworld theme is off");

    } else if (cmd == "undergroundThemeOn") {
      undergroundThemeActive = true;
      Serial.println("Underground theme is on");

    } else if (cmd == "undergroundThemeOff") {
      undergroundThemeActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Underground theme is off");

    } else if (cmd == "rotateServo90") {
      servoPos += 90;

      if (servoPos >= 180) servoPos = 180;

      for (int i = 0; i < num_servo; i++) {
        servo[i].write(servoPos);
      }

      Serial.println("Rotated the servo 90 deg");

    } else if (cmd == "rotateServo-90") {
      servoPos -= 90;

      if (servoPos <= 0) servoPos = 0;

      for (int i = 0; i < num_servo; i++) {
        servo[i].write(servoPos);
      }

      Serial.println("Roated the servo negative 90 deg");

    } else if (cmd == "servoSpinOn") {
      servoSpinActive = true;
      Serial.println("Servo spin is on");

    } else if (cmd == "servoSpinOff") {
      servoSpinActive = false;
      servoPos = 90;

      for (int i = 0; i < num_servo; i++) {
        servo[i].write(servoPos);
      }

      Serial.println("Servo spin is off");

    } else if (cmd == "rotatePrecise") {
      int deg = arg.toInt();
      servoPos = deg;
      
      for (int i = 0; i < num_servo; i++) {
        servo[i].write(servoPos);
      }

      Serial.println("Rotated precisely at: " + arg + " deg");

    } else if (cmd == "servoJoyControlOn") {
      servoJoystickActive = true;
      Serial.println("Rotating with joystick is on");

    } else if (cmd == "servoJoyControlOff") {
      servoJoystickActive = false;

      servoPos = 90;
      for (int i = 0; i < num_servo; i++) {
        servo[i].write(servoPos);
      }

      Serial.println("Rotating with joystick is off");

    } else if (cmd == "lightJoyControlOn") {
      lightJoystickActive = true;
      Serial.println("Controlling lights with joystick is on");

    } else if (cmd == "lightJoyControlOff") {
      lightJoystickActive = false;
      turnAllLEDsOff();

    } else if (cmd == "buzzerJoyControlOn") {
      buzzerJoystickActive = true;
      Serial.println("Controlling buzzer with joystick is on");

    } else if (cmd == "buzzerJoyControlOff") {
      buzzerJoystickActive = false;
      noTone(BUZZ_PIN);
      Serial.println("Controlling buzzer with joystick is off");

    } else if (cmd == "buzzPrecise") {
      int freq = arg.toInt();
      tone(BUZZ_PIN, freq);
      Serial.println("Buzzed precisely at: " + arg + " hz");

    } else if (cmd == "buzzPreciseOff") {
      noTone(BUZZ_PIN);
      Serial.println("Stopped buzzing precisely");

    } else if (cmd == "ledMatrixOn") {
      ledMatrixLoopActive = true;
      Serial.println("LED Matrix is on");

    } else if (cmd == "ledMatrixOff") {
      ledMatrixLoopActive = false;
      Serial.println("LED Matrix is off");

    } else if (cmd == "ledMatrixSmileyOn") {
      ledMatrixSmileyActive = true;
      Serial.println("LED Matrix Smiley is on");

    } else if (cmd == "ledMatrixSmileyOff") {
      ledMatrixSmileyActive = false;
      lc.clearDisplay(0);
      Serial.println("LED Matrix Smiley is off");

    } else if (cmd == "ledMatrixCountdown") {
      Serial.println("Counting down on LED Matrix");

      byte numbers[10][8] = {
        {B00111100, B01100110, B01101110, B01110110, B01100110, B01100110, B00111100, B00000000}, // 0
        {B00011000, B00111000, B00011000, B00011000, B00011000, B00011000, B00111100, B00000000}, // 1
        {B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110, B00000000}, // 2
        {B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100, B00000000}, // 3
        {B00001100, B00011100, B00101100, B01001100, B01111110, B00001100, B00011110, B00000000}, // 4
        {B01111110, B01100000, B01111100, B00000110, B00000110, B01100110, B00111100, B00000000}, // 5
        {B00111100, B01100110, B01100000, B01111100, B01100110, B01100110, B00111100, B00000000}, // 6
        {B01111110, B01100110, B00000110, B00001100, B00011000, B00011000, B00011000, B00000000}, // 7
        {B00111100, B01100110, B01100110, B00111100, B01100110, B01100110, B00111100, B00000000}, // 8
        {B00111100, B01100110, B01100110, B00111110, B00000110, B01100110, B00111100, B00000000}  // 9
      };

      for (int i = 9; i >= 0; i--) {
        for (int row = 0; row < 8; row++) {
          lc.setRow(0, row, numbers[i][row]);
        }
        delay(1000);
      }
      lc.clearDisplay(0);

    } else if (cmd == "ledMatrixRandomOn") {
      ledMatrixRandomActive = true;
      Serial.println("LED Matrix Random is on");

    } else if (cmd == "ledMatrixRandomOff") {
      ledMatrixRandomActive = false;
      lc.clearDisplay(0);
      Serial.println("LED Matrix Random is off");

    } else if (cmd == "ledMatrixJoyControlOn") {
      ledMatrixJoystickActive = true;
      Serial.println("LED Matrix Joystick control is on");

    } else if (cmd == "ledMatrixJoyControlOff") {
      ledMatrixJoystickActive = false;
      lc.clearDisplay(0);
      Serial.println("LED Matrix Joystick control is off");

    } else {
      Serial.print("unknown command: ");
      Serial.println(cmd);
    }
  }

  if (rgbShowActive) {
    RGB(75);
  }

  if (melodyActive) {
    playMelody();
  }

  if (sirenActive) {
    siren();
  }

  if (megalovaniaActive) {
    megalovania();
  }

  if (portalMainThemeActive) {
    portalTheme();
  }

  if (apertureThemeActive) {
    apertureTheme();
  }

  if (overworldThemeActive) {
    marioOverworld();
  }
  
  if (undergroundThemeActive) {
    marioUnderground();
  }

  if (servoSpinActive) {
    servoSpinNonBlocking();
  }

  if (servoJoystickActive) {
    int button = digitalRead(joyBtn);

    if (button == LOW) {
      servoSpinNonBlocking();
    } else {
      long totalX = 0;
      long totalY = 0;
      const int samples = 5;
      for (int i = 0; i < samples; i++) {
        totalX += analogRead(joyX);
        totalY += analogRead(joyY);
      }
      int avgX = totalX / samples;
      int avgY = totalY / samples;

      int posX = map(avgX, 0, 1023, 0, 180);
      int posY = map(avgY, 0, 1023, 0, 180);

      servo[1].write(posX);
      servo[2].write(posY);
    }
  }

  if (lightJoystickActive) {
    int xVal = analogRead(joyX);
    int yVal = analogRead(joyY);
    int button = digitalRead(joyBtn);

    int deadzone = 200;

    turnAllLEDsOff();

    // right 
    if (xVal > 512 + deadzone) {
      digitalWrite(BLUE_LED_PIN, HIGH);
    } 

    // left
    else if (xVal < 512 - deadzone) {
      digitalWrite(YELLOW_LED_PIN, HIGH);
    }

    // up
    if (yVal > 512 + deadzone) {
      digitalWrite(RED_LED_PIN, HIGH);
    }

    // down
    else if (yVal < 512 - deadzone) {
      digitalWrite(GREEN_LED_PIN, HIGH);
    }

    if (xVal == 512 && yVal == 512 && !allLightsOnToggle) {
      turnAllLEDsOff();
    }

    if (button == LOW) {
      allLightsOnToggle = !allLightsOnToggle;

      if (allLightsOnToggle) {
        turnAllLEDsOn();
      } else {
        turnAllLEDsOff();
      }
    }
  }

  if (buzzerJoystickActive) {
    int xVal = analogRead(joyX);
    int yVal = analogRead(joyY);
    int button = digitalRead(joyBtn);

    int freq = map(yVal, 0, 1023, 100, 2000);

    int duration = map(xVal, 0, 1023, 20, 300);

    tone(BUZZ_PIN, freq + duration);
  }

  if (ledMatrixLoopActive) {
    lc.setRow(0, 8, B00000000);
    lc.setRow(0, 1, B11111111);
    delay(500);
    lc.setRow(0, 1, B00000000);
    lc.setRow(0, 2, B11111111);
    delay(500);
    lc.setRow(0, 2, B00000000);
    lc.setRow(0, 3, B11111111);
    delay(500);
    lc.setRow(0, 3, B00000000);
    lc.setRow(0, 4, B11111111);
    delay(500);
    lc.setRow(0, 4, B00000000);
    lc.setRow(0, 5, B11111111);
    delay(500);
    lc.setRow(0, 5, B00000000);
    lc.setRow(0, 6, B11111111);
    delay(500);
    lc.setRow(0, 6, B00000000);
    lc.setRow(0, 7, B11111111);
    delay(500);
    lc.setRow(0, 7, B00000000);
    lc.setRow(0, 8, B11111111);
  }

  if (ledMatrixSmileyActive) {
    byte smiley[8] = {
      B00111100, //   ****
      B01000010, //  *    *
      B10100101, // * *  * *
      B10000001, // *      *
      B10100101, // * *  * *
      B10011001, // *  **  *
      B01000010, //  *    *
      B00111100  //   ****
    };

    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool ledOn = bitRead(smiley[row], 7 - col);
        lc.setLed(0, row, col, ledOn);
      }
    }
  }

  if (ledMatrixRandomActive) {
    for (int row = 0; row < 8; row++) {
      byte randomByte = random(0, 256);
      lc.setRow(0, row, randomByte);
    }
    delay(75);
  }

  if (ledMatrixJoystickActive) {
    int xVal = analogRead(joyX);
    int yVal = analogRead(joyY);

    int levelX = map(xVal, 0, 1023, 0, 8);
    int levelY = map(yVal, 0, 1023, 0, 8);

    lc.clearDisplay(0);

    for (int col = 0; col < levelX; col++) {
      for (int row = 0; row < levelY; row++) {
        lc.setLed(0, row, col, true);
      }
    }
  }
}

void RGB(int delayTime) {

  digitalWrite(BLUE_LED_PIN, HIGH);
  delay(delayTime);

  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);

  delay(delayTime);

  digitalWrite(BLUE_LED_PIN, LOW);
}

void turnAllLEDsOff() {
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void turnAllLEDsOn() {
  digitalWrite(BLUE_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
}

void playMelody() {
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523}; // C D E F G A B C
  int noteDuration = 200;

  for (int i = 0; i < 8; i++) {
    tone(BUZZ_PIN, melody[i]);
    delay(noteDuration);
    noTone(BUZZ_PIN);
    delay(50);
  }
}

void megalovania() {
  int melody[] = {147, 147, 294, 220, 208, 196, 175, 147, 175, 196, 131, 131, 294, 220, 208, 196, 175, 147, 175, 196, 123, 123, 294, 220, 208, 196, 175, 147, 175, 196, 117, 117, 294, 220, 208, 196, 175, 147, 175, 196};
  int noteDuration[] = {300, 300, 400, 300, 250, 250, 300, 400, 300, 300, 300, 300, 400, 300, 250, 250, 300, 400, 300, 300, 300, 300, 400, 300, 250, 250, 300, 400, 300, 300, 300, 300, 400, 300, 250, 250, 300, 400, 300, 300};

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    tone(BUZZ_PIN, melody[i]);
    delay(noteDuration[i]);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

void portalTheme() {
  int melody[] = {175, 262, 196, 208, 175, 262, 196, 233, 175, 262, 196, 208, 233, 208, 196, 175, // 1st half
  0, // pause
  175, 262, 208, 233, 196, 208, 175, 165, 175, 262, 208, 233, 277, 196, 233, 165}; // 2nd half

  int noteDuration[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 
  300, // pause
  350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 400, 400, 400};

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    if (melody[i] > 0) tone(BUZZ_PIN, melody[i]);
    delay(noteDuration[i]);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

void apertureTheme() {
  int melody[] = {175, 233, 294, 349, 294, 233};
  int noteDuration = 500;

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    if (melody[i] > 0) tone(BUZZ_PIN, melody[i]);
    delay(noteDuration);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

void marioOverworld() {
  int melody[] = {
    2637, 2637, 0, 2637,
    0, 2093, 2637, 0,
    3136, 0, 0,  0,
    1568, 0, 0, 0,

    2093, 0, 0, 1568,
    0, 0, 1319, 0,
    0, 1760, 0, 1976,
    0, 1865, 1760, 0,

    1568, 2637, 3136,
    3520, 0, 2794, 3136,
    0, 2637, 0, 2093,
    2349, 1976, 0, 0
  };

  int noteDuration[] = {
    125, 125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125,

    125, 125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125,

    125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125,
    125, 125, 125, 125
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    if (melody[i] > 0) tone(BUZZ_PIN, melody[i]);
    delay(noteDuration[i]);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

void marioUnderground() {
  int melody[] = {
    262, 523, 220, 440,
    233, 466, 0,
    0, 262, 523, 220, 440,
    233, 466, 0,
    0, 175, 349, 147, 294,
    156, 311, 0,
    0, 175, 349, 147, 294,
    156, 311, 0, 0,
  };

  int noteDuration[] = {
    250, 250, 250, 250,
    250, 250, 250,
    125, 250, 250, 250, 250,
    250, 250, 250,
    125, 250, 250, 250, 250,
    250, 250, 250,
    125, 250, 250, 250, 250,
    250, 250, 250, 250,
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    if (melody[i] > 0) tone(BUZZ_PIN, melody[i]);
    delay(noteDuration[i]);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

void siren() {
  for (int freq = 400; freq <= 1000; freq += 10) {
    tone(BUZZ_PIN, freq);
    delay(5);
  }
  for (int freq = 1000; freq >= 400; freq -= 10) {
    tone(BUZZ_PIN, freq);
    delay(5);
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