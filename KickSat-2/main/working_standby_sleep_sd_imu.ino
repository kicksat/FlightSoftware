// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "sd_log.h"
#include <SD.h>
#include <ax25.h>
#include "RTCCounter.h"

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;
sd_log mySd_log;
Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object


///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.


int current_log = 0;
int iteration = 0;
int n = 5;
float threshold = 2.5;
float v = 0.000;

void watchdog() {
  SerialUSB.println("Watchdog");
}


void setup(){
  SerialUSB.begin(115200);
  delay(3000);

  init_imu();

  mySd_log.sd_init();

  watchdogTimer.init(1,watchdog); // timer delay, seconds
  beaconTimer.init(10); // timer delay, seconds

}

void loop()
{

  if (beaconTimer.update()) { // Checks time for interrupt
    SerialUSB.println("Beacon things!");
  }

 float batt_level = read_battery();

 read_IMU(mySd_log.logData.IMU_data);


 mySd_log.sd_refresh();
 mySd_log.logData.log_num = current_log;
 mySd_log.write_log(mySd_log.logData);
 SerialUSB.print("Wrote Log ");
 SerialUSB.println(mySd_log.logData.log_num);
 SerialUSB.println(mySd_log.read_entry(current_log));
 delay(1000);
 mySd_log.sd_end();

 current_log++;
 sleepTimer.sleep(); // Go into sleep mode until next interrupt


 // TODO: read the most recent header from the SD card @emma

  String header_chirp = "I'm doing fine. Thanks for checking, earthling";
// TODO: @emma
  serial_transmit(header_chirp);

// END of SD Stuff

  // output the command menu
  output_menu(); // simulates the ground station menu of commands which will be nice and GUI

  int command = 70;

  command = return_selection();

  SerialUSB.print("command num: ");
  SerialUSB.println(command);

  int logs_to_send = 0;      // for command 2
  String multiple_logs = ""; // for command 2
  String config_string = "";

  // respond to the command
  switch(command)
  {
    // Send down Sensor data
    case 1:
      send_sensor_data();
      break;
    // Downlink last x number of logs, depends on user input
    case 2:
      // Get some user input for the number of logs to send down
      // Normally this o
      SerialUSB.println("How many logs shall we send down?");
      logs_to_send = return_selection();
      SerialUSB.print("Fetching the last ");
      SerialUSB.print(logs_to_send);
      SerialUSB.println(" logs");

      // TODO: @emma read the last logs_to_send logs into the multiple_logs

      serial_transmit(multiple_logs);

      break;

    // Rewrite the sensor config files @connor @max
    case 3:
      SerialUSB.println("Doing command #3");
      // TODO: make a function that does this...
      break;

    // Reflash the motherboard's code from MRAM @connor @max
    case 4:
      // TODO: make a function that does this...
      SerialUSB.println("Doing command #4");
      break;

    // Send the mission config files
    case 5:
      // TODO: @emma
      // read the mission config files into config_string =
      serial_transmit(config_string);
      SerialUSB.println("Doing command #5");
      break;

    // Enter arming mode, exit standby mode
    case 6:
      SerialUSB.println("Doing command #6");
      // TODO: enter arming mode
      // send: "Entered arming mode"
      // This is a transition condition in the more general state diagram
      // we will exit standby mode here
      break;

    // Enter End of Life mode
    case 7:
      SerialUSB.println("Doing command #7");
      // Send: ACK, are you sure you want to explode all of our hard work into pixie dust?
      serial_transmit("Are you sure you want to kill KickSat II? (y/n)");
      // wait for response: if (response != "yes explode" ) --> go back into standby mode
      command = return_selection();
      SerialUSB.print("case 7, command: ");
      SerialUSB.println(command);
      if((command == 41) || (command == 73))
      {
        // go into end of life mode
        // this is a state change into end of life mode
        SerialUSB.println("Killing the goddamn 3u that never deserved to live MUAHAHAHAHA!");
      }
      else
      {
        // do not kill the satelite

        command = 0;
      }

      break;

    // No command --> go back to sleep and go through another standby mode loop
    case 8:
    //
      SerialUSB.println("Doing no command... going to sleep");
      break;

    // No command --> go back to sleep and go through another standby mode loop
    default:
      SerialUSB.println("Doing no command... going to sleep");
      break;
  }




}

