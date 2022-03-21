#include "src/unoSsd1306Ascii_menu.h"
#include <IoLogging.h>

SSD1306AsciiAvrI2c gfx;

#define I2C_ADDRESS 0x3C

int currentCommits = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting");
    gfx.begin(&SH1106_128x64, I2C_ADDRESS);
    gfx.clear();
    menuMgr.setItemCommittedHook([](int id) {
        ++currentCommits;
    });
    setupMenu();
}

void loop() {
    taskManager.runLoop();
}

const char szDialogHeader[] PROGMEM = "Edits Committed";

void CALLBACK_FUNCTION onActionPressed(int id) {
    BaseDialog* dlg = renderer.getDialog();
    dlg->setButtons(BTNTYPE_NONE, BTNTYPE_OK);
    dlg->show(szDialogHeader, true);

    char extraData[10];
    ltoaClrBuff(extraData, currentCommits, 5, '0', sizeof(extraData));
    dlg->copyIntoBuffer(extraData);
}


// see tcMenu list documentation on thecoderscorner.com
int CALLBACK_FUNCTION fnListItemRtCall(RuntimeMenuItem * item, uint8_t row, RenderFnMode mode, char * buffer, int bufferSize) {
   switch(mode) {
    case RENDERFN_INVOKE:
        // if you wanted to take action on a list item being activated, you'd put the code here.
        return true;
    case RENDERFN_NAME:
        if(row == 0xff) {
            // row 0xff is the title row basically, this requests title rendering.
            strcpy(buffer, "My List");
        }
        else {
            // Any other row is the name of a numbered item
            ltoaClrBuff(buffer, row, 3, NOT_PADDED, bufferSize);
        }
        return true;
    case RENDERFN_VALUE:
        // Each row can has its own value - 0xff is the parent item and is probably empty.
        buffer[0] = 'V'; buffer[1]=0;
        fastltoa(buffer, row, 3, NOT_PADDED, bufferSize);
        return true;
    case RENDERFN_EEPROM_POS: return 0xFFFF; // lists are generally not saved to EEPROM
    default: return false;
    }
}



