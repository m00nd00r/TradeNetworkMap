#include "LPD8806.h"
#include "Adafruit_WS2801.h"
#include "SPI.h"

uint8_t nLEDsLightStrand1 = 54;

uint8_t dpLM1 = 4;
uint8_t cpLM1 = 5;

Adafruit_WS2801 lightStrand1 = Adafruit_WS2801(nLEDsLightStrand1, dpLM1, cpLM1);

uint8_t firstLight1 = 1;
uint8_t strand1 = 1;
uint8_t lastLight1 = nLEDsLightStrand1 - 1;
uint8_t pointLight1 = nLEDsLightStrand1;

uint32_t colorStrands;

void setup(){
  lightStrand1.begin();
  
  lightStrand1.show();
  
  colorStrands = Color(255, 255, 255);
}

void loop(){
  illuminatePixel(colorStrands, strand1, firstLight1);
  delay(5000);
  
  lightMarch(colorStrands, strand1);
  
  illuminatePixel(colorStrands, 0, 0);
  delay(2000);
}

uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// Illuminate point light in light strands:
void illuminatePixel(uint32_t c, uint8_t numStrand, uint8_t pixel) {
  uint8_t i;
  pixel = pixel - 1;
  switch(numStrand){
    case 0:
      for(i = 0; i < lightStrand1.numPixels(); i++)
       lightStrand1.setPixelColor(i, 0);
      lightStrand1.show();
      break;
      
    case 1:
      lightStrand1.setPixelColor(pixel,c);
      lightStrand1.show();
      break;
  }
}

void lightMarch(uint32_t c, uint8_t numStrand) {
  int i;
  switch(numStrand){
    case 1:
      // Turn all pixels off from to last for the light march portion:
      for(i = firstLight1; i < lastLight1; i++) lightStrand1.setPixelColor(i, 0);
      
      // Turn on each pixel and wait 96 milliseconds to turn on the next so that the march executes over 5 seconds
      for(i = firstLight1; i < lastLight1; i++){
        lightStrand1.setPixelColor(i, c);
        lightStrand1.show();
        delay(96);
      }
      
      // Turn off all the pixels at the same time except for the first one and the last one
      for(i = firstLight1; i < lastLight1 - 1; i++){
        lightStrand1.setPixelColor(i, 0);
      }
      lightStrand1.show();
      
      break;
  }
}
