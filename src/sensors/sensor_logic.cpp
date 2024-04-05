#include "sensor_logic.h"

int ldrStateTR, ldrStateTL, ldrStateBR, ldrStateBL;

unsigned long sensorReadInterval = 100;

// Sensors logic
void readAllLdrSensors()
{
    static unsigned long sensorLastReadTime = 0;

    if (millis() - sensorLastReadTime >= sensorReadInterval)
    {
        printf("Sensors reading: \n");

        ldrStateTR = analogRead(PIN_LDR_TR);
        printf("LDR Top Right: %d\n", ldrStateTR);

        ldrStateTL = analogRead(PIN_LDR_TL);
        printf("LDR Top Left: %d\n", ldrStateTL);

        ldrStateBR = analogRead(PIN_LDR_BR);
        printf("LDR Bottom Right: %d\n", ldrStateBR);

        ldrStateBL = analogRead(PIN_LDR_BL);
        printf("LDR Bottom Left: %d\n", ldrStateBL);

        sensorLastReadTime = millis();
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

int getLeftRightSensorDiff()
{
    int avgRight = calcAverage(ldrStateTR, ldrStateBR);
    int avgLeft = calcAverage(ldrStateTL, ldrStateBL);

    printf("Average Right: %d\n", avgRight);
    printf("Average left: %d\n", avgLeft);

    return calcDifference(avgRight, avgLeft);
};

int getUpDownSensorDiff()
{
    int avgTop = calcAverage(ldrStateTR, ldrStateTL);
    int avgBottom = calcAverage(ldrStateBR, ldrStateBL);

    printf("Average top: %d\n", avgTop);
    printf("Average bottom: %d\n", avgBottom);

    return calcDifference(avgTop, avgBottom);
}