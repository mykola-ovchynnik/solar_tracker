#include <potentiometr/potentiometr.h>

int readPotentiometr()
{
    // Read potentiometer value
    int potentiometr = map(analogRead(PIN_POTENTIOMETR), 0, ANALOG_MAX_VALUE, 0, SERVO_MAX_ANGLE);

    printf("Potentiometr: %d\n", potentiometr);

    return potentiometr;
}
