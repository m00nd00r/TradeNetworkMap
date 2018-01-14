/********************************************************************************************************************/
/********************************************************************************************************************/
/*                                                                                                                  */
/* Field Museum of Natural History - Exhibitions Department - Interactives Division                                 */
/*                                                                                                                  */
/* LED Light Program for Chocolate Abroad 2013 - Section E - Trade Network Map                                      */
/*                                                                                                                  */
/* Created by John Kinstler - May 2013                                                                              */
/*                                                                                                                  */
/********************************************************************************************************************/
/********************************************************************************************************************/
/*                                                                                                                  */
/* There are 3 illuminated graphics using the LPD8806 controlled light                                              */
/* strips from Adafruit.com. There are also 3 contiguous light strips that                                          */
/* incorporate all point lights as well as marching lights using the 12mm led                                       */
/* pixel strands using the WS2801 led controller from Adafruit.com as well.                                         */
/* These will be controlled using the Arduino Uno R3 controller board.                                              */
/*                                                                                                                  */
/********************************************************************************************************************/
/********************************************************************************************************************/
/*                                                                                                                  */
/* The algortithm for this light show is as follows:                                                                */
/* 1) @ T0 bring lightGraphic1 and firstLight1 high for length of program                                           */
/* 2) @ T6 bring lightMarch1 high for 5 seconds                                                                     */
/* 3) @ T11 bring lightMarch1 low;                                                                                  */
/*          firstLight1 and lightGraphic1 remain high;                                                              */
/*          bring lastLight2 and lightGraphic2 high                                                                 */
/* 4) @ T18 bring lastLight3, lastLight2, pointLight2, pointLight1,                                                 */
/*          pointLight3 high in succession over 3 seconds                                                           */
/*          Note: each pointLight and lastLight are connected to each of the                                        */ 
/*                lightStrands as indicated by their number                                                         */
/* 5) @ T23 bring firstLight2 and firstLight3 high;                                                                 */
/*          bring lightGraphic3 high                                                                                */
/* 6) @ T31 bring lightMarch1 high for 5 seconds then bring low                                                     */
/* 7) @ T32 bring lightMarch2 and lightMarch3 high for 5 seconds then bring low                                     */
/* 8) @ T38 bring lightGraphic1, lightGraphic2, lightGraphic3, firstLight1,                                         */
/*          lastLight1, firstLight2, lastLight2, firstLight3, lastLight3,                                           */
/*          pointLight1, pointLight2, and pointLight3 low simultaneously                                            */
/* 9) @ T40 end program                                                                                             */
/*                                                                                                                  */
/********************************************************************************************************************/
/********************************************************************************************************************/
/*                                                                                                                  */
/*                             Wiring Diagram                                                                       */
/*                                                                                                                  */
/* The controller was constructed using the Arduino Uno R3 with an attached                                         */
/* Screw Shield. The Screw Shield was also purchased from Adafruit.com and assembled in                             */
/* house. The Screw Shield is mounted to the Arduino using the soldered pin                                         */
/* connectors. The screw terminals are used to connect power input and data lines to                                */
/* the LEDs.                                                                                                        */
/*                                                                                                                  */
/*            -----------------------------------------------------------------                                     */
/*            |w   o     -----     screw shield                      o    gnd |                                     */
/*            |x   o     | ■ |                                      o    gnd  |                                    */
/*            |y   o     -----                gnd terminal           o    aref|                                     */
/*            |z   o  ѻѻ reset                    block              o    gnd |                                     */
/*            |rst o  ѻѻ                       ┌─────┐          o     13 |                                     */
/*            |3v  o  ѻѻ                       │ Ф  -  □│-------|  o     12 |                                     */
/*            |5v  o  ѻѻ                       │ Ф  -  □│-----| |  o pwm 11 |                                     */
/*            |gnd o  ѻѻ  5v terminal          │ Ф  -  □│---| | |  o pwm 10 |                                     */
/*            |vin o  ѻѻ    block              │ Ф  -  □│-| | | |  o pwm  9 |------------ clock ┐                */
/*            |                                └─────┘ | | | |  o      8 |------------ data  │ lightStrand 3  */                        
/*            |A0  o  ѻѻ ┌─────┐               ѻѻ      | | | |-----------|------------ gnd   │                */
/*            |A1  o  ѻѻ │ Ф  +  □│-----------------------+-+-+-------------|------------ 5v    ┘                */
/*            |A2  o  ѻѻ │ Ф  +  □│--------------------|  | | |-------------|------------ gnd   ┐                */
/*            |A3  o  ѻѻ │ Ф  +  □│-----|         ѻѻ   |--+-+---------------|------------ 5v    │                */
/*            |A4  o  ѻѻ │ Ф  +  □│-|   |         ѻѻ      | |      o      7 |------------ data  │ lightStrand 2  */          
/*            |A5  o  ѻѻ └─────┘ |   |         ѻѻ      | |      o pwm  6 |------------ clock ┘                */
/*            |gnd o  ѻѻ              |   |--------------|  | |---------------|------------ gnd   ┐                */
/*            |gnd o  ѻѻ              |             ѻѻ   |--+-----------------|------------ 5v    │                */
/*            |     gnd|5v            |             ѻѻ      |        o pwm  5 |------------ clock │                */
/*            |                       |             ѻѻ      |        o      4 |------------ data  ┘ lightStrand 1  */
/*            |                       |             ѻѻ      |        o pwm  3 |------------ clock ┐                */
/*            |                       |             ѻѻ      |        o      2 |------------ data  │ lightGraphics  */
/*            |                       |             ѻѻ      |        o     tx |       |---- gnd   │                */
/*            |                       |           gnd|5v    |        o     rx |       | |-- 5v    ┘                */
/*            -----------------------------------------------------------------       | |                           */
/*                                    |             ||      |                         | |                           */
/*           p                        |             ||      |                         | |                           */
/*           o                        |             ||      |                         | |                           */
/*           w - |--------------------+-------------++------|                         | |                           */
/*           e + |--------------------|             ||                                | |                           */
/*           r                                      ||                                | |                           */
/*           1                                      ||                                | |                           */
/*           p                                      ||                                | |                           */
/*           o - |----------------------------------||                                | |                           */
/*           w - |-----------------------------------+--------------------------------| |                           */
/*           e + |-----------------------------------|                                  |                           */
/*           r + |----------------------------------------------------------------------|                           */
/*           2                                                                                                      */
/*                                                                                                                  */
/*                                                                                                                  */
/*                                                                                                                  */
/********************************************************************************************************************/
/********************************************************************************************************************/




