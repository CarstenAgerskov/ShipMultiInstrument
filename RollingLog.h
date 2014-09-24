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

#ifndef _ROLLINGLOG_H
#define _ROLLINGLOG_H

#include <Arduino.h>

#define LOG_SIZE 10
#define MAX_SENTENCE_LENGTH 83

#define READ_READY 0
#define WRITE_READY 1

class RollingLog {
		char inputLog[LOG_SIZE][MAX_SENTENCE_LENGTH];
        unsigned long timeStamp[LOG_SIZE];
        boolean rwStatus[LOG_SIZE];
        char endSentenceChar1;
		char endSentenceChar2;
		unsigned int writeIndex;
		unsigned int readIndex;
        int charIndex;
public:
	RollingLog(char, char);
	void writeChar(char);
	char * readMessage();
    unsigned long readTimeStamp();
    boolean getRwStatus();
    void releaseEntry();
};

#endif
