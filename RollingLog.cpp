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

#include "RollingLog.h"

RollingLog::RollingLog(unsigned int a, char b, char c) {
	maxSentenceLength = a;
	endSentenceChar1 = b;
	endSentenceChar2 = c;
	writeIndex = 0;
	readIndex = 0;

	for (int i; i < LOG_SIZE; i++) {
		inputLog[i].reserve(maxSentenceLength);
		rwStatus[i] = WRITE_READY;
		inputLog[i] = "";
	}
}

void RollingLog::writeChar(char inChar) {

	if (rwStatus[writeIndex] == WRITE_READY) {
		if (inputLog[writeIndex].length() < maxSentenceLength - 1) {
			if (inChar != endSentenceChar1 && inChar != endSentenceChar2) {
				inputLog[writeIndex] += inChar;
			}
			if (inChar == endSentenceChar2) {
				timeStamp[writeIndex] = millis();
				rwStatus[writeIndex] = READ_READY;
				writeIndex++;
				if (writeIndex >= LOG_SIZE) writeIndex = 0;
			}
		}
		else
		{
			Serial.println("NMEA sentence too long, received:");
			Serial.println(inputLog[writeIndex]);
			inputLog[writeIndex] = "";
		}
	}
	else {
		Serial.println("Buffer overflow. RollingLog can only hold LOG_SIZE entries");
	}
}

String RollingLog::readMessage() {
	return inputLog[readIndex];
}

unsigned long RollingLog::readTimeStamp() {
	return timeStamp[readIndex];
}

boolean RollingLog::getRwStatus() {
	return rwStatus[readIndex];
}

void RollingLog::releaseEntry() {
	inputLog[readIndex] = "";
	rwStatus[readIndex] = WRITE_READY;
	readIndex = readIndex + 1;
	if (readIndex >= LOG_SIZE) readIndex = 0;
}

