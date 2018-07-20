//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************
#ifndef BURN_h
#define BURN_h


#define ANTENNA_BURN_1  PORT_PA14
#define ANTENNA_BURN_2  PORT_PA08
#define DEPLOY_BURN_1   PORT_PB03
#define DEPLOY_BURN_2   PORT_PA09
#define BURN_RELAY_A	PORT_PA15
#define BURN_RELAY_B	PORT_PA10
#define ANTENNA_BURN_TIME   2000 //milliseconds
#define SPRITE_BURN_TIME   12000 //milliseconds


    void init(void);
    void burnAntennas(void);
    void burnAB1(void);
    void burnAB2(void);
    void burnDB1(void);
    void burnDB2(void);
	void delay(int milliseconds);


#endif
