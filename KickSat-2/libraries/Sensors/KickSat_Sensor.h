//**********************************************************
// KickSat_Sensor.h
//**********************************************************
//TODO:

#include <Arduino.h>
#include <SdFat.h>

#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_
#define sensor1_BUF_LEN 5
#define sensor2_BUF_LEN 7
#define sensor3_BUF_LEN 9

class KickSat_Sensor
{
  public:
    KickSat_Sensor(String boardfile);

    void operate(byte* bufarray);
    void burstWriteRegs(byte start, uint8_t len, byte* data);
    void startADC();
    void stopADC();
    void resetADC();
    void shutdownADC();
    void wakeADC();
    void regReadout();
    void GPIO(byte pins, byte state);
    void writeReg(byte start, byte value);
    void sensorData(byte* data, uint8_t len);
    float getFloat(byte packet[], uint8_t i);
    float readTemp();
    float readPins(byte pinNums, byte idacPin, byte vbPin, int wait, int bufflen, byte idacMag);    
    float hallGen(uint8_t inp, uint8_t inn, byte idacMag, uint8_t idacMux, uint8_t vb, int delayT);    
    float dataConvert( byte a, byte b, byte c);   
    float voltageApplied;
    uint8_t bufflen = 0;
    String board;
    File datafile;
    float dataOut1[sensor1_BUF_LEN];
    float dataOut2[sensor2_BUF_LEN];
    float dataOut[sensor3_BUF_LEN];
    
    struct datastore {
      float dat1;
      float dat2;
      float dat3;
      float dat4;
    };
    struct sensorPayload {
      float d[100];
    };
  
  private:
    uint8_t _ADCchipSelect;
    uint8_t _ADCreset;
    uint8_t _SDchipSelect;
    byte readOut[17];
    const float refV = 2.5;
    const float pgaGain = 1;
    const float FSR = (refV*2)/pgaGain;
    const float LSBsize = FSR/pow(2,24);
    
};
#endif /* KICKSAT_SENSOR_H_ */