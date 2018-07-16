#include "flight_timers.h"

//config flags
int deployment_conditions_met = 0; //deployment flag, indicates when we are ready to deploy (3 month wait)
int antenna_extended = 0; //holds antenna status
int initial_delay_completed = 0; //whether or not we have done our initial delay

volatile int current_tick = 0; //global counter that increments every tick
//tickers
int next_chirp_tick = 0;
int sensor_1_tick = 0;
int listen_uplink_tick = 0;

//pin definitions
#define LED0 PORT_PA07
#define WDT_WDI PORT_PA19

//constants
#define chirp_battery_threshold 10 // TODO: pick a good battery value for chirping

// =================================== UTILITIES =================================== //
void handle_config() {
	//TODO: read config files and set flags/variables to recover last status
}

void extend_antenna() {
	//extend antenna here
}

void init_timers() {
	
}

void gather_sensor_data() {
	//TODO:
	//DMA out to sensors
	//SPI in to gather data
	//DMA out to write data to file
	
	//potentially turn the sensors on at the beginning and off at the end, if time permits
}

int get_battery_level() {
	//TODO: check battery level
}

int check_for_uplink() {
	//TODO: use radio library to check radio buffer
	
	//if (uplink detected) {
		//return 1
	//}
	return 0;
}

void handle_uplink() {
	//TODO: use radio library to read buffer and handle recieved data
}

int check_burnwire_uplink() {
	//check radio buffer for burnwire commands
	//returns a 0 if no commands found,
	//or a 1, 2, or 3 depending on which burnwire had been commanded to burn
}

void enter_sleep_mode() {
	//put the micro into sleep mode until the next interrupt
	__WFI(); //Wait For Interrupt instruction, enters STANDBY mode
}

// =========================== CRITICAL MODES AND FUNCTIONS =========================== //
void standby_mode() {
    while (current_tick < next_chirp_tick) {
		
		//these tasks are checked directly after the WDT loop has finished executing, meaning we should have almost an entire second to handle them
		//all of these are in if/else loops so that we never execute more than one per standby loop
		//tasks are ordered by priority
		//if a task is not executed in this loop when it normally would be, it will just wait until the next loop
		if (current_tick < listen_uplink_tick && check_for_uplink()) {
			handle_uplink():
		} else if (current_tick >= sensor_1_tick) {
			
			gather_sensor_data();
			sensor_1_tick += 100; //TODO choose value
			//do we want to do a simple sensor_1_tick += value, preserving the timing even if a tick is missed?
			//or do we want to do current_tick + value, in case current_tick is corrupted and ends up misaligned with sensor_1_tick?
		}
		//add entries for all 4-5 sensors
		
		//power down the micro, woken up by timer interrupt
		enter_sleep_mode();
	}
	
	if (get_battery_level() > chirp_battery_threshold) {
		chirp();
		//reset volatile counting integer
		next_chirp_tick = 270 + current_tick; //4.5 minutes
		
		//listen for a small amount of time for an uplink signal
		listen_uplink_tick = 15 + current_tick;
	}
}

void arming_mode() {
	int arming_timeout_tick = current_tick + 300; //set timeout to 5 minutes in the future 
	while (current_tick < arming_timeout_tick) {
		int w = check_burnwire_uplink();
		if (w == 1) {
			//burn wire 1
		} else if (w == 2) {
			//burn wire 2
		} else if (w == 3) {
			//burn wire 3
		}
		enter_sleep_mode();
	}
}

void chirp() {
	//TODO: collect self check data
	// check battery status
	// check solar panel status
	// check current time
	//TODO: write to self check data file
	//TODO: check for file corruption
}

void deploy_sprites() {
	//TODO: access deployment pre-compiled code and run
}

// =================================== MAIN CODE =================================== //
void setup() {
	//once power is on, we begin our initialization sequence
	init_timers();
	handle_config();
	if (!initial_delay_completed) {
		//hold for n minutes
	}
	
	//check if the antenna is down, if not, extend it
	if(!antenna_extended) {
		extend_antenna();
	}
	time_before_next_sensor_data = 300; //set here, as if we reboot, we want to skip past 15 minute wait
}

void loop() {
	while(!deployment_conditions_met){
		standby_mode();
	}
	//time to deploy
	deploy_sprites();
	while(true) { //go back to standby mode after deploying sprites
		standby_mode();
	}
}

// ============================== INTERRUPTS AND HANDLERS ============================== //
//WDT Loop
void TC3_Handler() {
	//TODO: check to see if this will automatically exit micro low power mode
	
	// Overflow interrupt triggered
	if ( TC3->COUNT16.INTFLAG.bit.OVF == 1 ) {
		//toggles WDT
		REG_PORT_OUTTGL0 = WDT_WDI;
		
		//increment/decrement any global volatiles here
		current_tick++;
		
		//clears the overflow interrupt flag
		REG_TC3_INTFLAG = TC_INTFLAG_OVF;
	}
	
	// Error interrupt triggered
	else if ( TC3->COUNT16.INTFLAG.bit.ERR == 1 ) {
		//possibly reset the micro if we ever get here
		
		//clears the error interrupt flag
		REG_TC3_INTFLAG = TC_INTFLAG_ERR;
	}
}
