#include <Arduino.h>
#include <SdFat.h>
#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_

class KickSat_Sensor
{

  public:
    KickSat_Sensor(int adc_cs, int adc_rst, int sd_cs, String cf_name, SdFat _sd);
    int operate(byte* dataOut);
    String getCommand(String data, char separator, int index);
    void handleCommand(String cmd, byte* buf, int* index);
    void parseMessage(String msg, String arg[]);

    bool rewriteConfigs(byte* buf, int len);
    bool validateConfigFiles();
    
    void burstWriteRegs(byte start, uint8_t len, byte* data);
    void startADC();
    void stopADC();
    void resetADC();
    void shutdownADC();
    void wakeADC();
    void regReadout();
    void mosfetV(byte pinNum);
    void readTemp();
    float dataConvert( byte a, byte b, byte c);

  private:
    int _ADCchipSelect;
    int _ADCreset;
    int _SDchipSelect;
    String _configFileName;
    byte val1, val2, val3;
    byte data_filter_chop_reg = 0x17;
    byte vbias_reg = 0x0;
    byte gain_pga_reg = 0x8;


    const float refV = 2.5; //reference voltage for the ADC. Usually use internal 2.5V by setting the registers below
    const float pgaGain = 1;
    const float FSR = (refV*2)/pgaGain;
    const float LSBsize = FSR/pow(2,24);
    bool showHex = true;
    SdFat SD;
};

#endif /* KICKSAT_SENSOR_H_ */
