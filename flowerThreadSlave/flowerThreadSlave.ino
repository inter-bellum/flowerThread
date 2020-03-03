//This sketch is used to communicate the data from multiple potmeters to a teensy using I2C
#include <Wire.h>
#include "Module.h"

enum ColorIndex{
  RED,
  GREEN,
  BLUE
};

int analogPotPins[3] = {2, 3, 6};

int colorXMin[3] = {59, 68, 127};
int colorXMax[3] = {139, 117, 121};
int colorYMin[3] = {59, 68, 127};
int colorYMax[3] = {43, 94, 74};

#define NUM_PIN_POTS  3
#define NUM_MUX_POTS  6
#define NUM_POTS      9
#define NUM_MODULES   3
#define MUX_PIN_1     0
#define MUX_PIN_2     1

Pot pots[NUM_POTS];
Module modules[NUM_MODULES];

byte potValues[NUM_POTS * 2];

void setup() {  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
#ifndef DEBUG_SIG
  //change for each arduino
  Wire.begin(0x00);
  Wire.onRequest(requestHandler);
#endif

  Serial.begin(9600);

  pots[0] = Pot(0, 0, 0, IT_MUX, potValues);
  pots[1] = Pot(1, 0, 1, IT_MUX, potValues);
  pots[2] = Pot(2, 0, 2, IT_MUX, potValues);

  Pot* mod1[3] = {&pots[0], &pots[1], &pots[2]};
  
  pots[3] = Pot(0, 1, 3, IT_MUX, potValues);
  pots[4] = Pot(1, 1, 4, IT_MUX, potValues);
  pots[5] = Pot(2, 1, 5, IT_MUX, potValues);

  Pot* mod2[3] = {&pots[3], &pots[4], &pots[5]};

  pots[6] = Pot(2, 6, IT_PIN, potValues);
  pots[7] = Pot(2, 7, IT_PIN, potValues);
  pots[8] = Pot(2, 8, IT_PIN, potValues);

  Pot* mod3[3] = {&pots[6], &pots[7], &pots[8]};

//  modules[0] = Module(0, mod1);
}

void loop() {

  for (int i = 0; i < 3; i++){
    pots[i].read();
    pots[i+3].readNoPortSelect();
  }
  Serial.println();

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

uint32_t interpolateColorSpace(int xIn, int yIn){
  float x = xIn / 1023.;
  float y = yIn / 1023.;
  
  
  byte r = interpolateColor(RED, x, y);
  byte g = interpolateColor(GREEN, x, y);
  byte b = interpolateColor(BLUE, x, y);

  return (uint32_t) (g << 16) | (r << 8) | b;
}

byte interpolateColor(ColorIndex c, float x, float y){
  return byte(((colorXMax[c] * x) + (colorXMin[c] * (1 - x)) + (colorYMax[c] * y) + (colorYMin[c] * (1 - y))) / 4.);
}
