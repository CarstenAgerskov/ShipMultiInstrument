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

#ifndef _SHIPDATA_H
#define _SHIPDATA_H

#include "Configuration.h"

#include <WString.h>
#include <Arduino.h>

#define MAX_VALUES 15
#define MAX_NAME_LENGTH 8
#define MAX_VALUE_LENGTH 10
#define DEFAULT_VALID_TIME 2000

class ShipData {
	String name[MAX_VALUES];
	String value[MAX_VALUES];
	unsigned long validTime[MAX_VALUES];
	unsigned long timeStamp[MAX_VALUES];
	float stringToFloat(String);
	String floatToString1d(float);
	String floatToString0d(float);
	String ktsToMs(String kts);
public:
	ShipData();
	String getValue(int);
	void setValue(int, String, unsigned long);
	void calculate();
	void print();
};

#endif