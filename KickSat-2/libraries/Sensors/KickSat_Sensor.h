

#ifndef KICKSAT_SENSOR_H_
#define KICKSAT_SENSOR_H_

class KickSat_Sensor
{

  public:
    KickSat_Sensor(int adc_cs, int sd_cs, String cf_name);
    void operate(byte* dataOut);
    void parseMessage(String msg, String arg[]);
    void handleCommand(String cmd, byte* buf, int* index);
    

  private:
    int _ADCchipSelect;
    int _SDchipSelect;
    String _configFileName;
}

#endif /* KICKSAT_SENSOR_H_ */
