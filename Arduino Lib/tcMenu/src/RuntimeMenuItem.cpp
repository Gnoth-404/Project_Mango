/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Nutricherry LTD).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#include <PlatformDetermination.h>
#include <IoLogging.h>
#include "RuntimeMenuItem.h"

static uint16_t nextAvailableRandomId = RANDOM_ID_START;

const char ALLOWABLE_EDIT_CHARACTERS[] PROGMEM = " .,0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"$%^&*()_-+=@';:#{/?\\|<>#{}~";

int nextRandomId() {
	return nextAvailableRandomId++;
}

RuntimeMenuItem::RuntimeMenuItem(MenuType menuType, uint16_t id, RuntimeRenderingFn renderFn,
	uint8_t itemPosition, uint8_t numberOfRows, MenuItem* next)	: MenuItem(menuType, NULL, next) {
	this->id = id;
	this->noOfParts = numberOfRows;
	this->renderFn = renderFn;
	this->itemPosition = itemPosition;
}

ListRuntimeMenuItem::ListRuntimeMenuItem(uint16_t id, int numberOfRows, RuntimeRenderingFn renderFn, MenuItem* next)
	: RuntimeMenuItem(MENUTYPE_RUNTIME_LIST, id, renderFn, 0xff, numberOfRows, next) {
	activeItem = 0;
}

void TextMenuItem::setTextValue(const char* text, bool silent) {
	// skip if they are the same
	if (strncmp(data, text, textLength()) == 0) return;

	strncpy(data, text, textLength());
	cleanUpArray();
	setChanged(true);
	setSendRemoteNeededAll();
	if (!silent) triggerCallback();
}

void TextMenuItem::cleanUpArray() {
	uint8_t len = 0;
	uint8_t actualLen = textLength();
	data[actualLen - 1] = 0;
	while (len < actualLen && data[len] != 0) len++;
	for (int i = len; i < actualLen; i++) {
		data[i] = 0;
	}
}

bool TextMenuItem::setCharValue(uint8_t location, char val) {
	if (location >= (noOfParts - 1)) return false;

	data[location] = val;
	// always ensure zero terminated at last position.
	cleanUpArray();
	setChanged(true);
	setSendRemoteNeededAll();
	return true;
}

void wrapForEdit(int val, int idx, uint8_t row, char* buffer, int bufferSize, bool forTime = false) {
	--row;

	if (idx == row) appendChar(buffer, '[', bufferSize);
	fastltoa(buffer, val, forTime ? 2 : 4, forTime ? '0' : NOT_PADDED, bufferSize);
	if (idx == row) appendChar(buffer, ']', bufferSize);
}

int ipAddressRenderFn(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
	if (item->getMenuType() != MENUTYPE_IPADDRESS) return 0;
	IpAddressMenuItem* ipItem = reinterpret_cast<IpAddressMenuItem*>(item);

	switch (mode) {
	case RENDERFN_VALUE: {
		buffer[0] = 0;
		uint8_t* data = ipItem->getIpAddress();
		wrapForEdit(data[0], 0, row, buffer, bufferSize);
		appendChar(buffer, '.', bufferSize);
		wrapForEdit(data[1], 1, row, buffer, bufferSize);
		appendChar(buffer, '.', bufferSize);
		wrapForEdit(data[2], 2, row, buffer, bufferSize);
		appendChar(buffer, '.', bufferSize);
		wrapForEdit(data[3], 3, row, buffer, bufferSize);
		return true;
	}
	case RENDERFN_SET_VALUE: {
		ipItem->setIpPart(row - 1, (uint8_t)buffer[0]);
		return true;
	}
	case RENDERFN_GETPART: {
		uint8_t* data = ipItem->getIpAddress();
		return (int)data[row - 1];
	}
	case RENDERFN_NAME: {
		if (buffer) buffer[0] = 0;
		return true;
	}
	case RENDERFN_GETRANGE: return 255;
    default: return false;
	}
}

int twelveHourTime(int hr) {
    if(hr == 0 || hr == 12) return 12;
    else if(hr < 12) return hr;
    else return hr - 12;
}

