#ifndef FSWVARIABLES_h
#define FSWVARIABLES_h

#include <SdFat.h>
#include <KickSatConfig.h>
#include <KickSatLog.h>

extern SdFat SD;
extern KickSatConfig configFile(SD);
extern LogHandler logfile(SD);


#endif
