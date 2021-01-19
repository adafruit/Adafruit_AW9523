#include <Adafruit_AW9523.h>

Adafruit_AW9523 aw;

uint8_t LedPin = 0;  // 0 thru 15

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);  // wait for serial port to open
  
  Serial.println("Adafruit AW9523 Constant Current LED test!");

  if (! aw.begin(0x58)) {
    Serial.println("AW9523 not found? Check wiring!");
    while (1) delay(10);  // halt forever
  }

  Serial.println("AW9523 found!");
  aw.pinMode(LedPin, AW9523_LED_MODE); // set to constant current drive!
}


uint8_t x = 0;

void loop() {
  // Loop from 0 to 255 and then wrap around to 0 again
  aw.analogWrite(LedPin, x++);
  delay(10);
}