// libraries necessary to run the led controllers

#include "LPD8806.h"
#include "Adafruit_WS2801.h"
#include "SPI.h"


/*********************************************************************************************************************/


// These first variables are the number of LEDs in various elements. The first is for all the illuminated graphics.
// The light strand led numbers may seem arbitrary but they're not. The difference in leds was needed to make 
// the spacing look as similar as possible across all three different paths as well as to add length between the
// lastLights and pointLights for each strand.
// The first one is simply the longer of the three. The lights are each about 1/2" in diameter. 
// They have been spaced out about a 1/2" apart as well. 
// In addition to being shorter, the other two strands cross the first one.
// This crossing means that the two strands have to share an led with the first, longer one. 
// Also, at these crossings there is some bunching that occurs, so I've spaced the leds out and tightened where 
// I could to get them to look more or less even at these crossings. Since the first strand is the one being 
// crossed, I decided to let that strand contain the shared leds. This necessarily decreases the number for 
// the other two strands. I will try to establish a light march where they all arrive to the end of the 
// strands in more or less the same number of seconds. 

uint16_t nLEDsGraphic = 120;
uint16_t nLEDsLightStrand1 = 56;
uint16_t nLEDsLightStrand2 = 47;
uint16_t nLEDsLightStrand3 = 47;

// Choose and initialize a data pin and clock pin for each graphic element and each light strip
// data pin = dp; clock pin = cp; lightGraphic = lg; light march = lm
// The order of the pins is specific to the Arduino Uno R3 board layout. All of the pins are digital. The
// clock pins use the digital pins that have analog output capability whose numbering is labeled on the 
// board itself. This is why the ordering looks odd.

uint8_t dpLG = 2;
uint8_t cpLG = 3;
uint8_t dpLM1 = 4;
uint8_t cpLM1 = 5;
uint8_t dpLM2 = 7;
uint8_t cpLM2 = 6;
uint8_t dpLM3 = 8;
uint8_t cpLM3 = 9;

