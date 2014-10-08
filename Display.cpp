/*

This file is part of Ship Multi-Instrument

Copyright(C) 2014  Carsten Agerskov

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "Display.h"
#include "Configuration.h"

#include <LiquidCrystal_I2C.h>


Display::Display(ShipData *sd) {
	shipData = sd;
	lcd = new LiquidCrystal_I2C(0x3f, 20, 4);
	lcd->init();
	lcd->backlight();
	lcd->setCursor(3, 0);
	lcd->print("Ship Multi-Instrument");
}

void Display::render() {
	for (int i = 0; i < DISPLAY_LINES*DISPLAY_COLUMNS; i++) {
		displayBuffer[i] = ' ';
	}
	memcpy(&displayBuffer[0], "TWS", 3);
	memcpy(&displayBuffer[4], shipData->getValue(TWS).c_str(), shipData->getValue(TWS).length());
	memcpy(&displayBuffer[10], "TWA", 3);
	memcpy(&displayBuffer[14], shipData->getValue(TWA).c_str(), shipData->getValue(TWA).length());
	memcpy(&displayBuffer[DISPLAY_COLUMNS], "C-45", 4);
	memcpy(&displayBuffer[DISPLAY_COLUMNS + 5], shipData->getValue(TCM45).c_str(), shipData->getValue(TCM45).length());
	memcpy(&displayBuffer[DISPLAY_COLUMNS + 10], "C+45", 4);
	memcpy(&displayBuffer[DISPLAY_COLUMNS + 15], shipData->getValue(TCP45).c_str(), shipData->getValue(TCP45).length());
	memcpy(&displayBuffer[2 * DISPLAY_COLUMNS], "M10", 3);
	memcpy(&displayBuffer[2 * DISPLAY_COLUMNS + 4], shipData->getValue(MWS10).c_str(), shipData->getValue(MWS10).length());
	memcpy(&displayBuffer[2 * DISPLAY_COLUMNS + 10], "MAXTWS60", 8);
	memcpy(&displayBuffer[3 * DISPLAY_COLUMNS + 0], "XXX", 3);
	memcpy(&displayBuffer[3 * DISPLAY_COLUMNS + 10], shipData->getValue(LTZ).c_str(), shipData->getValue(LTZ).length());
	displayBuffer[DISPLAY_LINES*DISPLAY_COLUMNS] = '\0';
	lcd->setCursor(0, 0);
	lcd->print(displayBuffer);

}
