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

#ifndef _APPROX_MAX_TIME_FILTER_H
#define _APPROX_MAX_TIME_FILTER_H

#include <Arduino.h>

#define MAX_TIME_FILTER_LENGTH 10

class ApproxMaxTimeFilter {
	float timeFilter[MAX_TIME_FILTER_LENGTH];
	unsigned long windIntervalTimeStamp;
	int windIntervalIndex;
	int filterLength;
	unsigned long timeSlotMillis;
public:
	ApproxMaxTimeFilter(int, unsigned long);
	void putValue(float);
	float getMax();
};


#endif