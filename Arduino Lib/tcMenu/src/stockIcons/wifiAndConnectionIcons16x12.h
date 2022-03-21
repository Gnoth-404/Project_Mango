/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Nutricherry LTD).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#ifndef _WIFI_AND_CONNECTION_ICONS_H_
#define _WIFI_AND_CONNECTION_ICONS_H_

#include <PlatformDetermination.h>

/**
 * @file wifiAndConnectionIcons16x12.h
 * 
 * Contains definitions for standard icons including the connection active icon and the wifi signal strength icon.
 * Icons in this file are all 16 x 12 pixels for medium to high res displays.
 */

//
// Notes for editors of this file.
// In this file we have the definition of icons for two widgets. They are fixed at 16x12 and the size must not be changed
//

const uint8_t iconWifiNotConnected[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x02, 0x40,
   0x04, 0x20, 0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0x80, 0x01 
};

const uint8_t iconWifiOneBar[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01 
};

const uint8_t iconWifiTwoBar[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x80, 0x01 
};

const uint8_t iconWifiThreeBar[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03,
   0x30, 0x0c, 0x08, 0x10, 0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x80, 0x01 
};

const uint8_t iconWifiFourBar[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0xc2, 0x43,
   0x30, 0x0c, 0x08, 0x10, 0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x80, 0x01 
};

const uint8_t iconConnectionNone[] PROGMEM = {
   0xfe, 0x7f, 0x01, 0x80, 0x55, 0x95, 0xa9, 0xaa, 0x55, 0x95, 0xa9, 0xaa,
   0x55, 0x95, 0xa9, 0xaa, 0x55, 0x95, 0xa9, 0xaa, 0x01, 0x80, 0xfe, 0x7f 
};

const uint8_t iconConnected[] PROGMEM = {
   0xfe, 0x7f, 0x01, 0x80, 0x6d, 0x96, 0x01, 0x80, 0x7d, 0x9b, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x81, 0x81, 0x01, 0x80, 0xfe, 0x7f 
};

static uint8_t iconEthernetConn[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x61, 0xfe, 0x31,
        0x00, 0x61, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t iconEthernetNone[] PROGMEM = {
        0x00, 0x00, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x3f, 0x00, 0x63, 0xfe, 0x31,
        0xc0, 0x61, 0x60, 0x3f, 0x30, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x00, 0x00
};

/** 
 * Defines a set of 5 icons for wifi, not connected and then various signal strength. 0 is no connection, 4 is good signal.
 * Usually used with a TitleWidget on a medium to high resolution display. the icons are 16 x 12 pixels.
 */
const uint8_t* const iconsWifi[]  PROGMEM = { iconWifiNotConnected, iconWifiOneBar, iconWifiTwoBar, iconWifiThreeBar, iconWifiFourBar };

/** 
 * Defines a set of 2 icons for connection active, a boolean state of either active (1) or not active (0).
 * Usually used with a TitleWidget on a medium to high resolution display. the icons are 16 x 12 pixels.
 */
const uint8_t* const iconsConnection[] PROGMEM = { iconConnectionNone, iconConnected };


/**
 * Defines a set of two icons to show the wired ethernet connected and disconnected state
 */
 const uint8_t* const iconsEthernetConnection[] PROGMEM = { iconEthernetNone, iconEthernetConn };
#endif