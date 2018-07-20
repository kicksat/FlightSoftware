#include "sam.h"

//PORT_PAXX: XX = # of port
#define LED0 PORT_PA07;
#define WDT_WDI PORT_PA19;

void init_TC3();
void enable_interrupts();

// Global error flag for TC3
volatile uint8_t TC3_error = 0;

int main(void)
{
	SystemInit(); // Initialize the SAM system
	enable_interrupts();
	init_TC3();
	
	// Configure LED0 and WDT_WDI as output
	//every bit in DIRSET corresponds to a pin. if set to 1, that pin is configured in output mode.
	REG_PORT_DIRSET0 = LED0;
	REG_PORT_DIRSET0 |= WDT_WDI;
	
	//this is a weird register that will toggle the state of any pin whose bit is set to 1.
	//as soon as you set the bit to 1, it toggles that pin, and goes back into its default state
	REG_PORT_OUTTGL0 = LED0;
	
	while (1)
	{
		//this is where the main flightcode would be
		REG_PORT_OUTTGL0 = LED0;
		volatile int index = 0;
		while (index < 1000000) {
			index++;
		}
	}
}


void init_TC3()
{
	/* Configure Timer/Counter 3 as a timer to blink LED0 */
	// Configure Clocks
	//                     Enable          Choose a clock generator       Select a generic clock to configure/use
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;
	//enabled the bus to TC3, somehow allowing its registers to be programmed
	REG_PM_APBCMASK |= PM_APBCMASK_TC3; // Enable TC3 bus clock
	
	// Configure TC3 (16 bit counter by default)
	//sets the prescaler value, which divides the clock frequency in order to provide lower frequency pulses
	REG_TC3_CTRLA |= TC_CTRLA_PRESCALER_DIV1024;
	
	// Enable interrupts
	//INTENSET register holds the data for which interrupts are enabled
	//each function of the micro has its own INTENSET register
	//in this case, we use the Timer/Counter 3 INTENSET to enable both the ERR and OVF interrupts
	REG_TC3_INTENSET = TC_INTENSET_OVF | TC_INTENSET_ERR;
	
	// Enable TC3
	//looks like the CTRLA register hold generic control items for the Timer/Counter
	//writing a 1 to the ENABLE bit enables the TC 
	REG_TC3_CTRLA |= TC_CTRLA_ENABLE;
	
	//waits until the synchronization between clocks is complete, then the status bit will be cleared
	while ( TC3->COUNT16.STATUS.bit.SYNCBUSY == 1 ){} // wait for TC3 to be enabled
}


void TC3_Handler()
{
	
	// Overflow interrupt triggered
	if ( TC3->COUNT16.INTFLAG.bit.OVF == 1 )
	{
		//toggles WDT
		REG_PORT_OUTTGL0 = WDT_WDI;
		
		//clears the overflow interrupt flag
		REG_TC3_INTFLAG = TC_INTFLAG_OVF;
	}
	
	// Error interrupt triggered
	else if ( TC3->COUNT16.INTFLAG.bit.ERR == 1 )
	{
		TC3_error = 1;
		
		//clears the error interrupt flag
		REG_TC3_INTFLAG = TC_INTFLAG_ERR;
	}
}


void enable_interrupts()
{
	NVIC_EnableIRQ( TC3_IRQn );
}
