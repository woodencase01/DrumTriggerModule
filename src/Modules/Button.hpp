#pragma once

#include <Arduino.h>

enum ButtonOutputState
{
    ButtonIdle,
    ButtonShortPress,
    ButtonLongPress
};

enum ButtonReadingState
{
    ButtonReadingIdle,
    ButtonDebouncing,
    ButtonWaiting,
    ButtonReset
};

class Button
{
private:
    uint8_t m_buttonPin;
    uint32_t m_longPress;
    uint32_t m_debounce;

    uint8_t m_currentButtonState;

    elapsedMillis m_pressedTime;
    uint8_t m_currentButtonReadState;

public:
    Button(uint8_t a_buttonPin, uint32_t a_longPress, uint32_t a_debounce);
    void initializeButton();
    uint8_t updateButton();
};