// Object defintion for illuminated graphics. All the graphics will be connected in series on the same data and clock
// pins, since the programming for them is the same. 
// Graphic 1 will use pixels 1 - 40; Graphic 2, pixels 41-80; Graphic 3, pixels 81-120.
// First parameter is the number of LEDs. Next two parameters are the data and clock pins:
LPD8806 lightGraphic = LPD8806(nLEDsGraphic, dpLG, cpLG);

// Object definition for light marches. First parameter is the number of LEDs in the strand.
// Next two parameters are the data and clock pins:
Adafruit_WS2801 lightStrand1 = Adafruit_WS2801(nLEDsLightStrand1, dpLM1, cpLM1);
Adafruit_WS2801 lightStrand2 = Adafruit_WS2801(nLEDsLightStrand2, dpLM2, cpLM2);
Adafruit_WS2801 lightStrand3 = Adafruit_WS2801(nLEDsLightStrand3, dpLM3, cpLM3);

// setup various point lights. The three separate point lights 1,2,3 will be part of lightStrands 1,2,3 and activated separately
int graphic1 = 1;
int graphic2 = 2;
int graphic3 = 3;
int lightGraphic1First = 1;
int lightGraphic1Last = 40;
int lightGraphic2First = 81;
int lightGraphic2Last = 120;
int lightGraphic3First = 41;
int lightGraphic3Last = 80;
int strand1 = 1;
int strand2 = 2;
int strand3 = 3;
int allStrands = 2;
uint16_t firstLight1 = 0;
uint16_t firstLight2 = 0;
uint16_t firstLight3 = 0; 
uint16_t lastLight1 = nLEDsLightStrand1 - 4;
uint16_t lastLight2 = nLEDsLightStrand2 - 2;
uint16_t lastLight3 = nLEDsLightStrand3 - 3;
uint16_t pointLight1 = nLEDsLightStrand1 - 1;
uint16_t pointLight2 = nLEDsLightStrand2 - 1;
uint16_t pointLight3 = nLEDsLightStrand3 - 1;

 // Set color for graphics:
 uint32_t colorGraphics = lightGraphic.Color(127,127,31); // These LED strips use a 7 bit color gamut
                                                        // This method call appends all the color info into one 24 bit number
                                                        // to be used by the LED ICs
 
 // Set color for strands:
 uint32_t colorStrands = Color(255, 255, 63);             // These LED strands use an 8 bit color gamut
                                                        // This function call appends all the color info into one 24 bit number
                                                        // to be used by the LED ICs
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
// Setup is used for all initializations required for execution of the program
void setup() {
  // Start up the illuminated graphic panels
  lightGraphic.begin();
  
  // Start up the light strands
  lightStrand1.begin();
  lightStrand2.begin();
  lightStrand3.begin();

  // Update all the lights, to start they are all 'off', this command pushes data to strips
  lightGraphic.show();
  lightStrand1.show();
  lightStrand2.show();
  lightStrand3.show();
  
  //Serial.begin(9600);  
}






// Loop is the main program body
void loop() {
 
  // T0: turn on lightGraphic1 and firstLight1 in lightMarch1. All leds on at the same time.
  illuminateGraphic(colorGraphics, graphic1);
  illuminatePixel(colorStrands, strand1, firstLight1);
  delay(5000);
  
  // T6: initiate lightMarch1. Complete in 5 seconds; turn off; leave firstLight1 and lastLight1 on.
  lightMarch(colorStrands, strand1);
  
  // T11: Turn on lightGraphic2
  illuminateGraphic(colorGraphics, graphic2);
  delay(7000);
  //illuminateGraphic(0, 0);
  
  // T18: bring lastLight2, lastLight3, pointLight1, pointLight2, pointLight3 high in order over 3 seconds
  illuminatePixel(colorStrands, strand3, lastLight3);
  delay(750);
  illuminatePixel(colorStrands, strand2, lastLight2);
  delay(750);
  illuminatePixel(colorStrands, strand2, pointLight2);
  delay(750);
  illuminatePixel(colorStrands, strand1, pointLight1);
  delay(750);
  illuminatePixel(colorStrands, strand3, pointLight3);
  delay(3000);

  // T23: bring firstLight2 and firstLight3 high; bring lightGraphic3 high 
  illuminatePixel(colorStrands, strand2, firstLight2);
  illuminatePixel(colorStrands, strand3, firstLight3);
  illuminateGraphic(colorGraphics, graphic3);
  delay(8000);
  //Serial.println("check1");
 
  // T31: bring lightMarch1 high for 5 seconds then bring low
  lightMarch(colorStrands, allStrands);
  //Serial.println("check2");
  delay(1000);
  
  // T38: bring lightGraphic1, lightGraphic2, lightGraphic3, firstLight1, lastLight1, firstLight2, lastLight2, firstLight3, 
  //      lastLight3, pointLight1, pointLight2, and pointLight3 low
  illuminateGraphic(colorGraphics, 0);
  illuminatePixel(colorStrands, 0, 0);
    //Serial.println("check3");
  delay(2000);
  // T40: end program 
}







