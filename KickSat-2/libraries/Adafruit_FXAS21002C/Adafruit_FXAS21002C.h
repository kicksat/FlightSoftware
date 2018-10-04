/*!
 * @file Adafruit_FXAS21002C.h
 *
 * This is part of Adafruit's FXAS21002C driver for the Arduino platform.  It
 * is designed specifically to work with the Adafruit FXAS21002C breakout:
 * https://www.adafruit.com/products/3463
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Kevin "KTOWN" Townsend for Adafruit Industries.
 *
 * MIT license, all text here must be included in any redistribution.
 *
 */
#ifndef __FXAS21002C_H__
#define __FXAS21002C_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_Sensor.h>
#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS AND SETTINGS
    -----------------------------------------------------------------------*/
    /** 7-bit address for this sensor */
    #define FXAS21002C_ADDRESS       (0x21)       // 0100001
    /** Device ID for this sensor (used as a sanity check during init) */
    #define FXAS21002C_ID            (0xD7)       // 1101 0111
    /** Gyroscope sensitivity at 250dps */
    #define GYRO_SENSITIVITY_250DPS  (0.0078125F) // Table 35 of datasheet
    /** Gyroscope sensitivity at 500dps */
    #define GYRO_SENSITIVITY_500DPS  (0.015625F)
    /** Gyroscope sensitivity at 1000dps */
    #define GYRO_SENSITIVITY_1000DPS (0.03125F)
    /** Gyroscope sensitivity at 2000dps */
    #define GYRO_SENSITIVITY_2000DPS (0.0625F)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    /*!
        Raw register addresses used to communicate with the sensor.
    */
    typedef enum
    {
      GYRO_REGISTER_STATUS              = 0x00, /**< 0x00 */
      GYRO_REGISTER_OUT_X_MSB           = 0x01, /**< 0x01 */
      GYRO_REGISTER_OUT_X_LSB           = 0x02, /**< 0x02 */
      GYRO_REGISTER_OUT_Y_MSB           = 0x03, /**< 0x03 */
      GYRO_REGISTER_OUT_Y_LSB           = 0x04, /**< 0x04 */
      GYRO_REGISTER_OUT_Z_MSB           = 0x05, /**< 0x05 */
      GYRO_REGISTER_OUT_Z_LSB           = 0x06, /**< 0x06 */
      GYRO_REGISTER_WHO_AM_I            = 0x0C, /**< 0x0C (default value = 0b11010111, read only) */
      GYRO_REGISTER_CTRL_REG0           = 0x0D, /**< 0x0D (default value = 0b00000000, read/write) */
      GYRO_REGISTER_CTRL_REG1           = 0x13, /**< 0x13 (default value = 0b00000000, read/write) */
      GYRO_REGISTER_CTRL_REG2           = 0x14, /**< 0x14 (default value = 0b00000000, read/write) */
    } gyroRegisters_t;
/*=========================================================================*/

/*=========================================================================
    OPTIONAL SPEED SETTINGS
    -----------------------------------------------------------------------*/
    /*!
        Enum to define valid gyroscope range values
    */
    typedef enum
    {
      GYRO_RANGE_250DPS  = 250,     /**< 250dps */
      GYRO_RANGE_500DPS  = 500,     /**< 500dps */
      GYRO_RANGE_1000DPS = 1000,    /**< 1000dps */
      GYRO_RANGE_2000DPS = 2000     /**< 2000dps */
    } gyroRange_t;
/*=========================================================================*/

/*=========================================================================
    RAW GYROSCOPE DATA TYPE
    -----------------------------------------------------------------------*/
    /*!
        Struct to store a single raw (integer-based) gyroscope vector
    */
    typedef struct gyroRawData_s
    {
      int16_t x;    /**< Raw int16_t value for the x axis */
      int16_t y;    /**< Raw int16_t value for the y axis */
      int16_t z;    /**< Raw int16_t value for the z axis */
    } gyroRawData_t;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  Unified sensor driver for the Adafruit FXAS21002C breakout.
*/
/**************************************************************************/
class Adafruit_FXAS21002C : public Adafruit_Sensor
{
  public:
    Adafruit_FXAS21002C(int32_t sensorID = -1);

    bool begin           ( gyroRange_t rng = GYRO_RANGE_250DPS );
    bool getEvent        ( sensors_event_t* event);
    void getSensor       ( sensor_t* sensor);

    /*! Raw gyroscope values from last sensor read */
    gyroRawData_t raw;

  private:
    void        write8  ( byte reg, byte value );
    byte        read8   ( byte reg );
    gyroRange_t _range;
    int32_t     _sensorID;
};

#endif
