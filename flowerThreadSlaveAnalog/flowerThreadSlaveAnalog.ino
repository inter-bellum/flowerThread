//This sketch is used to communicate the data from multiple potmeters to a teensy using I2C
#include <Wire.h>
#include "Module.h"
#include <Adafruit_NeoPixel.h>

//set adc prescaler to 16
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//end set adc prescaler to 16

Adafruit_NeoPixel strip;
Adafruit_NeoPixel strip2;

int analogPotPins[6] = {0, 1, 2, 3, 6, 7};

#define NUM_POTS      6
#define NUM_MODULES   2

#define NUM_LEDS      10

Pot pots[NUM_POTS];
Module modules[NUM_MODULES];

byte potValues[NUM_POTS * 2];

void setup() {

  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;

  strip = Adafruit_NeoPixel(NUM_LEDS, 5, NEO_GRB + NEO_KHZ800);
  strip2 = Adafruit_NeoPixel(NUM_LEDS, 6, NEO_GRB + NEO_KHZ800);

#ifndef DEBUG_SIG
  //change for each arduino
  Wire.begin(0x01);
  Wire.onRequest(requestHandler);
#endif

#ifdef DEBUG_POT_PLOT
  Serial.begin(9600);
#endif

  for (int i = 0; i < NUM_POTS; i++){
    pots[i] = Pot(analogPotPins[i], i, IT_PIN, potValues);
  }

  Pot* mod1[3] = {&pots[0], &pots[1], &pots[2]};

  Pot* mod2[3] = {&pots[3], &pots[4], &pots[5]};

  modules[0] = Module(0, mod1, &strip);
  modules[1] = Module(1, mod2, &strip2);
}

void loop() {
  modules[0].read(modules[1]);
  for (int i = 0; i < NUM_MODULES; i++){
    modules[i].updateColor();
  }
  #ifdef DEBUG_POT_PLOT
  Serial.println();
  #endif

#ifdef DEBUG_SIG
  int val1 = analogRead(A4);
  int val2 = analogRead(A5);

  Serial.print("SDA:");
  Serial.print(val1);
  Serial.print(",");
  Serial.print("SCK:");
  Serial.println(val2);
#endif
}

void requestHandler(){
  Wire.write(potValues, NUM_POTS * 2);
}
