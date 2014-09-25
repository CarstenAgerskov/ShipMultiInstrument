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

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include "ShipData.h"

#define DISPLAY_LINES 4
#define DISPLAY_COLUMNS 20
#define DISPLAY_I2C_ADDRESS  0x3F


class Display {
	ShipData *shipData;
    LiquidCrystal_I2C *lcd;
    void displayValue(int x, int y, String s);
	char displayBuffer[DISPLAY_LINES*DISPLAY_COLUMNS+1];
  public:
	Display(ShipData *);
    void render();
};

#endif



