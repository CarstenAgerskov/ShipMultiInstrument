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

#include "NMEA.h"
#include "Configuration.h"

NMEA::NMEA(ShipData *sd) {
	shipData = sd;
};

/* $WIMWV, 217, R, 13.4, N, A * 0F */
void NMEA::nmeaDecode(String nmeaSentence, unsigned long receiveTimeStamp) {
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

void NMEA::decodeRMC(String nmeaSentence, unsigned long receiveTimeStamp) {
	Serial.println("decode $xxRMC");
	int startIndex = 6;
	/* UTC time */
	String t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0 && t.length() == 6) {
		Serial.print("UTC: ");
		shipData->setValue(UTC, t.substring(0, 2) + ":" + t.substring(2, 4) + ":" + t.substring(4, 6), receiveTimeStamp);
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
		Serial.print("SOG: ");
		shipData->setValue(SOG, t, receiveTimeStamp);
	}
	/* Track angle (True course) */
	t = extractData(nmeaSentence, startIndex);
	if (startIndex > 0) {
		Serial.print("TC: ");
		shipData->setValue(TC, t, receiveTimeStamp);
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
		Serial.print("MV: ");
		shipData->setValue(MV, t, receiveTimeStamp);
	}
	/* Magnetic variation direction */
	t = extractData(nmeaSentence, startIndex);
};

/*$WIMWV,217,R,13.4,N,A*0F*/
void NMEA::decodeMWV(String nmeaSentence, unsigned long receiveTimeStamp) {
	Serial.println("decode $xxMWV");
	int startIndex = 6;
	/* Wind Angle, 0 to 360 degrees */
	String t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
		Serial.print("RWA: ");
		shipData->setValue(RWA, t, receiveTimeStamp);
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
		Serial.print("RWS: ");
		shipData->setValue(RWS, t, receiveTimeStamp);
	}
	/* Wind Speed Units, K / M / N */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (startIndex > 0) {
		Serial.print("WSU: ");
		shipData->setValue(WSU, t, receiveTimeStamp);
	}
	/* Status, A = Data Valid */
	t = extractData(nmeaSentence, startIndex);
	Serial.println(t);
	if (!startIndex > 0) t = "";
	if (!t.equals("A") || !windReference.equals("R")) {
		Serial.print("RWA: ");
		shipData->setValue(RWA, "0.0", receiveTimeStamp);
		Serial.print("RWS: ");
		shipData->setValue(RWS, "0.0", receiveTimeStamp);
	}
};

String NMEA::extractData(String unprocessedData, int &startIndex) {
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

String NMEA::nmeaEncode(String nameToEncode) {
	return "not impl";
};

