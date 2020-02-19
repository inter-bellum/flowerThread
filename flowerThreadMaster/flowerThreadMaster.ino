#include "ModuleCluster.h"

#define NUM_ARDUINOS 1

int loopCount = 0;

ModuleCluster clusters[NUM_ARDUINOS];
int potsPerCluster[3] = {3, 2, 2};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  for (int i = 0; i < NUM_ARDUINOS; i++){
    clusters[i] = ModuleCluster(byte(i + 5), potsPerCluster[i]);
  }
}

void loop() {
  for (int i = 0; i < NUM_ARDUINOS; i++){
    clusters[i].read();
    if (loopCount > 60){
      clusters[i].print();
      loopCount = 0;
    }
//    clusters[i].print();
  }
  loopCount++;
//  Wire.requestFrom(5, 18);
//  delay(100);
}
