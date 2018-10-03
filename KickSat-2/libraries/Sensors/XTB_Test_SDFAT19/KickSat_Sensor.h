#include <Arduino.h>
#include <SdFat.h>

#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_
#define ADC_CS A2
#define ADC_RST A1
#define SD_CS 4
#define GREEN 8                 // green LED pin on 8
#define RED 13                  // red LED pin on 13
#define VBATPIN A7              // Battery Voltage on Pin A7

#define SampleIntMin 00         // RTC - Sample interval in minutes
#define SampleIntSec 1          // RTC - Sample interval in seconds
#define SamplesPerCycle 5       // Number of samples to buffer before SD card flush is called. (not used) 


class KickSat_Sensor
{

  public:
    File logfile;
    KickSat_Sensor(int adc_cs, int adc_rst, int sd_cs, String cf_name);
    String cf_list = "";
    void operate(float* dataOut, String boardNum);
    void parseMessage(String msg, String arg[]);
    void handleCommand(String cmd, float* buf, int* index);
    void burstWriteRegs(byte start, uint8_t len, byte* data);
    void startADC();
    void stopADC();
    void resetADC();
    void shutdownADC();
    void wakeADC();
    void regReadout();
    void mosfetV(byte pinNum);
    float readTemp();
    void CreateFile();
    void writeHeader();
    void hallSpinC(byte idacmag);
    void hallSpinD(byte idacmag);
    float readPins(byte pinNums, byte idacPin, byte vbPin, bool save, int wait, int bufflen, byte idacMag);
    void BatteryVoltage(bool save);
    void GPIO(byte pins, byte state);
    void SFOCAL();        
    char filename[15];
    float hallGen(int inp, int inn, byte idacMag, int idacMux, int vb, int delayT, String label);
    
    
    float dataConvert( byte a, byte b, byte c);
    float mosData;
     
    float voltageApplied;
    bool writeable = false;
    int cnt = 0;
    int bufflen = 0;
    

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
    bool startUP = true; 
    
    
};

#endif /* KICKSAT_SENSOR_H_ */
