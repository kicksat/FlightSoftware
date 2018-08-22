/*
BatteryHandler - Gathers voltage and current data from the battery and its charging circuit

Last update on: 8-13-18
by: Andrea
*/

#ifndef BATT_h
#define BATT_h

#include "Arduino.h"

class BattHandle {
	public:
		BattHandle();
		void read(float buf[]);
		int readBattVoltage();
		float readBattCurrent();
		float readBattChargeCurrent();
};

#endif
