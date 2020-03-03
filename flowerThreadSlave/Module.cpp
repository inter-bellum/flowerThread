#include "Module.h"

extern int colorXMin[3] = {59, 68, 127};
extern int colorXMax[3] = {139, 117, 121};
extern int colorZMin[3] = {59, 68, 127};
extern int colorZMax[3] = {43, 94, 74};

//extern int colorXMin[3] = {200, 0, 0};
//extern int colorXMax[3] = {0, 200, 0};
//extern int colorZMin[3] = {0, 200, 0};
//extern int colorZMax[3] = {0, 0, 200};

extern int ledPins[3] = {5, 6, 9};

Module::Module(int index, Pot* _pots[3], Adafruit_NeoPixel* strip){
  this->index = index;
  this->ledPin = ledPins[index];
  memcpy(this->pots, _pots, sizeof(Pot*) * 3);
  this->strip = strip;
  this->strip->begin();
  this->strip->show();
  this->strip->setBrightness(10);
}


void Module::read(){
  for (int i = 0; i < 3; i++){
    pots[i]->read();
  }
}

void Module::read(Module &otherMod){
  for (int i = 0; i < 3; i++){
    pots[i]->read();
    otherMod.read(i);
  }

}

void Module::read(int i){
  pots[i]->readNoPortSelect();
}



void Module::updateColor(){
//  bool shouldUpdateColor = false;
//  for (int i = 0; i < 3; i++){
//    if (pots[i]->available()) shouldUpdateColor = true;
//  }
//
//  if (!shouldUpdateColor) return;
  
  int x = pots[0]->getValue();
  float y = (pots[1]->getValue()) / 102.3;
  int z = pots[2]->getValue();
  
  interpolateColorSpace(x, y, z);
}

void Module::interpolateColorSpace(int xIn, float y, int zIn){
  float x = xIn / 1023.;
  float z = zIn / 1023.;
  int floorY = floor(y);
 
  uint8_t r = interpolateColor(RED, x, z);
  uint8_t g = interpolateColor(GREEN, x, z);
  uint8_t b = interpolateColor(BLUE, x, z);

  Serial.print(r);
  Serial.print('\t');
  Serial.print(g);
  Serial.print('\t');
  Serial.println(b);

  
  for (int i = 0; i < ledCount / 2; i++){
    float diff = y - i;
    int mirror = ledCount - i - 1;
    
    if (diff > 1){
      uint32_t c = strip->Color(r, g, b);
      strip->setPixelColor(i, c);
      strip->setPixelColor(mirror, c);
    } else 
    if (diff > 0){
      uint32_t c = strip->Color(r * diff, g * diff, b * diff);
      strip->setPixelColor(i, c);
      strip->setPixelColor(mirror, c);
    } else {
      strip->setPixelColor(i, 0);
      strip->setPixelColor(mirror, 0);
    }
  }
  strip->show();
}

uint8_t Module::interpolateColor(ColorIndex c, float x, float z){
  return uint8_t(((colorXMax[c] * x) + (colorXMin[c] * (1 - x)) + (colorZMax[c] * z) + (colorZMin[c] * (1 - z))) / 2.);
}
