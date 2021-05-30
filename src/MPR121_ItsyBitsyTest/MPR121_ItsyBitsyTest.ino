/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
#define NUM_CAPS 3

Adafruit_MPR121 caps[NUM_CAPS];

/*Adafruit_MPR121 cap = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();
*/
// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {

  for(int i = 0; i < NUM_CAPS; i ++){
    caps[i] = Adafruit_MPR121();
    caps[i].begin(0x5A + i);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  /*cap.begin(0x5B);
  cap2.begin(0x5C);
  cap3.begin(0x5A);*/
}

void getTouches(bool touches[], uint16_t reading){
  for(int i = 0; i < 12; i ++){
    touches[i] = reading & _BV(i);
  }
}

void loop() {
  // Get the currently touched pads
  /*bool vals1[12];
  bool vals2[12];
  bool vals3[12];
  getTouches(vals1, cap.touched());
  getTouches(vals2, cap2.touched());
  getTouches(vals3, cap3.touched());*/

  bool vals[NUM_CAPS][12];
  for(int i = 0; i < NUM_CAPS; i ++){
    getTouches(vals[i], caps[i].touched());
  }
  
  if(vals[0][0] || vals[1][11] || vals[2][5]){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}
