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

#include "NMEAData.h"

NMEAData::NMEAData() {
	for (int i = 0; i < MAX_VALUES; i++) {
		value[i].reserve(MAX_VALUE_LENGTH);
		value[i] = "";
		validTime[i] = DEFAULT_VALID_TIME;
		timeStamp[i] = 0;
	}
};

/* $WIMWV, 217, R, 13.4, N, A * 0F */
void NMEAData::nmeaDecode(String nmeaSentence, unsigned long receiveTimeStamp) {
	int delimiterIndex = nmeaSentence.indexOf(",");
	if (delimiterIndex == 6 && nmeaSentence.length() > 6 && nmeaSentence.substring(0, 1).equals("$")) {
		if (nmeaSentence.substring(3, 6).equals("RMC")) {
			decodeRMC(nmeaSentence, receiveTimeStamp);
		}
		if (nmeaSentence.substring(3, 6).equals("MWV")) {
			decodeMWV(nmeaSentence, receiveTimeStamp);
		}
	}
};

void NMEAData::decodeRMC(String nmeaSentence, unsigned long receiveTimeStamp) {
	Serial.println("decode $xxRMC");
	int startIndex = 6;
	/* UTC time */
	String t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0 && t.length() == 6) {
		value[UTC] = t.substring(0, 2) + ":" + t.substring(2, 4) + ":" + t.substring(4, 6);
		timeStamp[UTC] = receiveTimeStamp;
		Serial.print("UTC: ");
		Serial.println(value[UTC]);
	}
	/* Data valid */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0 && t.equals("A")) {
		Serial.println("Data is valid");
	}
	else {
		Serial.println("Data is invalid");
		return;
	}
	/* Lattitude */
	t = extractData(nmeaSentence, startIndex);
	t = extractData(nmeaSentence, startIndex);
	/* Longtitude */
	t = extractData(nmeaSentence, startIndex);
	t = extractData(nmeaSentence, startIndex);
	/* SOG */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
		value[SOG] = t;
		timeStamp[SOG] = receiveTimeStamp;
		Serial.print("SOG: ");
		Serial.println(value[SOG]);
	}
	/* Track angle (True course) */
	t = extractData(nmeaSentence, startIndex);
	if (startIndex > 0) {
		value[TC] = t;
		timeStamp[TC] = receiveTimeStamp;
		Serial.print("TC: ");
		Serial.println(value[TC]);
	}
	/* Date */
	t = extractData(nmeaSentence, startIndex);
	if (startIndex > 0) {
		Serial.print("DATE: ");
		Serial.println(t);
	}
	/* Magnetic variation */
	t = extractData(nmeaSentence, startIndex);
	if (startIndex > 0) {
		value[MV] = t;
		timeStamp[MV] = receiveTimeStamp;
		Serial.print("MV: ");
		Serial.println(value[MV]);
	}
	/* Magnetic variation direction */
	t = extractData(nmeaSentence, startIndex);
};

/*$WIMWV,217,R,13.4,N,A*0F*/
void NMEAData::decodeMWV(String nmeaSentence, unsigned long receiveTimeStamp) {
	Serial.println("decode $xxMWV");
	int startIndex = 6;
	/* Wind Angle, 0 to 360 degrees */
	String t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
		value[RWA] = t;
		timeStamp[RWA] = receiveTimeStamp;
		Serial.print("RWA: ");
		Serial.println(value[RWA]);
	}
	/* Reference, R = Relative, T = True */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	String windReference = "";
	if (startIndex > 0) {
		windReference = t;
	}
	/* Wind Speed */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
		value[RWS] = t;
		timeStamp[RWS] = receiveTimeStamp;
		Serial.print("RWS: ");
		Serial.println(value[RWS]);
	}
	/* Wind Speed Units, K / M / N */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
	}
	/* Status, A = Data Valid */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (!startIndex > 0) t = "";
	if (!t.equals("A") || !windReference.equals("R")) {
		value[RWA] = "0.0";
		timeStamp[RWA] = receiveTimeStamp;
		Serial.print("RWA: ");
		Serial.println(value[RWA]);
		value[RWS] = "0.0";
		timeStamp[RWS] = receiveTimeStamp;
		Serial.print("RWS: ");
		Serial.println(value[RWS]);
	}
};

String NMEAData::extractData(String unprocessedData, int &startIndex) {
	int endIndex = -1;
	if (startIndex > 0 && startIndex + 1 < unprocessedData.length()) {
		if (unprocessedData.charAt(startIndex) == ',') {
			endIndex = unprocessedData.indexOf(',', startIndex + 1);
			if (endIndex < startIndex) {
				endIndex = unprocessedData.indexOf('*', startIndex + 1);
			}
			if (endIndex >= startIndex) {
				String d = unprocessedData.substring(startIndex + 1, endIndex);
				startIndex = endIndex;
				return d;
			}
		}
	}
	startIndex = endIndex;
	return "";
}

String NMEAData::nmeaEncode(String nameToEncode) {
	return "not impl";
};

String NMEAData::getValue(int indexToFind) {
	return value[indexToFind];
};

void NMEAData::setValue(int indexToSet, String valueToSet) {
	value[indexToSet] = valueToSet;
}

void NMEAData::calculate() {
	float sog = stringToFloat(value[SOG]);
	float rws = stringToFloat(value[RWS]);
	float rwa = stringToFloat(value[RWA]) * 0.0174532925; /* Convert to rad */

	float sog_in_ms = sog * 0.51444444444;
	/* source http://en.wikipedia.org/wiki/Apparent_wind */
	float tws = sqrt(rws*rws + sog_in_ms*sog_in_ms - 2 * rws*sog_in_ms*cos(rwa));
	float twa = 0;

	if (tws > 0.2) {
		twa = acos((rws*cos(rwa) - sog_in_ms) / tws) * 57.2957795; /* Convert to deg */
	}
	else {
		tws = 0;
	}


	value[TWS] = floatToString1d(tws);
	timeStamp[TWS] = millis();

	value[TWA] = floatToString1d(twa);
	timeStamp[TWA] = millis();

	if (timeStamp[MWS10] < (millis() - (1000 * 60 * 10))) {
		value[MWS10] = value[TWS];
		timeStamp[MWS10] = millis();
	}

	float mws10 = stringToFloat(value[MWS10]);
	if (tws > mws10) {
		value[MWS10] = value[TWS];
		timeStamp[MWS10] = millis();
	}

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

float NMEAData::stringToFloat(String s) {
	char carray[s.length() + 1];
	s.toCharArray(carray, sizeof(carray));
	return atof(carray);
}

String NMEAData::floatToString1d(float f) {
	char tmp[10];
	dtostrf(f, 8, 1, tmp);
	String s = tmp;
	s.trim();
	return s;
}

String NMEAData::floatToString0d(float f) {
	char tmp[10];
	dtostrf(f, 9, 0, tmp);
	String s = tmp;
	s.trim();
	return s;
}

