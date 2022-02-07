#include "Trigger.hpp"

// Trigger constructor
Trigger::Trigger()
{
    m_maxRawValue = 0;
    m_triggerRisingCount = 0;
}

void Trigger::readTrigger(uint16_t a_rawValue)
{
    m_rawValue = a_rawValue;
}

void Trigger::registerHit()
{
    m_triggerState = Delay;
    m_maxRawValue = 0;
    m_triggerRisingCount = 0;
    m_timeToReturnIdling = 0;
}

void Trigger::setTriggerMidiChannel(uint8_t a_midiChannel) 
{
    m_midiChannel = a_midiChannel;
}

void Trigger::setTriggerMidiNote(uint8_t a_midiNote) 
{
    m_midiNote = a_midiNote;
}

uint8_t Trigger::getTriggerNote() 
{
    return m_midiNote;
}

uint8_t Trigger::getTriggerVelocity() 
{
    return m_midiVelocity;
}

uint8_t Trigger::getTriggerChannel() 
{
    return m_midiChannel;
}

bool Trigger::getLCDState() 
{
    if(m_LCDStateTime > LCD_STATE_DURATION)
    {
        m_LCDState = false;
    }
    return m_LCDState;
}

bool Trigger::manageTrigger()
{
    switch (m_triggerState)
    {
    case Idle:
        if (m_rawValue > m_threshold)
        {
            m_triggerState = Rising;
        }
        return false;
        break;

    case Rising:
        if (m_rawValue > m_maxRawValue)
        {
            m_maxRawValue = m_rawValue;
            m_triggerRisingCount = 0;
        }
        else
        {
            m_triggerRisingCount++;
        }

        if (m_triggerRisingCount >= m_triggerRisingCountMax)
        {
            m_triggerState = Hit;
            
            // Prepare the LCD state
            m_LCDState = true;
            m_LCDStateTime = 0;
        }
        return false;
        break;
    case Hit:
        return true;
        break;
    case Delay:
        if (m_timeToReturnIdling >= m_retrigger)
        {
            m_triggerState = Idle;
        }
        return false;
        break;
    default:
        return false;
        break;
    }

    switch (m_triggerType)
    {
    case Piezo:
        /* code */
        break;

    case Switch:
        /* code */
        break;

    case Sustain:
        /* code */
        break;

    case HiHat:
        /* code */
        break;

    default:
        break;
    }
}