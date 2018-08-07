/*
 * Standby Mode Take II
 * 
 */

 /* ***********************************************
  *  IMU SETUP
  * ***********************************************
  */

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <Arduino.h>
#include <ax25.h>
#include "sd_log.h"
#include "SD_DataFile.h"
#include <SD.h>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;
sd_log mySd_log;


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
// TODO @ZAC is this fine... does this matter? do we want to dynamically update based on interpolated lat/long values

  /* ***********************************************
  *  END IMU SETUP
  * ***********************************************
  */

const int battery_pin = 0;
int current_log = 0;
int iteration = 0;
int n = 5;
float threshold = 2.5;
float v = 0.000;

void setup() 
{
  
  pinMode(battery_pin, INPUT);
  SerialUSB.begin(115200);

  delay(4000);
  
  SerialUSB.println("Alive");

  init_imu(); 

  SD.begin(SPI_CS_SD);

  pinMode(SPI_CS_SD, OUTPUT);

  mySd_log.sd_init();
  
}

void loop()
{

  SerialUSB.println("**********************************");
  SerialUSB.print("This is iteration number: ");
  SerialUSB.println(iteration++);
  /* 
   *  1) Sleep. 
   *  2) Wake up after set amount of seconds.
   *  3) Check Battery Level
   *  4) If above threshold, move on
   */
  
  do
  {
    // TODO: edit the sleep - wake after one minute code
    // Go to sleep @connor
    SerialUSB.println("Sleeping for 4 seconds...");
    delay(4000);
    SerialUSB.println("Awake");
    
    // TODO: make sure this is correct, I am not really convinced I am doing this correctly
    // We need to move on in the flightcode
    // check out this function definition far far far below to see if it makes sense. 
    // I tooled around with it till it gave me a nice number so we could continue writing flight code
    v = read_battery();
    
    SerialUSB.print("Threshold: ");
    SerialUSB.print(threshold);
    SerialUSB.println("v");
    SerialUSB.print("Battery Voltage: ");
    SerialUSB.print(v);
    SerialUSB.println("v");
    
  }while(v < threshold);

  SerialUSB.println("Waking up and starting my day");
  
  // TODO: gather sensor data @connor
  // sensors include the GPS, IMU, & science sensors from XLAB!!!
  delay(1000);


  /*
   * IMU routine
   */
  // IMU = { gx, gy, gz, ax, ay, az, mx, my, mz}
  float imu_data[9] = {0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000};
  
  if(imu.gyroAvailable())
  {
    imu.readGyro();
    imu_data[0] = imu.calcGyro(imu.gx);
    imu_data[1] = imu.calcGyro(imu.gy);
    imu_data[2] = imu.calcGyro(imu.gz);
  }
  if(imu.accelAvailable())
  {
    imu.readAccel();
    imu_data[3] = imu.calcAccel(imu.ax);
    imu_data[4] = imu.calcAccel(imu.ay);
    imu_data[5] = imu.calcAccel(imu.az);
  }
  if(imu.magAvailable())
  {
    imu.readMag();
    imu_data[6] = imu.calcMag(imu.mx);
    imu_data[7] = imu.calcMag(imu.my);
    imu_data[8] = imu.calcMag(imu.mz);
  }

  SerialUSB.println("IMU data: ");
  // WRITE this new data to the SD card... 
  for(int i = 0; i < 9; i++)
  {
    SerialUSB.print(i);
    SerialUSB.print(": ");
    SerialUSB.println(imu_data[i]);
  }

// Write a new log to the SD
// SD STUFF GOES HERE
// TODO: @emma write a new log with the updated stuff!

// TODO: @emma

  
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
  
  
  current_log++;
}

/* 
 *  Cool function to make initializing the IMU easy in the setup function!!
 */
void init_imu()
{
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    SerialUSB.println("Failed to communicate with LSM9DS1.");
    SerialUSB.println("Double-check wiring.");
    SerialUSB.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
    {
      // stay here
      SerialUSB.println("Imu did not init");
      delay(5000);
    }
  }
}

/*
 * Read battery function for reading the battery voltage!
 */
