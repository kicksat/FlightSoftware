//KickSatConfig.h


#ifndef KICKSATCONFIG_h
#define KICKSATCONFIG_h

#include "Arduino.h"
#include "SD.h"
#include "KickSatLog.h"

#define CS SPI_CS_SD
#define AB1_BYTE 0x1E
#define AB2_BYTE 0x3E
#define STANDBY_BYTE 0x02
#define ARMED_BYTE 0x06
#define DEPLOYED_BYTE 0x0E


class KickSatConfig {
public:
    int checkAntennaTimer();
    void incrementAntennaTimer();
    byte getStatus();
    bool getAB1status();
    bool getAB2status();
    bool getStandbyStatus();
    bool getArmedStatus();
    bool getDeployedStatus();

    void setAB1Deployed();
    void setAB2Deployed();
    void setStandby();
    void setArmed();
    void setDeployed();

    bool init();
    void startSD();
    void endSD();

    bool errorCorrectOpening();
    bool errorCorrectContents();

    bool initFile(String fileName);

//private:

    bool available(String filename);
    bool writeByteToThree(byte data, int location);
    bool readByteFromThree(byte data[3], int location);
    bool writeByte(String filename, byte data, int location);
    byte readByte(String filename, int location);
};




#endif
