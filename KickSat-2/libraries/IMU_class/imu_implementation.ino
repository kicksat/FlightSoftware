#include "imu.h"
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>


imu myIMU;
float imu_data[9];

void setup() 
{
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  myIMU.init_imu();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  myIMU.read_IMU(imu_data);
  for(int i = 0; i < 9; i++)
  {
    SerialUSB.print(i);
    SerialUSB.print(": ");
    SerialUSB.println(imu_data[i]);
  }
}
