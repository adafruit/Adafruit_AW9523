/*!
 *  @file Adafruit_AW9523.h
 *
 * 	I2C Driver for the Adafruit AW9523 GPIO expander
 *
 * 	This is a library for the Adafruit AW9523 breakout:
 * 	https://www.adafruit.com/products/4886
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_AW9523_H
#define _ADAFRUIT_AW9523_H

#include "Arduino.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>

#define AW9523_DEFAULT_ADDR 0x58
#define AW9523_REG_CHIPID 0x10
#define AW9523_REG_SOFTRESET 0x7F

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the AW9523 I2C GPIO expander
 */
class Adafruit_AW9523 {
public:
  Adafruit_AW9523();
  ~Adafruit_AW9523();

  bool begin(uint8_t address = AW9523_DEFAULT_ADDR, TwoWire *wire = &Wire);
  bool reset(void);

protected:
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
};

#endif
