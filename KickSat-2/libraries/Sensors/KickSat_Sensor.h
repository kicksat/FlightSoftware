#include <Arduino.h>

#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_

class KickSat_Sensor
{

  public:
    KickSat_Sensor(int adc_cs, int adc_rst, int sd_cs, String cf_name);
    void operate(byte* dataOut);
    void parseMessage(String msg, String arg[]);
    void handleCommand(String cmd, byte* buf, int* index);
    void burstWriteRegs(byte* data, uint8_t len);
    void startADC();
    void stopADC();
    void resetADC();
    void shutdownADC();
    void wakeADC();
    void regReadout();

  private:
    int _ADCchipSelect;
    int _ADCreset;
    int _SDchipSelect;
    String _configFileName;

    byte data_filter_chop_reg = 0x17;
    byte vbias_reg = 0x0;
    byte gain_pga_reg = 0x8;
};

#endif /* KICKSAT_SENSOR_H_ */
