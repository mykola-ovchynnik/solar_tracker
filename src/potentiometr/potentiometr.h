#include <Arduino.h>
#include <LibPrintf.h>

#ifndef POTENTIOMETR_H
#define POTENTIOMETR_H

const int SERVO_MAX_ANGLE = 180;
const int ANALOG_MAX_VALUE = 1023;
const int PIN_POTENTIOMETR = A4; // Pin for potentiometr

int readPotentiometr();

#endif