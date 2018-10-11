//**********************************************************
// KickSat_Sensor.h
//**********************************************************
//TODO:

#include <Arduino.h>
#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_

#include <SdFat.h>
#define sensor1_BUF_LEN 5
#define sensor2_BUF_LEN 7
#define sensor3_BUF_LEN 9

class KickSat_Sensor
{
  public:
    KickSat_Sensor(uint8_t adc_rst);
    void operate(String board);
    void burstWriteRegs(byte start, uint8_t len, byte* data);
    void startADC();
    void stopADC();
    void resetADC();
    void shutdownADC();
    void wakeADC();
    void regReadout();
    void GPIO(byte pins, byte state);
    void writeReg(byte start, byte value);
    void sensorPacket(byte* One, byte* Two, byte* Three);
    void sensorStream(String board, int length, byte* stream);
    float getFloat(byte packet[], uint8_t i);
    float readTemp();
    float readPins(byte pinNums, byte idacPin, byte vbPin, int wait, int bufflen, byte idacMag);    
    float hallGen(uint8_t inp, uint8_t inn, byte idacMag, uint8_t idacMux, uint8_t vb, int delayT);    
    float dataConvert( byte a, byte b, byte c);   
    float voltageApplied;
    String board;
    File datafile;
    struct sensorPayload {
      byte one[sensor1_BUF_LEN*4];
      byte two[sensor2_BUF_LEN*4];
      byte three[sensor3_BUF_LEN*4];
    };
    struct sensorPayload dataPac;
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
