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
    m_timeToReturnIdling = millis() + m_retrigger;
}

void Trigger::manageTrigger()
{
    switch (m_triggerState)
    {
    case Idle:
        if (m_rawValue > m_threshold)
        {
            m_triggerState = Rising;
        }
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
        }

        break;
    case Hit:
        // Managed externally
        break;
    case Delay:
        if (millis() >= m_timeToReturnIdling)
        {
            m_triggerState = Idle;
        }

        break;
    default:
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