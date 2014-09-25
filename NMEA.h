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

#ifndef _NMEA_H
#define _NMEA_H

#include <WString.h>
#include <Arduino.h>
#include "ShipData.h"

class NMEA {
	ShipData *shipData;
	String extractData(String, int &);
	void decodeRMC(String, unsigned long);
	void decodeMWV(String, unsigned long);
public:
	NMEA(ShipData *shipData);
	void nmeaDecode(String, unsigned long);
	String nmeaEncode(String);
};

#endif
