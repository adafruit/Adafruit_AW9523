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

  configureDirection(0x0); // all inputs!
  openDrainPort0(false); // push pull default

  return true;
}

bool Adafruit_AW9523::reset(void) {
  Adafruit_I2CRegister resetreg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_SOFTRESET);
  return resetreg.write(0);
}


bool Adafruit_AW9523::outputGPIO(uint16_t pins) {
  Adafruit_I2CRegister output0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_OUTPUT0, 2, LSBFIRST);
  if (! output0reg.write(pins)) {
    return false;
  }
  _pin_outputs = pins;

  return true;
}

uint16_t Adafruit_AW9523::inputGPIO(void) {
  Adafruit_I2CRegister input0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_INPUT0, 2, LSBFIRST);
  return input0reg.read();
}

bool Adafruit_AW9523::configureDirection(uint16_t pins) {
  Adafruit_I2CRegister conf0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_CONFIG0, 2, LSBFIRST);
  if (! conf0reg.write(~pins)) {
    return false;
  }

  return true;
}

bool Adafruit_AW9523::configureLEDMode(uint16_t pins) {
  Adafruit_I2CRegister ledmodereg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_LEDMODE, 2, LSBFIRST);
  if (! ledmodereg.write(~pins)) {
    return false;
  }

  return true;
}

void Adafruit_AW9523::analogWrite(uint8_t pin, uint8_t val) {
  uint8_t reg;

  // See Table 13. 256 step dimming control register
  if ((pin >= 0) && (pin <= 7)) {
    reg = 0x24 + pin;
  } 
  if ((pin >= 8) && (pin <= 11)) {
    reg = 0x20 + pin - 8;
  }     
  if ((pin >= 12) && (pin <= 15)) {
    reg = 0x2C + pin - 12;
  } 

  Adafruit_I2CRegister ledCCreg = Adafruit_I2CRegister(i2c_dev, reg);

  ledCCreg.write(val);
}

void Adafruit_AW9523::digitalWrite(uint8_t pin, bool val) {
  Adafruit_I2CRegister output0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_OUTPUT0, 2, LSBFIRST);
  Adafruit_I2CRegisterBits outbit =
      Adafruit_I2CRegisterBits(&output0reg, 1, pin); // # bits, bit_shift

  outbit.write(val);
}

bool Adafruit_AW9523::digitalRead(uint8_t pin) {
  Adafruit_I2CRegister input0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_INPUT0, 2, LSBFIRST);
  Adafruit_I2CRegisterBits inbit =
      Adafruit_I2CRegisterBits(&input0reg, 1, pin); // # bits, bit_shift

  return inbit.read();
}


void Adafruit_AW9523::enableInterrupts(uint8_t pin, bool en) {
  Adafruit_I2CRegister intenablereg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_INTENABLE0, 2, LSBFIRST);
  Adafruit_I2CRegisterBits irqbit =
      Adafruit_I2CRegisterBits(&intenablereg, 1, pin); // # bits, bit_shift

  irqbit.write(!en);
}



void Adafruit_AW9523::pinMode(uint8_t pin, uint8_t mode) {
  // GPIO Direction
  Adafruit_I2CRegister conf0reg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_CONFIG0, 2, LSBFIRST);
  Adafruit_I2CRegisterBits confbit =
      Adafruit_I2CRegisterBits(&conf0reg, 1, pin); // # bits, bit_shift

  // GPIO mode or LED mode?
  Adafruit_I2CRegister ledmodereg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_LEDMODE, 2, LSBFIRST);
  Adafruit_I2CRegisterBits modebit =
    Adafruit_I2CRegisterBits(&ledmodereg, 1, pin); // # bits, bit_shift

  if (mode == OUTPUT) {
    confbit.write(0);
    modebit.write(1);
  }
  if (mode == INPUT) {
    confbit.write(1);
    modebit.write(1);
  }

  if (mode == AW9523_LED_MODE) {
    confbit.write(0);
    modebit.write(0);
  }

}

bool Adafruit_AW9523::openDrainPort0(bool od) {
  Adafruit_I2CRegister gcrreg = Adafruit_I2CRegister(i2c_dev, AW9523_REG_GCR, 1);

  Adafruit_I2CRegisterBits opendrain =
      Adafruit_I2CRegisterBits(&gcrreg, 1, 4); // # bits, bit_shift

  return opendrain.write(!od);
}
