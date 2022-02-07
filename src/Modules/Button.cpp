#include "Button.hpp"

Button::Button(uint8_t a_buttonPin, uint32_t a_longPress, uint32_t a_debounce)
{
    m_buttonPin = a_buttonPin;
    m_longPress = a_longPress;
    m_debounce = a_debounce;
}

void Button::initializeButton()
{
    pinMode(m_buttonPin, INPUT_PULLUP);
    m_currentButtonReadState = ButtonIdle;
}

uint8_t Button::updateButton()
{
    switch (m_currentButtonReadState)
    {
    case ButtonReadingIdle:
        if (!digitalReadFast(m_buttonPin))
        {
            m_currentButtonReadState = ButtonDebouncing;
            m_pressedTime = 0;
        }
        break;
    case ButtonDebouncing:
        if (m_pressedTime >= m_debounce)
        {
            m_currentButtonReadState = ButtonWaiting;
        }

        break;
    case ButtonWaiting:
        if (digitalReadFast(m_buttonPin))
        {

            if (m_pressedTime > m_longPress)
            {
                m_currentButtonState = ButtonLongPress;
            }
            else
            {
                m_currentButtonState = ButtonShortPress;
            }

            m_currentButtonReadState = ButtonReset;
        }
        break;
    case ButtonReset:
        m_currentButtonState = ButtonIdle;
        m_currentButtonReadState = ButtonReadingIdle;
        break;
    default:
        break;
    }
    return m_currentButtonState;
}