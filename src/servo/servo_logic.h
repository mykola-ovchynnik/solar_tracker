#include <Servo.h>
#include <LibPrintf.h>
#include <util/atomic.h>

#include <buttons/buttons.h>
#include <potentiometr/potentiometr.h>
#include <sensors/sensor_logic.h>

#ifndef SERVO_LOGIC_H
#define SERVO_LOGIC_H

const int PIN_SERVO_UPDOWN = 5;
const int PIN_SERVO_LEFTRIGHT = 6;

extern Servo servo_updown;    // Declaration of up-down servo object
extern Servo servo_leftright; // Declaration of left-right servo object

void resetServos();

void rotate_manual();
void rotate_auto();

void rotate_UpDown();
void rotate_LeftRight();

char shouldRotateUpDown();
char shouldRotateLeftRight();

#endif