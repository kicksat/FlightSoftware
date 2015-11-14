



#ifndef PiksiGPS_h
#define PiksiGPS_h

#include <WProgram.h>
#include <libsbp/sbp.h>
#include <libsbp/navigation.h>
#include <libsbp/common.h>
#include <libsbp/settings.h>
#include <libsbp/logging.h>
#include <string.h>

class PiksiGPS {


public:
	PiksiGPS();
	void update();
	void initialize();
	void setThis(char* group, char* setting, char* value);
	void readThis(char* section, char* setting, char* result);
	msg_log_t get_logging();
	msg_pos_ecef_t get_pos_ecef();
	msg_vel_ecef_t get_vel_ecef();
	double get_pos_x();
	double get_pos_y();
	double get_pos_z();
	uint32_t get_tow();
	uint16_t get_week_num();
	double get_lat();
	double get_long();
	double get_height();

	//bool ready;
	int weekNum;
	int timeOfWeek;
	double xPos;
	double yPos;
	double zPos;
	int gpsAccuracy;
	short numSat;
	signed int xVel;
	signed int yVel;
	signed int zVel;
	volatile int read_wait;
	int write_wait;



};

#endif
