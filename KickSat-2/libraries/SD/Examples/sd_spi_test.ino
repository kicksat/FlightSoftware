////TEST for sd_log_test function
#include "sd_log.h"
#include "SD_DataFile.h"
#include <SD.h>

int n = 0;
sd_log mySd_log;
void setup(){
  SerialUSB.begin(9600);
  delay(2000);
  SD.begin(10);

  pinMode(SPI_CS_SD, OUTPUT);
  mySd_log.sd_init();

  mySd_log.logData = {0, (byte)'a', {(byte)'v', (byte)'v', (byte)'v'}, {3.14159, 5454.54, 1111.1}, {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3}, {(byte)'c', (byte)'o', (byte)'m', (byte)'m', (byte)'a', (byte)'n', (byte)'d', (byte)'!'}};
  mySd_log.write_log(mySd_log.logData);

}

void loop(){
 mySd_log.sd_init();
 mySd_log.logData.log_num = n;
 mySd_log.write_log(mySd_log.logData);
 SerialUSB.print("Wrote Log ");
 SerialUSB.println(mySd_log.logData.log_num);
 SerialUSB.println(mySd_log.read_entry(n));
 n++;
 delay(1000);
 mySd_log.sd_end();
}
