/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
*/

#include <Arduino.h>
#include <tcMenu.h>
#include "remoteControlSerial_menu.h"

// Global variable declarations

const PROGMEM ConnectorLocalInfo applicationInfo = { "Remote Ctrl", "f018e07a-f33f-42d2-b3a0-689a1bf6849c" };
Adafruit_PCD8544 gfx(35, 34, 38, 37, 36);
AdaColorGfxMenuConfig gfxConfig;
AdaFruitGfxMenuRenderer renderer;

// Global Menu Item declarations

extern const char* choiceRamArray;
RENDERING_CALLBACK_NAME_INVOKE(fnChoiceRtCall, enumItemRenderFn, "Choice", -1, NO_CALLBACK)
ScrollChoiceMenuItem menuChoice(9, fnChoiceRtCall, 0, choiceRamArray, 5, 4, NULL);
const AnyMenuInfo PROGMEM minfoPushMe = { "Push Me", 6, 0xFFFF, 0, onPushMe };
ActionMenuItem menuPushMe(&minfoPushMe, &menuChoice);
const char enumStrFood_0[] PROGMEM  = "Pizza";
const char enumStrFood_1[] PROGMEM  = "Pasta";
const char enumStrFood_2[] PROGMEM  = "Salad";
const char* const enumStrFood[] PROGMEM  = { enumStrFood_0, enumStrFood_1, enumStrFood_2 };
const EnumMenuInfo PROGMEM minfoFood = { "Food", 5, 0xFFFF, 2, NO_CALLBACK, enumStrFood };
EnumMenuItem menuFood(&minfoFood, 0, &menuPushMe);
RENDERING_CALLBACK_NAME_INVOKE(fnTextRtCall, textItemRenderFn, "Text", -1, NO_CALLBACK)
TextMenuItem menuText(fnTextRtCall, 4, 10, &menuFood);
const AnalogMenuInfo PROGMEM minfoA2 = { "A2", 3, 0xFFFF, 1024, NO_CALLBACK, 0, 200, "V" };
AnalogMenuItem menuA2(&minfoA2, 0, NULL);
const AnalogMenuInfo PROGMEM minfoA1 = { "A1", 2, 0xFFFF, 1024, NO_CALLBACK, 0, 200, "V" };
AnalogMenuItem menuA1(&minfoA1, 0, &menuA2);
const AnalogMenuInfo PROGMEM minfoA0 = { "A0", 8, 0xFFFF, 1024, NO_CALLBACK, 0, 200, "V" };
AnalogMenuItem menuA0(&minfoA0, 0, &menuA1);
const SubMenuInfo PROGMEM minfoAnalogIn = { "Analog In", 7, 0xFFFF, 0, NO_CALLBACK };
RENDERING_CALLBACK_NAME_INVOKE(fnAnalogInRtCall, backSubItemRenderFn, "Analog In", -1, NO_CALLBACK)
BackMenuItem menuBackAnalogIn(fnAnalogInRtCall, &menuA0);
SubMenuItem menuAnalogIn(&minfoAnalogIn, &menuBackAnalogIn, &menuText);


// Set up code

void setupMenu() {
    menuA0.setReadOnly(true);
    menuA1.setReadOnly(true);
    menuA2.setReadOnly(true);

    prepareAdaMonoGfxConfigLoRes(&gfxConfig);
    gfx.begin();
    gfx.setRotation(0);
    renderer.setGraphicsDevice(&gfx, &gfxConfig);
    switches.initialise(internalDigitalIo(), true);
    menuMgr.initForEncoder(&renderer, &menuAnalogIn, 2, 6, A3);
    remoteServer.begin(&Serial1, &applicationInfo);
}