int timeItemRenderFn(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
	if (item->getMenuType() != MENUTYPE_TIME) return 0;
    TimeFormattedMenuItem* timeItem = reinterpret_cast<TimeFormattedMenuItem*>(item);
    int idx = row - 1;
    TimeStorage data = timeItem->getTime();

    switch(mode) {
    case RENDERFN_NAME: {
		if (buffer) buffer[0] = 0;
		return true;
	}
    case RENDERFN_VALUE: {
        bool twelveHr = timeItem->getFormat() == EDITMODE_TIME_12H;
		buffer[0] = 0;
        int hr = twelveHr ? twelveHourTime(data.hours) : data.hours;
		wrapForEdit(hr, 0, row, buffer, bufferSize, true);
		appendChar(buffer, ':', bufferSize);
		wrapForEdit(data.minutes, 1, row, buffer, bufferSize, true);
		appendChar(buffer, ':', bufferSize);
		wrapForEdit(data.seconds, 2, row, buffer, bufferSize, true);
        if(timeItem->getFormat() == EDITMODE_TIME_HUNDREDS_24H) {
            appendChar(buffer, '.', bufferSize);
            wrapForEdit(data.hundreds, 3, row, buffer, bufferSize, true);
        }
        else if(twelveHr)  {
            appendChar(buffer, (data.hours > 11) ? 'P' : 'A', bufferSize);
            appendChar(buffer, 'M', bufferSize);
        }
		return true;
	}
    case RENDERFN_GETRANGE: {
		if(idx == 0) return 23;
        else if(idx == 1 || idx == 2) return 59;
        else if(idx == 3) return 99;
        else return true;		
    }
    case RENDERFN_GETPART: {
		if(idx == 0) return data.hours;
        else if(idx==1) return data.minutes;
        else if(idx==2) return data.seconds;
        else return data.hundreds;
	}

	case RENDERFN_SET_VALUE: {
		int idx = row - 1;
        if(idx == 0) timeItem->getUnderlyingData()->hours = buffer[0];
        else if(idx == 1) timeItem->getUnderlyingData()->minutes = buffer[0];
        else if(idx == 2) timeItem->getUnderlyingData()->seconds = buffer[0];
        else if(idx == 3) timeItem->getUnderlyingData()->hundreds = buffer[0];
        return true;
	}
    default: return false;
    }
}

char DateFormattedMenuItem::separator = '/';
DateFormattedMenuItem::DateFormatOption DateFormattedMenuItem::dateFormatMode = DateFormattedMenuItem::DD_MM_YYYY;

int daysForMonth(DateStorage& theDate) {
    auto month = theDate.month;
    auto year = theDate.year;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return isLeap ?  29 : 28;
    }
    else return 31;
}

int toNaturalDateIndex(int act) {
    auto fmt = DateFormattedMenuItem::getDateFormatStyle();
    if(fmt == DateFormattedMenuItem::DD_MM_YYYY) {
        return act;
    }
    else if(fmt == DateFormattedMenuItem::MM_DD_YYYY) {
        return act == 2 ? 2 : act == 1 ? 0 : 1;
    }
    else /* YYYY_MM_DD */ {
        return act == 1 ? 1 : act == 2 ? 0 : 2;
    }
}
int toNaturalDateField(DateStorage dateStorage, int act) {
    auto fld = toNaturalDateIndex(act);
    return fld == 0 ? dateStorage.day : fld == 1 ? dateStorage.month : dateStorage.year;
}
int dateItemRenderFn(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
    if (item->getMenuType() != MENUTYPE_DATE) return 0;
    auto timeItem = reinterpret_cast<DateFormattedMenuItem*>(item);
    auto idx = row - 1;
    auto data = timeItem->getDate();
    auto sep = DateFormattedMenuItem::getDateSeparator();

    switch(mode) {
        case RENDERFN_NAME: {
            if (buffer) buffer[0] = 0;
            return true;
        }
        case RENDERFN_VALUE: {
            buffer[0] = 0;
            wrapForEdit(toNaturalDateField(timeItem->getDate(), 0), 0, row, buffer, bufferSize, toNaturalDateIndex(0) != 2);
            appendChar(buffer, sep, bufferSize);
            wrapForEdit(toNaturalDateField(timeItem->getDate(), 1), 1, row, buffer, bufferSize, toNaturalDateIndex(1) != 2);
            appendChar(buffer, sep, bufferSize);
            wrapForEdit(toNaturalDateField(timeItem->getDate(), 2), 2, row, buffer, bufferSize, toNaturalDateIndex(2) != 2);
            return true;
        }
        case RENDERFN_GETRANGE: {
            if(idx == toNaturalDateIndex(0)) return daysForMonth(data);
            else if(idx == toNaturalDateIndex(1)) return 12;
            else if(idx == toNaturalDateIndex(2)) return 9999;
            else return true;
        }
        case RENDERFN_GETPART: {
            if(idx == toNaturalDateIndex(0)) return data.day;
            else if(idx==toNaturalDateIndex(1)) return data.month;
            else return data.year;
        }

        case RENDERFN_SET_VALUE: {
            int idx = row - 1;
            if(idx == toNaturalDateIndex(0)) timeItem->getUnderlyingData()->day = buffer[0];
            else if(idx == toNaturalDateIndex(1)) timeItem->getUnderlyingData()->month = buffer[0];
            else if(idx == toNaturalDateIndex(2)) timeItem->getUnderlyingData()->year = *((int*)buffer);
            return true;
        }
        default: return false;
    }
}

