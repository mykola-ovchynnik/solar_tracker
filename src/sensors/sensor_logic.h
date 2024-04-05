#include <Arduino.h>
#include <utils/utils.h>

#ifndef SENSOR_LOGIC_H
#define SENSOR_LOGIC_H

// LDR Pin Definitions
const int PIN_LDR_TR = A0;
const int PIN_LDR_TL = A1;
const int PIN_LDR_BR = A2;
const int PIN_LDR_BL = A3;

const int SENSOR_ERROR_THRESHOLD = 0; // to adjust according to the minimum sensor reading (for example 0 - if no light)

void readAllLdrSensors();
bool isSunlightDetected();

int getLeftRightSensorDiff();
int getUpDownSensorDiff();

#endif
