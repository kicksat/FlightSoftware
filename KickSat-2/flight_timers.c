#include <flight_timers.h>

/*
 * flight_timers.c
 *
 * Created: 02/07/2018 03:50:35 p.m.
 *  Author: Andrea Ramirez
 *
 * Contains all the timers used in the flight code for KickSat mission, Summer 2018
 */ 
//

//#define WDT_WDI PORT_PA19

//Currently using TC3 as WDT timer
void init_WDT(void) {
	SystemInit(); // Initialize the SAM system
	
	NVIC_EnableIRQ( TC3_IRQn );
	
	
	REG_PORT_DIRSET0 |= WDT_WDI;
	
	
	// Configure Clocks
	//                     Enable          Choose a clock generator       Select a generic clock to configure/use
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;
	//enabled the bus to TC3, somehow allowing its registers to be programmed
	REG_PM_APBCMASK |= PM_APBCMASK_TC3; // Enable TC3 bus clock
	
	// Configure TC3 (16 bit counter by default)
	//sets the prescaler value, which divides the clock frequency in order to provide lower frequency pulses
	REG_TC3_CTRLA |= TC_CTRLA_PRESCALER_DIV1024;
	//sets the clock to run in standby mode, which will be needed for low power operation
	REG_TC3_CTRLA |= TC_CTRLA_RUNSTDBY;
		
	// Enable interrupts
	//INTENSET register holds the data for which interrupts are enabled
	//each function of the micro has its own INTENSET register
	//in this case, we use the Timer/Counter 3 INTENSET to enable both the ERR and OVF interrupts
	REG_TC3_INTENSET = TC_INTENSET_OVF | TC_INTENSET_ERR;
		
	// Enable TC3
	//looks like the CTRLA register hold generic control items for the Timer/Counter
	//writing a 1 to the ENABLE bit enables the TC
	REG_TC3_CTRLA |= TC_CTRLA_ENABLE;
		
	//prepare micro for STANDBY mode
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	//waits until the synchronization between clocks is complete, then the status bit will be cleared
	while ( TC3->COUNT16.STATUS.bit.SYNCBUSY == 1 ){} // wait for TC3 to be enabled
}
