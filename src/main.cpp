#include <servo/servo_logic.h>

void setup()
{
  pinMode(PIN_MODE_BTN, INPUT);
  pinMode(PIN_AXIS_BTN, INPUT);
  pinMode(PIN_POTENTIOMETR, INPUT);

  // Enable pin change interrupt for PCINT2 (pins 8 to 13)
  PCICR |= (1 << PCIE2);

  // Enable interrupts for pins 11 (PCINT2) and 12 (PCINT3) within PCINT2
  PCMSK2 |= (1 << PCINT2) | (1 << PCINT3);

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
