#include <Arduino.h>
#include "Managers\TriggersManager.hpp"
#include "Managers\InterfaceManager.hpp"
#include <MIDI.h>

void setup()
{
  Serial.begin(115200);
  initializeInterface();
  initializeTriggers();
}

void loop()
{
  updateTriggers();
  updateKnob();
  updateInterface();
  usbMIDI.send_now();
}