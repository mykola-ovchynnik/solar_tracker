#include <Arduino.h>
#include <LibPrintf.h>
#include <util/atomic.h>

#ifndef BUTTONS_H
#define BUTTONS_H

const int PIN_MODE_BTN = 12; // Mode button (manual or auto)
const int PIN_AXIS_BTN = 11; // Button to switch axes in manual mode

extern volatile bool auto_mode;
extern volatile bool axis_mode; // false is for left and right / true is for up and down

void readModeBtn();
void readAxisBtn();

#endif