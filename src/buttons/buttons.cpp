#include <buttons/buttons.h>

volatile bool auto_mode = false;
volatile bool axis_mode = false;

unsigned long buttonReadInterval = 200;

void readModeBtn()
{
    static unsigned long buttonModeLastReadTime = 0;

    if (millis() - buttonModeLastReadTime >= buttonReadInterval)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            auto_mode = !auto_mode;
        }
        printf(auto_mode ? "Auto mode activated\n" : "Manual mode activated\n");

        buttonModeLastReadTime = millis();
    }
}

void readAxisBtn()
{
    static unsigned long buttonAxisLastReadTime = 0;

    if (millis() - buttonAxisLastReadTime >= buttonReadInterval)
    {

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            axis_mode = !axis_mode;
            printf(axis_mode ? "Up-Down axis selected\n" : "Left-Right axis selected\n");
        }

        buttonAxisLastReadTime = millis();
    }
}
