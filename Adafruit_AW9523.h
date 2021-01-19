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

#define AW9523_LED_MODE 0x3

#define AW9523_REG_CHIPID 0x10
#define AW9523_REG_SOFTRESET 0x7F
#define AW9523_REG_INPUT0 0x00
#define AW9523_REG_OUTPUT0 0x02
#define AW9523_REG_CONFIG0 0x04
#define AW9523_REG_INTENABLE0 0x06
#define AW9523_REG_GCR 0x11
#define AW9523_REG_LEDMODE 0x12


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
  bool openDrainPort0(bool od);

  // All 16 pins at once
  bool outputGPIO(uint16_t pins);
  uint16_t inputGPIO(void);
  bool configureDirection(uint16_t pins);
  bool configureLEDMode(uint16_t pins);
  bool interruptEnableGPIO(uint16_t pins);

  // Individual pin control
  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, bool val);
  bool digitalRead(uint8_t pin);
  void analogWrite(uint8_t pin, uint8_t val);
  void enableInterrupt(uint8_t pin, bool en);

protected:
  uint16_t _pin_outputs = 0;
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
};

#endif
