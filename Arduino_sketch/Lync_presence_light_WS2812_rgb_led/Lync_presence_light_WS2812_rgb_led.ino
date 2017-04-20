/* Lync presence light
 */

#include <Adafruit_NeoPixel.h>  // lib for WS2812 LEDs

#include "SuperMarioMelody.h"

//define NeoPixel Pin and Number of LEDs
#define NEO_PIN 2
#define NEO_NUM_LEDS 8
#define BRIGHTNESS 30

//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_NUM_LEDS, NEO_PIN, NEO_GRB + NEO_KHZ800);

unsigned char redValue = 0; // value to write to the red LED
unsigned char  greenValue = 0; // value to write to the green LED
unsigned char  blueValue = 0; // value to write to the blue LED

unsigned char  newSerialStream = false;
unsigned char commandValue = 0;

#define LDR_PIN 1   // analog input pin for LDR
#define BUZZER_PIN 4

int SensorLDRReading = 0;

#define LED_PIN LED_BUILTIN // the number of the LED pin


char ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated

unsigned long NeoLEDpreviousMillis = 0; 
int NeoLEDcount = 0;

const unsigned int interval = 1000;           // interval at which to blink (milliseconds)
unsigned int NeoLEDinterval = 40;           // interval (milliseconds)

float nextPause;

void setup() {
  Serial.begin(9600);  // initialize serial
  pinMode(LED_PIN, OUTPUT);  // set the digital pin as output
//  pinMode(NEO_PIN, OUTPUT); .. done by strip.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  
  // start the strip and blank it out
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    SensorLDRReading = analogRead(LDR_PIN);
    
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
//      redValue = 255;
    }
    else {
      ledState = LOW;
//      redValue = 0;
    }

    // set the LED with the ledState of the variable:
   // digitalWrite(LED_PIN, ledState);

// blink test
//    strip.setPixelColor(2, redValue, 50, 50);
//    strip.show(); // This sends the updated pixel color to the hardware
    
  }

  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int SredValue = Serial.parseInt();
    // do it again for the other values
    int SgreenValue = Serial.parseInt();
    int SblueValue = Serial.parseInt();
    int ScommandValue = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255
      redValue = constrain(SredValue, 0, 255);
      greenValue = constrain(SgreenValue, 0, 255);
      blueValue = constrain(SblueValue, 0, 255);
      commandValue = constrain(ScommandValue, 0, 255);
       
      newSerialStream = true;
    }
  }
  
    if (newSerialStream == true) {
      // update LEDs:

      unsigned long currentMillis = millis();
      
      if (currentMillis - NeoLEDpreviousMillis >= NeoLEDinterval) {
        if (commandValue == 3)
          NeoLEDinterval = 300;
        else
          NeoLEDinterval = 10;
        
        NeoLEDpreviousMillis = currentMillis;
        
        // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one        
        // for (int i = 0; i < NEO_NUM_LEDS; i++) {
        if (NeoLEDcount < NEO_NUM_LEDS) {
          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          strip.setPixelColor(NeoLEDcount, strip.Color(redValue, greenValue, blueValue));
          // strip.setPixelColor(0, strip.Color(200, 0, 0));
          strip.show(); // This sends the updated pixel color to the hardware
 //       delay(60); // Delay for a period of time (in milliseconds)
          NeoLEDcount++;
         }
         else {
          //strip.show(); // This sends the updated pixel color to the hardware
          NeoLEDcount = 0;
          newSerialStream = false;
                    
      // print the three numbers in one string as hexadecimal:
      Serial.print(redValue, HEX);
      Serial.print(greenValue, HEX);
      Serial.println(blueValue, HEX);
          
        }
      }
    }
    
  
  if (commandValue == 51) {
    nextPause = moveToNextStepSong(BUZZER_PIN, 1.2);
    delay(nextPause);
  }
  else {
    noTone(BUZZER_PIN);
  }
}

