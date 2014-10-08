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

#include "ShipData.h"

ShipData::ShipData() {
	for (int i = 0; i < MAX_VALUES; i++) {
		value[i].reserve(MAX_VALUE_LENGTH);
		value[i] = "";
		validTime[i] = DEFAULT_VALID_TIME;
		timeStamp[i] = 0;
		approxMaxTimeFilter10m = new ApproxMaxTimeFilter(10);
		approxMaxTimeFilter60m = new ApproxMaxTimeFilter(6);
	}
};

void ShipData::print() {
	for (int i = 0; i < MAX_VALUES; i++) {
		Serial.print("i = ");
		Serial.print(i);
		Serial.print(": value = ");
		Serial.print(value[i]);
		Serial.print(": timeStamp = ");
		Serial.println(timeStamp[i]);
	}

}

String ShipData::getValue(int indexToFind) {
	return value[indexToFind];
};

void ShipData::setValue(int indexToSet, String valueToSet, unsigned long timeStampToSet) {
	value[indexToSet] = valueToSet;
	timeStamp[indexToSet] = timeStampToSet;
	Serial.println(value[indexToSet]);
}

float ShipData::stringToFloat(String s) {
	char carray[s.length() + 1];
	s.toCharArray(carray, sizeof(carray));
	return atof(carray);
}

String ShipData::floatToString1d(float f) {
	char tmp[10];
	dtostrf(f, 8, 1, tmp);
	String s = tmp;
	s.trim();
	return s;
}

String ShipData::floatToString0d(float f) {
	char tmp[10];
	dtostrf(f, 9, 0, tmp);
	String s = tmp;
	s.trim();
	return s;
}

String ShipData::ktsToMs(String kts) {
	float fKts = stringToFloat(kts);
	return floatToString1d(fKts*0.514);
}

void ShipData::calculate() {
	float sog = stringToFloat(value[SOG]);
	float rws = stringToFloat(value[RWS]);
	float rwa = stringToFloat(value[RWA]) * 0.0174532925; /* Convert to rad */

	/* source http://en.wikipedia.org/wiki/Apparent_wind */
	float tws = sqrt(rws*rws + sog*sog - 2 * rws*sog*cos(rwa));
	float twa = 0;

	if (tws > 0.2) {
		twa = acos((rws*cos(rwa) - sog) / tws) * 57.2957795; /* Convert to deg */
	}
	else {
		tws = 0;
	}


	value[TWS] = ktsToMs(floatToString1d(tws));
	timeStamp[TWS] = millis();

	value[TWA] = floatToString1d(twa);
	timeStamp[TWA] = millis();

	approxMaxTimeFilter10m->putValue(stringToFloat(value[TWS]));
	
	value[MWS10] = floatToString1d(approxMaxTimeFilter10m->getMax());
	timeStamp[MWS10] = millis();


	float hh = stringToFloat(value[UTC].substring(0, 2));
	hh = hh + 2;
	if (hh > 24) hh = hh - 24;
	if (hh < 0) hh = hh + 24;
	value[LTZ] = floatToString0d(hh) + value[UTC].substring(2, 5);

	float tcp45 = stringToFloat(value[TC]) + 45;
	if (tcp45 > 359) tcp45 = tcp45 - 360;
	value[TCP45] = floatToString0d(tcp45);

	float tcm45 = stringToFloat(value[TC]) - 45;
	if (tcm45 < 0) tcm45 = tcm45 + 360;
	value[TCM45] = floatToString0d(tcm45);

}