// The following functions are called by various elements within Loop



// Illuminate a graphic panel. Input is the light color in GRB as well as which one of the 3 is to be brought high:
void illuminateGraphic(uint32_t c, int numGraphic){
  int i;     // loop counter

  switch(numGraphic){
    case 0:
      // Bring all graphics low
      for(i = 0; i < nLEDsGraphic; i++){
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





// Create a 24 bit color value from R,G,B
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
void illuminatePixel(uint32_t c, int numStrand, uint16_t pixel) {
  int i;
  switch(numStrand){
    // Turn off all point lights
    case 0:
      for(i = 0; i < 57; i++){
       lightStrand1.setPixelColor(i, 0);
       lightStrand2.setPixelColor(i, 0);
       lightStrand3.setPixelColor(i, 0);
      }
      lightStrand1.show();
      lightStrand2.show();
      lightStrand3.show();
      break;
    
    // Turn on pixels for lightStrand1  
    case 1:
      lightStrand1.setPixelColor(pixel,c);
      lightStrand1.show();
      break;
      
    // Turn on pixels for lightStrand2
    case 2:
      lightStrand2.setPixelColor(pixel,c);
      lightStrand2.show();
      break;
      
    // Turn on pixels for lightStrand3
    case 3:
      lightStrand3.setPixelColor(pixel,c);
      lightStrand3.show();
      break;
  }
}







// Marching lights routine for showing the three different light paths across the ocean. Each strand should illuminate over
// 5 seconds from beginning to end and then turn off leaving the first light and last light on.
void lightMarch(uint32_t c, int numStrand) {
  int i = 0;
  int j = 0;
  int time = 0;
  switch(numStrand){
    case 1:
      // Turn on each pixel and wait 96 milliseconds to turn on the next so that the march executes over 5 seconds
      for(i > firstLight1; i <= lastLight1; i++){
        lightStrand1.setPixelColor(i, c);
        lightStrand1.show();
        delay(96);
      }
      
      // Turn off all the pixels at the same time except for the first one and the last one
      for(i = firstLight1 + 1; i < lastLight1; i++){
        lightStrand1.setPixelColor(i, 0);
      }
      lightStrand1.show();
      
      break;
    
    // Since Strands 2 and 3 only come on together and at the same time, couple them both into the same light show so they come on at the 
    // same time. Since they have different lengths, I have to do a little fancy footwork with logic. I didn't realize this would be a problem
    // before I had already cut all the parts and mounting boards. This arose as I was writing the code. The while block inside the third for loop
    // is meant to prevent strand 3 from illuminating it's last light, since it has one less light than strand 2.
    
    case 2:
      // Turn on each pixel for lightStrand1 and wait 96 milliseconds to turn on the next so that the march executes over 5 seconds
      
      for(i = 0; i < 70; i++){
        if((i > firstLight1) && (i < lastLight1)){
          lightStrand1.setPixelColor(i, c);
          lightStrand1.show();
        }
        if(i >= 13){
          if((j >= firstLight2) && (j < lastLight2)){
            lightStrand2.setPixelColor(j, c);
            lightStrand2.show();
          }
          if((j >= firstLight3) && (j < lastLight3)){
            lightStrand3.setPixelColor(j, c);
            lightStrand3.show();
          }
          j++;
        }
      delay(96);
      }
      break;
  }
}


