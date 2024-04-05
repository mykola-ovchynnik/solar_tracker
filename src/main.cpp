#include <servo/servo_logic.h>

void setup()
{
  pinMode(PIN_MODE_BTN, INPUT);
  pinMode(PIN_AXIS_BTN, INPUT);
  pinMode(PIN_POTENTIOMETR, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_MODE_BTN), readModeBtn, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_AXIS_BTN), readAxisBtn, RISING);

  servo_updown.attach(PIN_SERVO_UPDOWN);
  servo_leftright.attach(PIN_SERVO_LEFTRIGHT);

  resetServos();

  Serial.begin(9600);
  delay(3000);
}

void loop()
{
  bool mode;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    mode = auto_mode;
  }

  // Execute selected mode (auto/manual)
  printf("Current mode: ");
  if (mode)
  {
    rotate_auto();
  }
  else
  {
    rotate_manual();
  }

  printf("--------------------\n");
}
