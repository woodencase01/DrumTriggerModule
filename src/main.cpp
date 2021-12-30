#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Managers\TriggersManager.hpp"

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  //LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Open Drum Trigger");
  lcd.setCursor(2, 1);
  lcd.print("Initializing...");
  // Custom character https://maxpromer.github.io/LCD-Character-Creator/

  initializeTriggers();

  lcd.setCursor(0, 1);
  lcd.print("       Ready       ");
}

void loop()
{
  // put your main code here, to run repeatedly:
}