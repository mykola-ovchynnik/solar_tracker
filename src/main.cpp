#include <Arduino.h>
#include <Servo.h>
#include <LibPrintf.h>

// Pin Definitions
const int PIN_SERVO_UPDOWN = 5;
const int PIN_SERVO_LEFTRIGHT = 6;
const int PIN_MODE_BTN = 12;     // Mode button (manual or auto)
const int PIN_AXIS_BTN = 11;     // Button to switch axes in manual mode
const int PIN_POTENTIOMETR = A4; // Pin for potentiometr

// LDR Pin Definitions
const int PIN_LDR_TR = A0;
const int PIN_LDR_TL = A1;
const int PIN_LDR_BR = A2;
const int PIN_LDR_BL = A3;

// Constants
const int SERVO_MAX_ANGLE = 180;
const int ANALOG_MAX_VALUE = 1023;
const int SERVO_INCREMENT = 1;
const int SERVO_INITIAL_POSITION = 90; // to be adjusted as needed for the reset
const int SENSOR_ERROR_THRESHOLD = 0;  // to adjust according to the minimum sensor reading (for example 0 - if no light)

int ldrStateTR, ldrStateTL, ldrStateBR, ldrStateBL;

Servo servo_updown;    // Servo object to rotate the platform up and down
Servo servo_leftright; // Servo object to rotate the platform left and right

int modeButtonPrev = 0;

int axisBtnPrev = 0;

bool auto_mode = false;
bool axis_mode = false; // false is for left and right / true is for up and down

int sensitivity = 10;
unsigned long previousTime = 0;
unsigned long interval = 100;

// Function prototypes
void readLdrSensors();
void rotate_manual();
void readAxisBtn();
void rotate_auto();
bool isSunlightDetected();
void resetServos();
char shouldRotateUpDown();
char shouldRotateLeftRight();
int calcAverage(int value1, int value2);
int calcDifference(int value1, int value2);
int readPotentiometr();
void readModeBtn();
void readAxisBtn();

void setup()
{
  pinMode(PIN_MODE_BTN, INPUT);
  pinMode(PIN_AXIS_BTN, INPUT);
  pinMode(PIN_POTENTIOMETR, INPUT);

  servo_updown.attach(PIN_SERVO_UPDOWN);
  servo_leftright.attach(PIN_SERVO_LEFTRIGHT);

  resetServos();

  Serial.begin(9600);
  delay(3000);
}

void loop()
{
  readModeBtn();

  // Execute appropriate mode (auto/manual)
  Serial.print("Mode: ");
  if (auto_mode)
  {
    rotate_auto();
  }
  else
  {
    rotate_manual();
  }

  Serial.println("--------------------");
}

void readModeBtn()
{
  int modeBtn = digitalRead(PIN_MODE_BTN);

  if (modeBtn != modeButtonPrev)
  {
    if (modeBtn == HIGH)
    {
      auto_mode = !auto_mode;
      Serial.println(auto_mode ? "Auto mode activated" : "Manual mode activated");
    }

    modeButtonPrev = modeBtn;
  }
}

void readLdrSensors()
{
  ldrStateTR = analogRead(PIN_LDR_TR);
  ldrStateTL = analogRead(PIN_LDR_TL);
  ldrStateBR = analogRead(PIN_LDR_BR);
  ldrStateBL = analogRead(PIN_LDR_BL);

  Serial.print("Sensor Top Right: ");
  Serial.println(ldrStateTR);
  Serial.print("Sensor Top Left: ");
  Serial.println(ldrStateTL);
  Serial.print("Sensor Bottom Right: ");
  Serial.println(ldrStateBR);
  Serial.print("Sensor Bottom Left: ");
  Serial.println(ldrStateBL);
  Serial.println("--------");
}

void rotate_manual()
{
  Serial.println("manual");
  readAxisBtn();

  // Read potentiometer value
  int potentiometr = readPotentiometr();

  Serial.print("Potentiometr: ");
  Serial.println(potentiometr);
  Serial.print("Axis: ");
  Serial.println(axis_mode ? "Up-Down" : "Left-Right");

  // Control servo based on axis mode
  if (!axis_mode)
  {
    servo_leftright.write(potentiometr);
  }
  else
  {
    servo_updown.write(potentiometr);
  }
}

