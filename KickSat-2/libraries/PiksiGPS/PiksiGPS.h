
#ifndef PiksiGPS_h
#define PiksiGPS_h

#include <WProgram.h>
#include <libsbp/observation.h>
#include <libsbp/acquisition.h>
#include <libsbp/tracking.h>
#include <libsbp/sbp.h>
#include <libsbp/navigation.h>
#include <libsbp/common.h>
#include <libsbp/settings.h>
#include <libsbp/logging.h>
#include <libsbp/system.h>
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
	msg_acq_result_t get_acq_result();

	msg_tracking_state_dep_a_t get_tracking_state();

	msg_obs_dep_a_t get_obs();

	// msg_pos_llh_t                 pos_llh;
	// msg_baseline_ned_t            baseline_ned;
	// msg_baseline_ecef_t           baseline_ecef;
	// msg_vel_ned_t                 vel_ned;
	// msg_vel_ecef_t                vel_ecef;
	// msg_dops_t                    dops;
	// msg_gps_time_t                gps_time;
	// msg_pos_ecef_t                pos_ecef;
	// msg_settings_read_resp_t      settings;
	// msg_log_t                     logging;
	// msg_settings_read_req_t       setting_req;
	// msg_startup_t                 startup;

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
