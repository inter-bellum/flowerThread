#include "ModuleCluster.h"

ModuleCluster::ModuleCluster(){}

ModuleCluster::~ModuleCluster(){
  free(byteValues);
  free(intValues);
}

ModuleCluster::ModuleCluster(Arduino_h::byte address, int numModules){
  this->address = address;
  this->numModules = numModules;
  this->numBytes = numModules * 6;
  this->numValues = numModules * 3;
  //each module has 3 pots, each of which has 2 bytes (an int) of data
  this->byteValues = (Arduino_h::byte*) calloc(numBytes, sizeof(Arduino_h::byte));
  this->intValues = (uint16_t*) calloc(numValues, sizeof(uint16_t));
  this->valueArrIndex = 0;
}

void ModuleCluster::read(){
  Wire.requestFrom(address, numBytes);

  valueArrIndex = 0;
  while (Wire.available()){
    Arduino_h::byte in = Wire.read();
    byteValues[valueArrIndex] = in;
    valueArrIndex++;
  }

  parseBytes();
}


void ModuleCluster::parseBytes(){
  for (int i = 0; i < numValues; i++){
    intValues[i] = byteValues[i] << 8 | byteValues[i+1];
  }
}

void ModuleCluster::print(){
  for (int i = 0; i < numValues; i++){
    Serial.print(intValues[i]);
    Serial.print(", ");
  }
  Serial.println();
}
