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

#include "ApproxMaxTimeFilter.h"

ApproxMaxTimeFilter::ApproxMaxTimeFilter(int l) {
	filterLength = l;
	for (int i = 0; i < filterLength; i++) {
		timeFilter[i] = 0;
	}
	windIntervalTimeStamp = 0;
	windIntervalIndex = 0;
}

void ApproxMaxTimeFilter::putValue(float value) {
	if (millis() > windIntervalTimeStamp + TIME_SLOT_MILLIS) {
		windIntervalIndex = windIntervalIndex + 1;
		if (windIntervalIndex == filterLength) windIntervalIndex = 0;
		timeFilter[windIntervalIndex] = 0;
		windIntervalTimeStamp = millis();
	}
	if (value > timeFilter[windIntervalIndex]) {
		timeFilter[windIntervalIndex] = value;
	}
}

float ApproxMaxTimeFilter::getMax() {
	float value = 0;
	for (int i = 0; i < filterLength; i++) {
		if (timeFilter[i] > value) value = timeFilter[i];
	}
	return value;
}