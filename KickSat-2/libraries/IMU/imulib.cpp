#include <imulib.h>

///////////////////
// IMU FUNCTIONS //
///////////////////

float return_gx(LSM9DS1 imu) // Calculates gyroscope, x-component
{
  imu.readGyro();
  return imu.calcGyro(imu.gx);
}

float return_gy(LSM9DS1 imu) // Calculates gyroscope, y-component
{
  imu.readGyro();
  return imu.calcGyro(imu.gy);
}

float return_gz(LSM9DS1 imu) // Calculates gyroscope, z-component
{
  imu.readGyro();
  return imu.calcGyro(imu.gz);
}

float return_ax(LSM9DS1 imu) // Calculates accelerometer, x-component
{
  imu.readAccel();
  return imu.calcAccel(imu.ax);
}

float return_ay(LSM9DS1 imu) // Calculates accelerometer, y-component
{
  imu.readAccel();
  return imu.calcAccel(imu.ay);
}

float return_az(LSM9DS1 imu) // Calculates accelerometer, z-component
{
  imu.readAccel();
  return imu.calcAccel(imu.az);
}

float return_mx(LSM9DS1 imu) // Calculates magnetometer, x-component
{
  imu.readMag();
  return imu.calcMag(imu.mx);
}

float return_my(LSM9DS1 imu) // Calculates magnetometer, y-component
{
  imu.readMag();
  return imu.calcMag(imu.my);
}

float return_mz(LSM9DS1 imu) // Calculates magnetometer, z-component
{
  imu.readMag();
  return imu.calcMag(imu.mz);
}

float return_roll() // Calculates roll
{
  float roll = atan2(return_ay(), return_az());
  roll  *= 180.0 / PI;
  return roll;
}

float return_pitch() // Calculates pitch
{
  float pitch = atan2(-return_ax(), sqrt(return_ay() * return_ay() + return_az() * return_az()));
  pitch *= 180.0 / PI;
  return pitch;
}

float return_yaw() // Calculates yaw
{
  float ax1 = return_ax();
  float ay1 = return_ay();
  float az1 = return_az();
  float mx1 = return_mx();
  float my1 = return_my();
  float mz1 = return_mz();

  float roll = atan2(ay1, az1);
  float pitch = atan2(-ax1, sqrt(ay1 * ay1 + az1 * az1));

  float yaw;
  if (my1 == 0)
    yaw = (mx1 < 0) ? 180.0 : 0;
  else
    yaw = atan2(mx1, my1);

  yaw -= DECLINATION * PI / 180;

  if (yaw > PI) yaw -= (2 * PI);
  else if (yaw < -PI) yaw += (2 * PI);
  else if (yaw < 0) yaw += 2 * PI;

  // Convert everything from radians to degrees:
  yaw *= 180.0 / PI;

  return yaw;
}

void init_imu(LSM9DS1 imu) // Initialize the IMU
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
  }
}

///////////////////////
// END IMU FUNCTIONS //
///////////////////////
