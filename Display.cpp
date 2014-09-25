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

#define CLEAR_STR "          "

Display::Display(ShipData *sd){
  shipData = sd;
  lcd = new LiquidCrystal_I2C (0x3f,20,4);
  lcd->init();
  lcd->init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd->backlight();
  lcd->setCursor(3,0);
  lcd->print("NMEA Instrument");
}

void Display::displayValue(int x, int y, String s) {
  lcd->setCursor(x,y);
  lcd->print(CLEAR_STR);
  lcd->setCursor(x,y);
  lcd->print(s);
}

void Display::render() {
  displayValue(0,0,"TWS");
  displayValue(4, 0, shipData->getValue(TWS));
  displayValue(10,0,"TWA");
  displayValue(14,0, shipData->getValue(TWA));
  displayValue(0, 1, "C-45");
  displayValue(5, 1, shipData->getValue(TCM45));
  displayValue(10, 1, "C+45");
  displayValue(15, 1, shipData->getValue(TCP45));
  displayValue(0, 2, "M10");
  displayValue(4,2, shipData->getValue(MWS10));
  displayValue(10, 2, "MAXTWS60");
  displayValue(0,3,"XXX");
  displayValue(10,3,shipData->getValue(LTZ));
}
