#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <Modules\Button.hpp>
#include <Modules\Trigger.hpp>
#include "triggerModule_Defs.hpp"

// Encoder
Encoder knob(6, 7);
int32_t currentKnobPosition;
int32_t deltaKnobPosition;

// Button
const uint8_t BUTTON_PIN = 17;
Button button(BUTTON_PIN, 750, 10);

// LCD
enum Menu
{
    MenuMain,
    MenuDebug,
    NumberOfMenus
};
Menu currentMenu;
uint8_t currentPadID;
char* LCDBuffer = "                   ";
uint8_t currentLineUpdate;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
bool forceLCDUpdate;
elapsedMillis lastLCDupdate;
const uint32_t LCD_UPDATE_TIME = 500 / 4;

extern Trigger triggers[];

void initializeInterface()
{
    //LCD
    lcd.begin(20, 4);
    // Custom character https://maxpromer.github.io/LCD-Character-Creator/

    // Button
    button.initializeButton();

    // Variables
    currentMenu = MenuMain;
    currentLineUpdate = 0;
    currentPadID = 0;
}

void updateKnob()
{
    int32_t newKnobPosition;
    newKnobPosition = knob.read();
    deltaKnobPosition = newKnobPosition - currentKnobPosition;
    if (newKnobPosition != currentKnobPosition && (deltaKnobPosition % 4) == 0) // The knob has turned and the turn is finished (4 increments)
    {
        currentKnobPosition = newKnobPosition;
        switch (currentMenu)
        {
        case MenuMain:
            /* code */
            break;

        case MenuDebug:
            currentPadID += deltaKnobPosition / 4;
            if (currentPadID == NUMBER_TRIGGERS)
                currentPadID = 0;
            if (currentPadID > NUMBER_TRIGGERS) // Overflow from going lower than 0
                currentPadID = 31;
            break;

        default:
            break;
        }
    }

    switch (button.updateButton())
    {
    case ButtonShortPress:

        break;
    case ButtonLongPress:

        if (currentMenu == MenuMain)
        {
            currentMenu = MenuDebug;
        }
        else
        {
            currentMenu = MenuMain;
        }

        break;

    default:
        break;
    }
}

void updateInterface()
{
    if (forceLCDUpdate || lastLCDupdate > LCD_UPDATE_TIME) // Only refresh if forced or timing is achieved
    {
        switch (currentMenu)
        {
        case MenuMain:
            switch (currentLineUpdate)
            {
            case 0:
                LCDBuffer = "    Main Window    ";
                break;
            case 1:
                LCDBuffer = "                   ";
                break;
            case 2:
                LCDBuffer = "                   ";
                break;
            case 3:
                LCDBuffer = "                   ";
                break;

            default:
                break;
            }
            break;

        case MenuDebug:
            // > ID  :
            //   Note:
            //   Ch  :
            switch (currentLineUpdate)
            {
            case 0:
                LCDBuffer = "    Debug Window    ";
                break;
            case 1:
                LCDBuffer = ">ID  : ";
                __itoa(currentPadID,LCDBuffer[7],10);
                // strcat(LCDBuffer,);
                //LCDBuffer = String(">ID  : ") + currentPadID + "   "; // Clear the invalid numbers
                break;
            case 2:
                LCDBuffer = String(">Note: ") + triggers[currentPadID].getTriggerNote()+ "   ";
                break;
            case 3:
                LCDBuffer = String(">Chan: ") + triggers[currentPadID].getTriggerChannel()+ "   ";
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }

        lcd.setCursor(0, currentLineUpdate);
        lcd.print(LCDBuffer);

        currentLineUpdate++;
        if (currentLineUpdate >= 4)
            currentLineUpdate = 0;

        lastLCDupdate = 0;
        forceLCDUpdate = false;
    }
}