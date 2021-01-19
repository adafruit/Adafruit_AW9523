/*!
 *  @file Adafruit_AW9523.cpp
 *
 *  @mainpage Adafruit AW9523 Battery Monitor library
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Adafruit AW9523 Battery Monitor library
 *
 * 	This is a library for the Adafruit AW9523 breakout:
 * 	https://www.adafruit.com/product/4712
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *  @section author Author
 *
 *  Limor Fried (Adafruit Industries)
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include "Arduino.h"

#include "Adafruit_AW9523.h"

/*!
 *    @brief  Instantiates a new AW9523 class
 */
Adafruit_AW9523::Adafruit_AW9523(void) {}

Adafruit_AW9523::~Adafruit_AW9523(void) {}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_AW9523::begin(uint8_t addr, TwoWire *wire) {
  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(addr, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  if (!reset()) {
    return false;
  }

  Adafruit_I2CRegister idreg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_CHIPID);
  if (idreg.read() != 0x23) {
    return false;
  }

  return true;
}

bool Adafruit_AW9523::reset(void) {
  Adafruit_I2CRegister resetreg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_SOFTRESET);
  return resetreg.write(0);
}