void readAxisBtn()
{
  int axisBtn = digitalRead(PIN_AXIS_BTN);

  if (axisBtn != axisBtnPrev)
  {
    if (axisBtn == HIGH)
    {
      axis_mode = !axis_mode;
      Serial.println(axis_mode ? "Up-Down axis selected" : "Left-Right axis selected");
    }

    axisBtnPrev = axisBtn;
  }
}

int readPotentiometr()
{
  return map(analogRead(PIN_POTENTIOMETR), 0, ANALOG_MAX_VALUE, 0, SERVO_MAX_ANGLE);
}

void rotate_auto()
{
  Serial.println("auto");

  unsigned long currentTime = millis();
  if (currentTime - previousTime >= interval)
  {
    readLdrSensors();
  }

  if (!isSunlightDetected())
  {
    resetServos();
    Serial.println("No sunlight detected, resetting servos to initial position.");
    return;
  }

  // Calculate time difference since last servo movement
  if (currentTime - previousTime >= interval)
  {

    char shouldRotateUD = shouldRotateUpDown();
    char shouldRotateLR = shouldRotateLeftRight();
    Serial.println("--------");

    // Rotate servo based on sunlight direction
    Serial.print("Up-Down rotation: ");
    if (shouldRotateUD != 'N')
    {
      int newUDAngle = servo_updown.read() + (shouldRotateUD == 'U' ? SERVO_INCREMENT : -SERVO_INCREMENT);
      servo_updown.write(constrain(newUDAngle, 0, SERVO_MAX_ANGLE));

      Serial.println(shouldRotateUD == 'U' ? "rotating Up" : "rotating Down");
    }
    else
    {
      Serial.println("Don't rotate. Position is optimal");
    }

    Serial.print("Left-Right rotation: ");
    if (shouldRotateLR != 'N')
    {
      int newLRAngle = servo_leftright.read() + (shouldRotateLR == 'R' ? SERVO_INCREMENT : -SERVO_INCREMENT);
      servo_leftright.write(constrain(newLRAngle, 0, SERVO_MAX_ANGLE));

      Serial.println(shouldRotateLR == 'R' ? "rotating Right" : "rotating Left");
    }
    else
    {
      Serial.println("Don't rotate. Position is optimal");
    }

    previousTime = currentTime;
  }
}

bool isSunlightDetected()
{
  // Check if sensor readings indicate sunlight
  return (abs(ldrStateTR) > SENSOR_ERROR_THRESHOLD &&
          abs(ldrStateTL) > SENSOR_ERROR_THRESHOLD &&
          abs(ldrStateBR) > SENSOR_ERROR_THRESHOLD &&
          abs(ldrStateBL) > SENSOR_ERROR_THRESHOLD);
}

void resetServos()
{
  // Reset servo positions to initial values
  servo_updown.write(SERVO_INITIAL_POSITION);    // Initial position for up-down servo
  servo_leftright.write(SERVO_INITIAL_POSITION); // Initial position for left-right servo
}

char shouldRotateUpDown()
{
  // Calculate difference between top and bottom sensor readings
  int avgTop = calcAverage(ldrStateTR, ldrStateTL);
  int avgBottom = calcAverage(ldrStateBR, ldrStateBL);
  int difference = calcDifference(avgTop, avgBottom);

  Serial.print("Average top: ");
  Serial.println(avgTop);
  Serial.print("Average bottom: ");
  Serial.println(avgBottom);

  Serial.print("Servo Up Down position: ");
  Serial.println(servo_updown.read());

  if (abs(difference) < sensitivity)
  {
    return 'N'; // If difference is below sensitivity threshold, return 'N'
  }
  else
  {
    return (difference > 0) ? 'U' : 'D'; // Up or down rotation
  }
}

char shouldRotateLeftRight()
{
  int avgRight = calcAverage(ldrStateTR, ldrStateBR);
  int avgLeft = calcAverage(ldrStateTL, ldrStateBL);
  int difference = calcDifference(avgRight, avgLeft);

  Serial.print("Average Right: ");
  Serial.println(avgRight);
  Serial.print("Average left: ");
  Serial.println(avgLeft);

  Serial.print("Servo Left Right position: ");
  Serial.println(servo_leftright.read());

  if (abs(difference) < sensitivity)
  {
    return 'N'; // If difference is below sensitivity threshold, return 'N'
  }
  else
  {
    return (difference > 0) ? 'R' : 'L'; // Right or left rotation
  }
}

int calcAverage(int value1, int value2)
{
  return (value1 + value2) / 2;
}

int calcDifference(int value1, int value2)
{
  return value1 - value2;
}
