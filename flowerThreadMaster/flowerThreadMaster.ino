#include "ModuleCluster.h"

#define NUM_ARDUINOS 1

int loopCount = 0;

ModuleCluster clusters[NUM_ARDUINOS];
int potsPerCluster[3] = {3, 2, 2};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  for (int i = 0; i < NUM_ARDUINOS; i++){
    clusters[i] = ModuleCluster(byte(i), potsPerCluster[i], &Serial);
    Serial.print("Initiated: ");
    Serial.println(i);
  }
}

void loop() {
  for (int i = 0; i < NUM_ARDUINOS; i++){
//    Serial.println(i);
    clusters[i].read();
    clusters[i].print(INT);
  }
}
