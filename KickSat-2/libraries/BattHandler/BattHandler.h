/*
BatteryHandler - Gathers voltage and current data from the battery and its charging circuit

Last update on: 24-8-18
by: Andrea
*/

#ifndef BATT_h
#define BATT_h

#include "Arduino.h"

class BattHandle {
	public:
		BattHandle();
		void read(float buf[]); //read battery voltage, current, and charging current and stores them on a buffer
		float readBattVoltage();
		float readBattCurrent();
		float readBattChargeCurrent();
};

#endif
