#include <Wire.h>
#include "MAX17320.h"

//uint8_t r_sense = 1;
void setup() {
  Wire.begin(DEVICE_ADDRESS);
  Wire.begin(NVM_ADDRESS);
  Serial.begin(9600);
}

void loop() {
  //  read_cell1();
  delay(1000);
  //  Serial.println(MAX17320::read_cell1());
  MAX17320::set_cell_balancing(ZenerEnable, mV50, 11.7, 100);
}
