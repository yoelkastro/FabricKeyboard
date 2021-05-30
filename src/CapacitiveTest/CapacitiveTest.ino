#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CapacitiveSensor.h>

#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, 4);

CapacitiveSensor css[] = {CapacitiveSensor(9, 10), CapacitiveSensor(9, 11), CapacitiveSensor(9, 12)};

int results[] = {0, 0};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  delay(200);
  display.clearDisplay();

  int vals[3];
  for(int i = 0; i < 3; i ++){
    vals[i] = css[i].capacitiveSensor(10);
    display.setCursor(10, 20 * i);
    display.println(vals[i]);

    if(vals[i] > 200){
      if(results[0] == 0){
        results[0] = i + 1;
      }
      else if(results[1] == 0 && (i + 1) != results[0]){
        results[1] = i + 1;
      }
      else if(i + 1 != results[1]){
        results[0] = results[1];
        results[1] = i + 1;
      }
    }
  }
  
  int dif = results[1] - results[0];
  String toPrint = "N/A";
  if(dif == 1 || dif == -2)
    toPrint = "f";
  else if(dif == -1 || dif == 2)
    toPrint = "b";
    
  display.setCursor(74, 0);
  display.println(toPrint);
  display.display();

}
