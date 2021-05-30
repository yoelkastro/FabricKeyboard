
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

bool vals[12];
Adafruit_MPR121 caps = Adafruit_MPR121();

void setup() {
  caps.begin(0x5A + 1);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(0), getTouches, FALLING);
}

void getTouches(){
  digitalWrite(LED_BUILTIN, HIGH);
  uint16_t reading = caps.touched();
  for(int i = 0; i < 12; i ++){
    vals[i] = reading & _BV(i);
  }
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {

  digitalWrite(LED_BUILTIN, vals[5]);
 
}
