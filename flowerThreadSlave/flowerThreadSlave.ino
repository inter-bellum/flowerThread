//This sketch is used to communicate the data from multiple potmeters to a teensy using I2C
#include <Wire.h>
#include "Module.h"

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip;
Adafruit_NeoPixel stri2p;

#define NUM_POTS      9
#define NUM_MODULES   3

#define NUM_LEDS      10

Pot pots[NUM_POTS];
Module modules[NUM_MODULES];

byte potValues[NUM_POTS * 2];

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  strip = Adafruit_NeoPixel(NUM_LEDS, 5, NEO_GRB + NEO_KHZ800);

#ifndef DEBUG_SIG
  //change for each arduino
  Wire.begin(0x00);
  Wire.onRequest(requestHandler);
#endif

  Serial.begin(9600);

  pots[0] = Pot(0, 0, potValues);
  pots[1] = Pot(1, 1, potValues);
  pots[2] = Pot(2, 2, potValues);

  Pot* mod1[3] = {&pots[0], &pots[1], &pots[2]};

  pots[3] = Pot(3, 3, potValues);
  pots[4] = Pot(6, 4, potValues);
  pots[5] = Pot(7, 5, potValues);

  Pot* mod2[3] = {&pots[3], &pots[4], &pots[5]};

  pots[6] = Pot(8, 6, potValues);
  pots[7] = Pot(9, 7, potValues);
  pots[8] = Pot(10, 8, potValues);

  Pot* mod3[3] = {&pots[6], &pots[7], &pots[8]};

  modules[0] = Module(0, mod1, &strip);
  modules[1] = Module(1, mod2, &stri2p);
  modules[2] = Module(2, mod3, &stri2p);
}

void loop() {
  modules[0].read(modules[1]);
  modules[2].read();
  modules[0].updateColor();

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
