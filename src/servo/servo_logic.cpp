#include <servo/servo_logic.h>

Servo servo_updown;    // Definition of up-down servo object
Servo servo_leftright; // Definition of left-right servo object

int sensitivity = 10;
int servoIncrement = 1;
int servoInitialPosition = 90; // to be adjusted as needed for the reset

void resetServos()
{
    // Reset servo positions to initial values
    servo_updown.write(servoInitialPosition);    // Initial position for up-down servo
    servo_leftright.write(servoInitialPosition); // Initial position for left-right servo
}

void rotate_manual()
{
    printf("manual\n");

    bool axis;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        axis = axis_mode;
        printf("Axis: %s\n", axis ? "Up-Down" : "Left-Right");
    }

    // Control servo based on axis mode
    if (!axis)
    {
        servo_leftright.write(readPotentiometr());
    }
    else
    {
        servo_updown.write(readPotentiometr());
    }
}

void rotate_auto()
{
    printf("auto\n");

    readAllLdrSensors();

    if (!isSunlightDetected())
    {
        resetServos();
        printf("No sunlight detected, resetting servos to initial position.\n");
        return;
    }

    // Rotate servo based on sunlight direction
    rotate_UpDown();
    rotate_LeftRight();
}

void rotate_UpDown()
{
    char shouldRotateUD = shouldRotateUpDown();

    printf("Up-Down rotation: ");
    if (shouldRotateUD != 'N')
    {
        int newUDAngle = servo_updown.read() + (shouldRotateUD == 'U' ? servoIncrement : -servoIncrement);
        servo_updown.write(newUDAngle);

        printf("%s\n", shouldRotateUD == 'U' ? "rotating Up" : "rotating Down");
    }
    else
    {
        printf("Don't rotate. Position is optimal\n");
    }
}

void rotate_LeftRight()
{
    char shouldRotateLR = shouldRotateLeftRight();

    printf("Left-Right rotation: ");
    if (shouldRotateLR != 'N')
    {
        int newLRAngle = servo_leftright.read() + (shouldRotateLR == 'R' ? servoIncrement : -servoIncrement);
        servo_leftright.write(newLRAngle);

        printf("%s\n", shouldRotateLR == 'R' ? "rotating Right" : "rotating Left");
    }
    else
    {
        printf("Don't rotate. Position is optimal\n");
    }
}

char shouldRotateUpDown()
{
    // Calculate difference between top and bottom sensor readings

    int difference = getUpDownSensorDiff();

    printf("Servo Up Down position: %d\n", servo_updown.read());

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
    int difference = getLeftRightSensorDiff();

    printf("Servo Left Right position: %d\n", servo_leftright.read());

    if (abs(difference) < sensitivity)
    {
        return 'N'; // If difference is below sensitivity threshold, return 'N'
    }
    else
    {
        return (difference > 0) ? 'R' : 'L'; // Right or left rotation
    }
}