float read_battery()
{
  int val = analogRead(battery_pin); // Read Voltage
  SerialUSB.println(val);
  v = val*(416.0/110.0)*(3.3/1024); // Convert to Volts
  return v;
}

/*
 * Serial prints the heading, does not send through radio... 
 * In practice, save this to a string, use the ax25 function to convert to a nice ax25 packet
 * then use rh_rf22 to send this out using the radio
 */
bool heading_chirp(int in_log, sd_log my_log)
{
  byte buf[6];
  mySd_log.read_header(in_log, buf);
  SerialUSB.println("Header data from struct");
  SerialUSB.print("log_num: ");
  SerialUSB.println(mySd_log.logData.log_num);
  SerialUSB.print("status_byte: ");
  SerialUSB.println(mySd_log.logData.status_byte);
  SerialUSB.print("power_data[0]: ");
  SerialUSB.println(mySd_log.logData.power_data[0]);
  SerialUSB.print("power_data[1]: ");
  SerialUSB.println(mySd_log.logData.power_data[1]);
  SerialUSB.print("power_data[2]: ");
  SerialUSB.println(mySd_log.logData.power_data[2]);
}

/*
 * OUTPUT menu of commands
 */
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

/*
 * takes in input and returns 
 */
int return_selection()
{
  int incoming_command = 70;
  
  while(!SerialUSB.available())
  {
    SerialUSB.println("Enter a command");
    delay(1500);
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
 * SD STUFF
 */
//
// 
//  /*
//   * SD magic!!
//   * Sets the following:
//    int log_num;
//    byte status_byte;
//    byte power_data[3];
//    float GPS_data[3];
//    float IMU_data[9];
//    byte command_data[8];
//   */
//  SerialUSB.println("Updating log variables...");
//  mySd_log.sd_init();
//  mySd_log.logData.log_num = current_log;
//  mySd_log.logData.status_byte = 'l';
//  
//  mySd_log.logData.power_data[0] = 'l';
//  mySd_log.logData.power_data[1] = 'o';
//  mySd_log.logData.power_data[2] = 'l';
//  
//  mySd_log.logData.GPS_data[0] = 12.345;
//  mySd_log.logData.GPS_data[1] = 67.891;
//  mySd_log.logData.GPS_data[2] = 45.678;
//  mySd_log.logData.GPS_data[3] = 45.998;
//  
//  for(int i = 0; i < 9; i++)
//  {
//    mySd_log.logData.IMU_data[i] = imu_data[i];
//  }
//  mySd_log.logData.command_data[0] = 'r';
//  mySd_log.logData.command_data[1] = 'a';
//  mySd_log.logData.command_data[2] = 'l';
//  mySd_log.logData.command_data[3] = 'e';
//  mySd_log.logData.command_data[4] = 'n';
//  mySd_log.logData.command_data[5] = 's';
//  mySd_log.logData.command_data[6] = 'u';
//  mySd_log.logData.command_data[7] = 'x';
//  
//  delay(1000);
//  SerialUSB.println("Vos Chifres sont pres monsieur...");
//
//  SerialUSB.println("Writing SD data to the sd card...");
//  mySd_log.write_log(mySd_log.logData);
//
//  delay(1000);
//
//  SerialUSB.println("finito, don micheli!...");
//  
//
//  /* 
//   *  Send out a basic chirp with the heading of the most recent log
//   *  In practice, save this to a string, use the ax25 function to convert to a nice ax25 packet
//   *  then use rh_rf22 to send this out using the radio
//   */
//  byte buf[6];
//  mySd_log.read_header(current_log, buf);
//  SerialUSB.println("Header data from struct");
//  SerialUSB.print("log_num: ");
//  SerialUSB.println(mySd_log.logData.log_num);
//  SerialUSB.print("status_byte: ");
//  SerialUSB.println(mySd_log.logData.status_byte);
//  SerialUSB.print("power_data[0]: ");
//  SerialUSB.println(mySd_log.logData.power_data[0]);
//  SerialUSB.print("power_data[1]: ");
//  SerialUSB.println(mySd_log.logData.power_data[1]);
//  SerialUSB.print("power_data[2]: ");
//  SerialUSB.println(mySd_log.logData.power_data[2]);