int backSubItemRenderFn(RuntimeMenuItem* item, uint8_t /*row*/, RenderFnMode mode, char* buffer, int bufferSize) {
	switch (mode) {
	    case RENDERFN_NAME:
	        if(item->getMenuType() == MENUTYPE_SUB_VALUE && ((SubMenuItem*)item)->getNamePGMUnsafe()) {
	            const char* name = ((SubMenuItem*)item)->getNamePGMUnsafe();
                safeProgCpy(buffer, name, bufferSize);
	        }
	        else {
	            buffer[0] = 0;
	        }
	        return true;
	    case RENDERFN_EEPROM_POS:
	        return -1;
	    case RENDERFN_VALUE:
            buffer[0] = 0;
            return true;
        default: return false;
	}
}

inline char charFromEditableSet(int i) {
	const char* ptr = &ALLOWABLE_EDIT_CHARACTERS[i];
	return pgm_read_byte_near(ptr);
}

int findPositionInEditorSet(char ch) {
	if (ch == 0) return 0;
	for (int i = 0; i < ALLOWABLE_CHARS_ENCODER_SIZE; i++) {
		char pgmCmp = charFromEditableSet(i);
		if (pgmCmp == ch) {
			return i + 1;
		}
	}
	return 0; // return zero terminator when not found
}

int textItemRenderFn(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
	if (item->getMenuType() != MENUTYPE_TEXT_VALUE) return 0;
	auto txtItem = reinterpret_cast<TextMenuItem*>(item);

	switch (mode) {
	case RENDERFN_VALUE: {
		buffer[0] = 0;
		row--;
		for (int i = 0; i < txtItem->textLength(); ++i) {
			char txtVal = txtItem->getTextValue()[i];

			if (i == row) {
                appendChar(buffer, '[', bufferSize);
    			appendChar(buffer, txtVal, bufferSize);
    			appendChar(buffer, ']', bufferSize);
            } 
            else if(txtVal != 0) {
                appendChar(buffer, (txtItem->isPasswordField()) ? '*' : txtVal, bufferSize);
            }
            else {
                appendChar(buffer, 0, bufferSize);
            }

			if (!txtVal) break;
		}
		return true;
	}
	case RENDERFN_GETRANGE: {
		// we only enter more rows if there isn't a null terminator
		// but position 0 is always allowed for editing.
		int idx = row - 1;
		return (idx > 1 && txtItem->getTextValue()[idx - 1] == 0) ? 0 : ALLOWABLE_CHARS_ENCODER_SIZE;
	}
	case RENDERFN_SET_VALUE: {
		int idx = row - 1;
		int offset = buffer[0];
		if (offset == 0) {
			txtItem->setCharValue(idx, 0);
			return true;
		}
		return txtItem->setCharValue(idx, charFromEditableSet(offset - 1));
	}
	case RENDERFN_NAME: {
		if (buffer) buffer[0] = 0;
		return true;
	}
	case RENDERFN_GETPART: {
		return findPositionInEditorSet(txtItem->getTextValue()[row - 1]);
	}
    default: return false;
	}
}

void IpAddressMenuItem::setIpAddress(const char * ipData) {
	memset(data, 0, sizeof(data));
	char part[4];
	uint8_t currPart = 0;
	while (*ipData && currPart < 4) {
		part[0] = 0;
		while (*ipData && *ipData != '.') {
			appendChar(part, *ipData, sizeof(part));
			ipData++;
		}
		serdebugF3("IpPart", getId(), part);
		setIpPart(currPart, atoi(part));
		currPart++;
		if(*ipData) ipData++;
	}
}

void IpAddressMenuItem::setIpAddress(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
	data[0] = p1;
	data[1] = p2;
	data[2] = p3;
	data[3] = p4;
	setChanged(true);
	setSendRemoteNeededAll();
}

long parseIntUntilSeparator(const char* ptr, int& offset) {
    char sz[10];
    unsigned int pos = 0;
    // skip any non numerics
    while(ptr[offset] && (ptr[offset] < '0' || ptr[offset] > '9')) {
        offset++;
    }
    // collect the numerics
    while(pos < (sizeof(sz)-1) && ptr[offset] && ptr[offset] >= '0' && ptr[offset] <= '9') {
        sz[pos++] = ptr[offset++];
    }
    sz[pos] = 0;
	return atol(sz);
}

void TimeFormattedMenuItem::setTimeFromString(const char* ptr) {
    int offset = 0;
    data.hours = parseIntUntilSeparator(ptr, offset);
    data.minutes = parseIntUntilSeparator(ptr, offset);
    data.seconds = parseIntUntilSeparator(ptr, offset);
    data.hundreds = parseIntUntilSeparator(ptr, offset);
}

void DateFormattedMenuItem::setDateFromString(const char *dateText) {
    int offset = 0;
    data.year = parseIntUntilSeparator(dateText, offset);
    data.month = parseIntUntilSeparator(dateText, offset);
    data.day = parseIntUntilSeparator(dateText, offset);
}