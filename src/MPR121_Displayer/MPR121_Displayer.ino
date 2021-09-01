
#include "Adafruit_MPR121.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

#define SCREEN_ADDRESS 0x3C
#define NUM_CAPS 4

Adafruit_MPR121 caps[NUM_CAPS];
Adafruit_SSD1306 display(128, 64, &Wire, 4);

void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();

  for(int i = 0; i < NUM_CAPS; i ++){
    caps[i] = Adafruit_MPR121();
    caps[i].begin(0x5A + i);
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void getTouches(bool touches[], uint16_t reading){
  for(int i = 0; i < 12; i ++){
    touches[i] = reading & _BV(i);
  }
}

void loop() {

  display.clearDisplay();
  
  bool vals[NUM_CAPS][12];
  for(int i = 0; i < NUM_CAPS; i ++){
    getTouches(vals[i], caps[i].touched());

    for(int j = 0; j < 12; j ++){
      if(vals[i][j]){
        display.fillRect((display.width() / 12) * (12 - j), (display.height() / NUM_CAPS) * i, (display.width() / 16), (display.width() / 16), SSD1306_WHITE);
        Serial.println(i);
      }
      else
        display.drawRect((display.width() / 12) * (12 - j), (display.height() / NUM_CAPS) * i, (display.width() / 16), (display.width() / 16), SSD1306_WHITE);
    }
  }
  display.display();
 
}
