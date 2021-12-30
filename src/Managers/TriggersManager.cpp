#include <Arduino.h>
#include "TriggersManager.hpp"
#include <Modules/Trigger.hpp>

// Analog input and MUX
const u_int8_t MUX_BITS = 4;
const uint8_t selectPins[MUX_BITS] = {2, 3, 4, 5};
const uint8_t analogInputs[] = {A0, A1};

//Triggers
Trigger triggers[32];

void selectMuxPin(uint8_t a_pin)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        if (a_pin & (1 << i))
            digitalWrite(selectPins[i], HIGH);
        else
            digitalWrite(selectPins[i], LOW);
    }
}

void initializeTriggers()
{
    // MUX select and analog
    for (uint8_t i = 0; i < MUX_BITS; i++)
    {
        pinMode(selectPins[i], OUTPUT);
    }
    for (uint8_t i = 0; i < sizeof(analogInputs); i++)
    {
        pinMode(analogInputs[i], INPUT);
    }
}