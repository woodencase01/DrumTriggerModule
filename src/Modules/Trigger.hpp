#pragma once

#include <Arduino.h>

enum TriggerType
{
    Piezo,
    Switch,
    Sustain,
    HiHat,
};

enum TriggerState
{
    Idle,
    Rising,
    Hit,
    Delay,
};

class Trigger
{
private:
    uint8_t m_triggerState;
    uint8_t m_midiNote;
    uint8_t m_midiChannel;
    uint8_t m_gain;
    uint8_t m_velocityCurve;
    uint8_t m_threshold;
    uint8_t m_crossTalk;
    uint8_t m_retrigger; // Delais in ms before the trigger becomes available for a new hit
    uint8_t m_triggerType;

    uint16_t m_rawValue;
    uint16_t m_lastRawValue;
    uint16_t m_maxRawValue;

    uint8_t m_triggerRisingCount;
    const uint8_t m_triggerRisingCountMax = 5;

    uint32_t m_timeToReturnIdling;

public:
    Trigger();

    void manageTrigger();
    void readTrigger(uint16_t a_rawValue);
    void registerHit();
};