void output_menu()
{
  // Listen for Uplink:
  SerialUSB.println("Choose a command from the following menu: ");
  SerialUSB.println("1) Send Sensor Data");
  SerialUSB.println("2) Downlink last 10 Logs");
  SerialUSB.println("3) Rewrite the sensor config files");
  SerialUSB.println("4) Reflash the motherboard's code from MRAM");
  SerialUSB.println("5) Send Mission Config Files");
  SerialUSB.println("6) Enter Arming Mode");
  SerialUSB.println("7) Enter End of Life Mode");
  SerialUSB.println("8) No command");
  SerialUSB.print("Choose a command: ");
}


void read_IMU(float buf[]){
  float gyroData[3];
  float accelData[3];
  float magData[3];
  getGyro(gyroData);
  getAccel(accelData);
  getMag(magData);
  buf[0] = gyroData[0];
  buf[1] = gyroData[1];
  buf[2] = gyroData[2];
  buf[3] = accelData[0];
  buf[4] = accelData[1];
  buf[5] = accelData[2];
  buf[6] = magData[0];
  buf[7] = magData[1];
  buf[8] = magData[2];
}

void getGyro(float buf[])
{
 // To read from the gyroscope, you must first call the
 // readGyro() function. When this exits, it'll update the
 // gx, gy, and gz variables with the most current data.
 imu.readGyro();

 // Now we can use the gx, gy, and gz variables as we please.
 // Either print them as raw ADC values, or calculated in DPS.
 SerialUSB.print("G: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcGyro helper function to convert a raw ADC value to
 // DPS. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcGyro(imu.gx), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcGyro(imu.gy), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcGyro(imu.gz), 2);
 buf[0] = imu.calcGyro(imu.gx);
 buf[1] = imu.calcGyro(imu.gy);
 buf[2] = imu.calcGyro(imu.gz);
#elif defined PRINT_RAW
 SerialUSB.print(imu.gx);
 SerialUSB.print(", ");
 SerialUSB.print(imu.gy);
 SerialUSB.print(", ");
 SerialUSB.println(imu.gz);
 buf[0] = imu.gx;
 buf[1] = imu.gy;
 buf[2] = imu.gz;
#endif
}

void getAccel(float buf[])
{
 // To read from the accelerometer, you must first call the
 // readAccel() function. When this exits, it'll update the
 // ax, ay, and az variables with the most current data.
 imu.readAccel();

 // Now we can use the ax, ay, and az variables as we please.
 // Either print them as raw ADC values, or calculated in g's.
 SerialUSB.print("A: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcAccel helper function to convert a raw ADC value to
 // g's. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcAccel(imu.ax), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcAccel(imu.ay), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcAccel(imu.az), 2);
 buf[0] = imu.calcAccel(imu.ax);
 buf[1] = imu.calcAccel(imu.ay);
 buf[2] = imu.calcAccel(imu.az);
#elif defined PRINT_RAW
 SerialUSB.print(imu.ax);
 SerialUSB.print(", ");
 SerialUSB.print(imu.ay);
 SerialUSB.print(", ");
 SerialUSB.println(imu.az);
 buf[0] = imu.ax;
 buf[1] = imu.ay;
 buf[2] = imu.az;
#endif

}

void getMag(float buf[])
{
 // To read from the magnetometer, you must first call the
 // readMag() function. When this exits, it'll update the
 // mx, my, and mz variables with the most current data.
 imu.readMag();

 // Now we can use the mx, my, and mz variables as we please.
 // Either print them as raw ADC values, or calculated in Gauss.
 SerialUSB.print("M: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcMag helper function to convert a raw ADC value to
 // Gauss. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcMag(imu.mx), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcMag(imu.my), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcMag(imu.mz), 2);
 buf[0] = imu.calcMag(imu.mx);
 buf[1] = imu.calcMag(imu.my);
 buf[2] = imu.calcMag(imu.mz);
#elif defined PRINT_RAW
 SerialUSB.print(imu.mx);
 SerialUSB.print(", ");
 SerialUSB.print(imu.my);
 SerialUSB.print(", ");
 SerialUSB.println(imu.mz);
 buf[0] = imu.mx;
 buf[1] = imu.my;
 buf[2] = imu.mz;
#endif
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
 float roll = atan2(ay, az);
 float pitch = atan2(-ax, sqrt(ay * ay + az * az));

 float heading;
 if (my == 0)
   heading = (mx < 0) ? PI : 0;
 else
   heading = atan2(mx, my);

 heading -= DECLINATION * PI / 180;

 if (heading > PI) heading -= (2 * PI);
 else if (heading < -PI) heading += (2 * PI);
 else if (heading < 0) heading += 2 * PI;

 // Convert everything from radians to degrees:
 heading *= 180.0 / PI;
 pitch *= 180.0 / PI;
 roll  *= 180.0 / PI;

 SerialUSB.print("Pitch, Roll: ");
 SerialUSB.print(pitch, 2);
 SerialUSB.print(", ");
 SerialUSB.println(roll, 2);
 SerialUSB.print("Heading: "); SerialUSB.println(heading, 2);
}

/*
*  Cool function to make initializing the IMU easy in the setup function!!
*/
void init_imu() {
 // Before initializing the IMU, there are a few settings
 // we may need to adjust. Use the settings struct to set
 // the device's communication mode and addresses:
 imu.settings.device.commInterface = IMU_MODE_I2C;
 imu.settings.device.mAddress = LSM9DS1_M;
 imu.settings.device.agAddress = LSM9DS1_AG;
 // The above lines will only take effect AFTER calling
 // imu.begin(), which verifies communication with the IMU
 // and turns it on.
 if (!imu.begin()) {
   SerialUSB.println("Failed to communicate with LSM9DS1.");
   SerialUSB.println("Double-check wiring.");
   SerialUSB.println("Default settings in this sketch will " \
                 "work for an out of the box LSM9DS1 " \
                 "Breakout, but may need to be modified " \
                 "if the board jumpers are.");
   while (1) {
     // stay here
     SerialUSB.println("Imu did not init");
     delay(5000);
   }
 }
}


/*
 * takes in input and returns
 */
int return_selection()
{
  int incoming_command = 70;

  SerialUSB.println("Enter a command");
  while(!SerialUSB.available())
  {
    delay(1000);
  }

  if(SerialUSB.available() > 0)
  {
    SerialUSB.println("");
    // read the incoming byte:
    incoming_command = SerialUSB.read();      // reads the available char
    incoming_command = incoming_command - 48; // ASCII to int
    // say what you got:
    SerialUSB.print("I received command #");
    SerialUSB.println(incoming_command, DEC);
  }

  return incoming_command;
}


// handles command one to send down the sensor data
void send_sensor_data()
{
  while(1)
  {
    float command1_threshold = 2.055; // TODO: measure the actual battery level needed to complete the sensor downlink
    v = read_battery(); // TODO: make this function correct... it does not work as is (board A)
    SerialUSB.print("CMD 1 Threshold: ");
    SerialUSB.println(command1_threshold);
    SerialUSB.print("Battery Voltage: ");
    SerialUSB.println(v);
    if(v < command1_threshold)
    {
      SerialUSB.println("Going to sleep...");
      // TODO: go to sleep for 30 seconds
      SerialUSB.println("Awake!");
    }
    else
    {
      SerialUSB.println("Battery is charged enough to send sensor info");
      break;
    }
  }

  // out of the while(1) loop, send the sensor data

  // TODO: Read sensor data from SD card @ Emma
  // TODO: @max @connor --> do we want to send another command for which sensor to send data on at the top of this sensor
  // this way the function would begin with listening for which sensor number to send data about

  // Send down the sensor data via Serial (radio)
  String sensor_data_string = "Rad: 759 kRad";
  // translate to ax25 packet;
  serial_transmit(sensor_data_string);

}

void serial_transmit(String to_send)
{
  SerialUSB.print("Send via Radio: '");
  SerialUSB.print(to_send);
  SerialUSB.println("'");
}

/*
* Read battery function for reading the battery voltage!
*/
float read_battery() {
//  int batt_val = analogRead(VBATT);
//  float batt_voltage = batt_val * (3.3 / 1023.0) * (426.0/110.0);
//  SerialUSB.println("VBATT: ");
//  SerialUSB.println(batt_voltage);
 return 5;
}
