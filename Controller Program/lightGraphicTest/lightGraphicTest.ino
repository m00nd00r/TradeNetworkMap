#include "LPD8806.h"
#include "Adafruit_WS2801.h"
#include "SPI.h"

uint8_t nLEDsGraphic = 120;

uint8_t dpLG1 = 2;
uint8_t cpLG1 = 3;

uint8_t graphic1 = 1;
  uint8_t graphic2 = 2;
  uint8_t graphic3 = 3;
  uint8_t lightGraphic1First = 1;
  uint8_t lightGraphic1Last = 40;
  uint8_t lightGraphic2First = 41;
  uint8_t lightGraphic2Last = 80;
  uint8_t lightGraphic3First = 81;
  uint8_t lightGraphic3Last = 120;
  uint8_t strand1 = 1;
  uint8_t strand2 = 2;
  uint8_t strand3 = 3;
  uint8_t strand2And3 = 2;
  uint8_t firstLight1 = 1;
  uint8_t firstLight2 = 1;
  uint8_t firstLight3 = 1; 

LPD8806 lightGraphic = LPD8806(nLEDsGraphic, dpLG1, cpLG1);

void setup(){
  lightGraphic.begin();
  
  lightGraphic.show();
}

void loop(){
   illuminateGraphic(lightGraphic.Color(127,127,31), 1);
   delay(5000);
   illuminateGraphic(lightGraphic.Color(127,127,31), 0);
   delay(5000);
}

void illuminateGraphic(uint32_t c, uint8_t numGraphic){
  uint8_t i;     // loop counter

  switch(numGraphic){
    case 0:
      // Bring all graphics low
      for(i = 0; i < lightGraphic3Last; i++){
        lightGraphic.setPixelColor(i, 0);
      }
      lightGraphic.show();
      break;
      
    case 1:
      // Make sure all pixels are off:
      for(i = lightGraphic1First - 1; i < lightGraphic1Last; i++) lightGraphic.setPixelColor(i,0);
  
      // Now turn them all on at once:
      for(i = lightGraphic1First - 1; i < lightGraphic1Last; i++){
        lightGraphic.setPixelColor(i, c);
      }
      lightGraphic.show();
      break;
      
    case 2:
      // Make sure all pixels are off:
      for(i = lightGraphic2First - 1; i < lightGraphic2Last; i++) lightGraphic.setPixelColor(i,0);
  
      // Now turn them all on at once:
      for(i = lightGraphic2First - 1; i < lightGraphic2Last; i++){
        lightGraphic.setPixelColor(i, c);
      }
      lightGraphic.show();
      break;
      
    case 3:
      // Make sure all pixels are off:
      for(i = lightGraphic3First - 1; i < lightGraphic3Last; i++) lightGraphic.setPixelColor(i,0);
  
      // Now turn them all on at once:
      for(i = lightGraphic3First - 1; i < lightGraphic3Last; i++){
        lightGraphic.setPixelColor(i, c);
      }
      lightGraphic.show();
      break;
  }
}

