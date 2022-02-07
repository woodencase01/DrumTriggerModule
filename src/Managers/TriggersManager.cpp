#include <Arduino.h>
#include <ADC.h>
#include <ADC_util.h>
#include "TriggersManager.hpp"
#include <Modules/Trigger.hpp>
#include "triggerModule_Defs.hpp"

uint8_t currentChannel = 0;

// Analog input and MUX
const uint8_t MUX_BITS = 4;
const uint8_t SELECT_PINS[MUX_BITS] = {10, 9, 8, 11};
const uint8_t ANALOG_INPUTS[] = {A0, A1};
ADC *adc = new ADC();
ADC::Sync_result result;
uint16_t tempRawADC[NUMBER_TRIGGERS];

//Triggers
Trigger triggers[NUMBER_TRIGGERS];
bool forceTriggerUpdate = false;
const uint8_t DEFAULT_MIDI_CHANNEL = 10;
enum DrumNotes
{
    NoDrumNote = 0,
    Laser = 27,
    Whip = 28,
    ScratchPush = 29,
    ScratchPull = 30,
    StickClick = 31,
    MetronomeClick = 32,
    MetronomeBell = 34,
    BassDrum = 35,
    KickDrum = 36,
    SnareCrossStick = 37,
    SnareDrum = 38,
    HandClap = 39,
    ElectricSnareDrum = 40,
    FloorTom2 = 41,
    HiHatClosed = 42,
    FloorTom1 = 43,
    HiHatFoot = 44,
    LowTom = 45,
    HiHatOpen = 46,
    LowMidTom = 47,
    HighMidTom = 48,
    CrashCymbal = 49,
    HighTom = 50,
    RideCymbal = 51,
    ChinaCymbal = 52,
    RideBell = 53,
    Tambourine = 54,
    Splashcymbal = 55,
    Cowbell = 56,
    CrashCymbal2 = 57,
    Vibraslap = 58,
    RideCymbal2 = 59,
    HighBongo = 60,
    LowBongo = 61,
    CongaDeadStroke = 62,
    Conga = 63,
    Tumba = 64,
    HighTimbale = 65,
    LowTimbale = 66,
    HighAgogo = 67,
    LowAgogo = 68,
    Cabasa = 69,
    Maracas = 70,
    WhistleShort = 71,
    WhistleLong = 72,
    GuiroShort = 73,
    GuiroLong = 74,
    Claves = 75,
    HighWoodblock = 76,
    LowWoodblock = 77,
    CuicaHigh = 78,
    CuicaLow = 79,
    TriangleMute = 80,
    TriangleOpen = 81,
    Shaker = 82,
    SleighBell = 83,
    BellTree = 84,
    Castanets = 85,
    SurduDeadStroke = 86,
    Surdu = 87,
    SnareDrumRod = 91,
    OceanDrum = 92,
    SnareDrumBrush = 93
};

const uint8_t DEFAULT_MIDI_NOTES[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

void selectMuxPin(uint8_t a_pin)
{
    for (uint8_t i = 0; i < MUX_BITS; i++)
    {
        if (a_pin & (1 << i))
            digitalWrite(SELECT_PINS[i], HIGH);
        else
            digitalWrite(SELECT_PINS[i], LOW);
    }
}

void initializeTriggers()
{
    // MUX select and analog
    for (uint8_t i = 0; i < MUX_BITS; i++)
    {
        pinMode(SELECT_PINS[i], OUTPUT);
    }
    for (uint8_t i = 0; i < sizeof(ANALOG_INPUTS); i++)
    {
        pinMode(ANALOG_INPUTS[i], INPUT);
    }

    adc->adc0->setAveraging(4);
    adc->adc1->setAveraging(4);
    adc->adc0->setResolution(10);
    adc->adc1->setResolution(10);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

    selectMuxPin(0);
    adc->startSynchronizedContinuous(ANALOG_INPUTS[0], ANALOG_INPUTS[1]);
    adc->adc0->enableInterrupts(adc0_isr);

    for (uint8_t i = 0; i < NUMBER_TRIGGERS; i++)
    {
        triggers[i].setTriggerMidiChannel(DEFAULT_MIDI_CHANNEL);
        triggers[i].setTriggerMidiNote(DEFAULT_MIDI_NOTES[i]);
    }
    
}

void updateTriggers()
{
    if (forceTriggerUpdate) // All channels have been read
    {
        for (uint8_t i = 0; i < NUMBER_TRIGGERS; i++)
        {
            if (triggers[i].manageTrigger()) // Compute the current state and return if there is a hit
            {
                usbMIDI.sendNoteOn(triggers[i].getTriggerNote(), triggers[i].getTriggerVelocity(), triggers[i].getTriggerChannel());
                usbMIDI.sendNoteOff(triggers[i].getTriggerNote(), triggers[i].getTriggerVelocity(), triggers[i].getTriggerChannel());
                // USB Midi can handle 16 messages, thus a maximum of 8 notes On/Off
                // We aren't sending the midi message here to optimize the packets since there is no reasons sending a message for each hits
            }
        }

        forceTriggerUpdate = false;
    }
}

void adc0_isr(void)
{
    result = adc->readSynchronizedContinuous();

    tempRawADC[currentChannel] = (uint16_t)result.result_adc0;
    tempRawADC[currentChannel + NUMBER_CHANNELS] = (uint16_t)result.result_adc0;

    currentChannel++;
    if (currentChannel >= NUMBER_CHANNELS)
    {
        // Transfer the raw values into the triggers objects so the ISR can continue pooling in peace
        for (uint8_t i = 0; i < NUMBER_CHANNELS; i++)
        {
            triggers[i].readTrigger(tempRawADC[i]);
            triggers[i + NUMBER_CHANNELS].readTrigger(tempRawADC[i + NUMBER_CHANNELS]);
        }

        currentChannel = 0;
        forceTriggerUpdate = true;
    }
    selectMuxPin(currentChannel);
}