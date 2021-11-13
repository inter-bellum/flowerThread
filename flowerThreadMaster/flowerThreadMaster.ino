#include <MIDI.h>
#include "ModuleCluster.h"

#define NUM_ARDUINOS 1
#define MIDI_CHANNEL 0
#define CC_START 32

// #define MODE_SERIAL
#define MODE_MIDI

#ifdef MODE_MIDI
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

int loopCount = 0;
const int numberOfModules = NUM_ARDUINOS > 1 ? 3 + ((NUM_ARDUINOS - 1) * 2) : 3;
const int numberOfPots = numberOfModules * 3;

ModuleCluster clusters[NUM_ARDUINOS];
uint16_t potValues[numberOfPots];
int modulesPerCluster[3] = {3, 2, 2};

void setup() {
  Wire.begin();

  #ifdef MODE_MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  #endif

  #ifdef MODE_SERIAL
  Serial.begin(9600);
  #endif
  Serial.println("a");

  for (int i = 0; i < NUM_ARDUINOS; i++){
    //offset is previous number of modules * 3 (pots) * 2 (bytes per pot)
    int offset = (i > 0) ? i * modulesPerCluster[i - 1] * 3 * 2 : 0;
    clusters[i] = ModuleCluster(byte(i), modulesPerCluster[i], &Serial, potValues, offset);

    #ifdef MODE_SERIAL
    Serial.print("Initiated: ");
    Serial.println(i);
    #endif
  }
}

void loop() {
  for (int i = 0; i < NUM_ARDUINOS; i++){
    clusters[i].read();
    #ifdef MODE_SERIAL
    // clusters[i].print(INT);
    #endif
  }

  for (int i = 0; i < numberOfPots; i++){
    #ifdef MODE_SERIAL
    Serial.print(potValues[i]);
    Serial.print(", ");
    #endif

    #ifdef MODE_MIDI
    MIDI.sendControlChange(CC_START + i, potValues[i] / 8, 1);
    #endif
  }
  Serial.println();
}
