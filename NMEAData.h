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

#define MAX_VALUES 15
#define MAX_NAME_LENGTH 8
#define MAX_VALUE_LENGTH 10
#define DEFAULT_VALID_TIME 2000


#define SOG 0
#define UTC 1
#define RWS 3
#define TWS 4
#define RWA 5
#define TC 6
#define TWA 7
#define MWS10 8
#define MWS60 9
#define MV 10
#define TCP45 11
#define TCM45 12
#define LTZ 13

class NMEAData {
  String name[MAX_VALUES];
  String value[MAX_VALUES];
  unsigned long validTime[MAX_VALUES];  
  unsigned long timeStamp[MAX_VALUES];  
  String extractData(String, int &);
  float stringToFloat(String);
  String floatToString1d(float);
  String floatToString0d(float);
  void decodeRMC(String, unsigned long);
  void decodeMWV(String, unsigned long);
public:
  NMEAData();
  void nmeaDecode(String, unsigned long);
  String nmeaEncode(String);
  String getValue(int);
  void setValue(int, String);
  void calculate();
};

#endif
