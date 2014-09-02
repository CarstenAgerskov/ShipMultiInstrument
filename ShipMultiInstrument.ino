/*
Ship Multi-Instrument

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

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "RollingLog.h"
#include "NMEAData.h"
#include "Display.h"

#define NMEA_MAX_SENTENCE_LENGTH 120
#define LED_PIN 13

#define RENDER_INTERVAL 1000

RollingLog rollingLog1(NMEA_MAX_SENTENCE_LENGTH, 13, 13);
RollingLog rollingLog2(NMEA_MAX_SENTENCE_LENGTH, 13, 10);
NMEAData nmeaData;
Display *disp;

unsigned long renderTime = 0;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	Serial.begin(115200);
	Serial1.begin(4800);
	Serial2.begin(4800);
	Serial3.begin(115200);
	Serial.println("hello");
	delay(2000);
	disp = new Display();
}

void loop() {
	if (rollingLog1.getRwStatus() == READ_READY) {
		Serial.print("Log 1 Time: ");
		Serial.print(rollingLog1.readTimeStamp());
		Serial.print(" Message: ");
		Serial.println(rollingLog1.readMessage());
		nmeaData.nmeaDecode(rollingLog1.readMessage(), rollingLog1.readTimeStamp());
		Serial3.println(rollingLog1.readMessage());
		rollingLog1.releaseEntry();
	}
	if (rollingLog2.getRwStatus() == READ_READY) {
		Serial.print("Log 2 Time: ");
		Serial.print(rollingLog2.readTimeStamp());
		Serial.print(" Message: ");
		Serial.println(rollingLog2.readMessage());
		nmeaData.nmeaDecode(rollingLog2.readMessage(), rollingLog2.readTimeStamp());
		Serial3.println(rollingLog2.readMessage());
		rollingLog2.releaseEntry();
	}

	if (millis() > renderTime) {
		nmeaData.calculate();
		disp->render(&nmeaData);
		renderTime = renderTime + RENDER_INTERVAL;
	}
}

void serialEvent1() {
	while (Serial1.available()) {
		char inChar = (char)Serial1.read();
		rollingLog1.writeChar(inChar);
	}
}


void serialEvent2() {
	while (Serial2.available()) {
		char inChar = (char)Serial2.read();
		rollingLog2.writeChar(inChar);
	}
}

