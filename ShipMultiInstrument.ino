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
#include "ShipData.h"
#include "NMEA.h"
#include "Display.h"

#define LED_PIN 13

#define RENDER_INTERVAL 1000

RollingLog rollingLog1(13, 10);
RollingLog rollingLog2(13, 10);
ShipData *shipData;
Display *disp;
NMEA * nmea;

unsigned long renderTime = 0;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	Serial.begin(115200);
	Serial1.begin(4800);
	Serial2.begin(38400);
	Serial3.begin(115200);
	delay(2000);
	shipData = new ShipData();
	nmea = new NMEA(shipData);
	disp = new Display(shipData);
	Serial.println("hello");
}

void loop() {
	if (rollingLog1.getRwStatus() == READ_READY) {
		Serial.print("Log 1 Time: ");
		Serial.print(rollingLog1.readTimeStamp());
		Serial.print(" Message: ");
		Serial.println(rollingLog1.readMessage());
		nmea->nmeaDecode(rollingLog1.readMessage(), rollingLog1.readTimeStamp());
		Serial3.println(rollingLog1.readMessage());
		rollingLog1.releaseEntry();
	}
	if (rollingLog2.getRwStatus() == READ_READY) {
		Serial.print("Log 2 Time: ");
		Serial.print(rollingLog2.readTimeStamp());
		Serial.print(" Message: ");
		Serial.println(rollingLog2.readMessage());
		nmea->nmeaDecode(rollingLog2.readMessage(), rollingLog2.readTimeStamp());
		Serial3.println(rollingLog2.readMessage());
		rollingLog2.releaseEntry();
	}

	if (millis() > renderTime) {
		shipData->calculate();
		disp->render();
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

