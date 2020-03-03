#include "ModuleCluster.h"

ModuleCluster::ModuleCluster(){}

ModuleCluster::~ModuleCluster(){
  free(byteValues);
  free(intValues);
}

ModuleCluster::ModuleCluster(Arduino_h::byte address, int numModules, Arduino_h::HardwareSerial* ser){
  this->address = address;
  this->numModules = numModules;
  this->numBytes = numModules * 6;
  this->numValues = numModules * 3;
  //each module has 3 pots, each of which has 2 bytes (an int) of data
  this->byteValues = (Arduino_h::byte*) calloc(numBytes, sizeof(Arduino_h::byte));
  this->intValues = (uint16_t*) calloc(numValues, sizeof(uint16_t));
  this->valueArrIndex = 0;
  this->startTime = millis();
  this->ser = ser;
}

void ModuleCluster::read(){
//  ser->println("start read");
  Wire.requestFrom(address, numBytes);
//  ser->println("got data, parsing:");

  valueArrIndex = 0;
  while (Wire.available()){
    Arduino_h::byte in = Wire.read();
//    ser->print("byte: ");
//    ser->print(in);
//    ser->print("\t");
//    if (valueArrIndex % 2 == 0) ser->println();
    byteValues[valueArrIndex] = in;
    valueArrIndex++;
  }
//  Serial.println();

  parseBytes();
}


void ModuleCluster::parseBytes(){
  for (int i = 0; i < numValues; i++){
    int index = i * 2;
    intValues[i] = (byteValues[index] << 8) | byteValues[index+1];
  }
}

void ModuleCluster::print(PrintType pt){
  if (pt == INT){
    for (int i = 0; i < numValues; i++){
      if (i < numValues - 1){
        ser->print(intValues[i]);
        ser->print(", ");
      } else {
        ser->println(intValues[i]);
      }
    }
  } else if (pt == BYTE){
    for (int i = 0; i < numBytes; i++){
      if (i < numBytes - 1){
        ser->print(byteValues[i]);
        ser->print(", ");
      } else {
        ser->println(byteValues[i]);
      }
    }
  }
}

Arduino_h::HardwareSerial* ModuleCluster::getSerial(){
  return ser;
}
