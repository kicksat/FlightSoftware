#ifndef FSWVARIABLES_h
#define FSWVARIABLES_h

#include <SdFat.h>
#include <KickSatConfig.h>
#include <KickSatLog.h>
#include <KickSat_Sensor.h>

extern SdFat SD;
extern KickSatConfig configFile(SD);
extern LogHandler logfile(SD);


extern KickSat_Sensor kSensor1(SPI_CS_XTB1, XTB_RESET, SPI_CS_SD, "c11.txt", SD);
extern KickSat_Sensor kSensor2(SPI_CS_XTB2, XTB_RESET, SPI_CS_SD, "c12.txt", SD);
//extern KickSat_Sensor kSensor3(SPI_CS_XTB3, XTB_RESET, SPI_CS_SD, "c13.txt", SD);
//extern KickSat_Sensor kSensor4(SPI_CS_XTB4, XTB_RESET, SPI_CS_SD, "c14.txt", SD);

#endif
