#include "intFunctions.h"
#include "Module.h"
#include <Adafruit_NeoPixel.h>

#define MIDI_CHANNEL 0
#define CC_START 32

#define NUM_MODULES           3
#define NUM_POTS_PER_MODULE   3
#define NUM_POTS              NUM_MODULES * NUM_POTS_PER_MODULE
#define NUM_LEDS              5

#define MAPPING_MODE_PIN      6

//#define MODE_SERIAL
#define DEBUG_ACTIVE_MODULE 0
#define DEBUG_POTS          0
#define DEBUG_COLOR         0
#define MODE_MIDI

Adafruit_NeoPixel strip[NUM_MODULES];

Pot pots[NUM_POTS];
Module modules[NUM_MODULES];

#ifdef DEBUG_COLOR
uint8_t colors[3];
#endif

typedef enum CurrentMode {
  MODE_PLAYING,
  MODE_MAPPING
} CurrentMode;

CurrentMode mode = MODE_PLAYING;
uint8_t currentPot = 0, _prevPot = 0;

void setup() {
  #ifdef MODE_SERIAL
  Serial.begin(9600);
  Serial.println("Num pots: " + String(NUM_POTS));
  #endif

  analogReadRes(12);

  pinMode(MAPPING_MODE_PIN, INPUT);
  attachInterrupt(MAPPING_MODE_PIN, mappingPinInt, FALLING);

  for (int i = 0; i < NUM_MODULES; i++){
    strip[i] = Adafruit_NeoPixel(NUM_LEDS, i + 2, NEO_GRB + NEO_KHZ800);

    uint8_t moduleNumber = i;
    uint8_t pinNumber = i * NUM_POTS_PER_MODULE;
    
    modules[i] = Module(strip + i, moduleNumber, pinNumber, NUM_LEDS);

    #ifdef MODE_SERIAL
    Serial.print("Initialized module: ");
    Serial.println(i);
    #endif
  }
}

void loop() {
  if (goToMappingMode){
    mode = MODE_MAPPING;
    goToMappingMode = false;
  }

  switch(mode){
    case MODE_PLAYING:
      #ifdef MODE_MIDI
      for (int i = 0; i < NUM_MODULES; i++){
        modules[i].read();
        modules[i].sendMidi();
        modules[i].updateColor(NULL);
      }
      #endif
      //DEBUG STUFF
      //NOT COMPILED DUE TO PREPROCESSOR COMMANDS
  
      #ifdef MODE_SERIAL && !MODE_MIDI
      for (int i = 0; i < NUM_MODULES; i++){
        if (DEBUG_ACTIVE_MODULE && i == DEBUG_ACTIVE_MODULE - 1){
          #if DEBUG_POTS == 1
          Serial.print("Module ");
          Serial.print(i);
          Serial.print("\t");
          #endif
          modules[i].read();
          for (int j = 0; j < 3; j++){
            #if DEBUG_POTS == 1
            Serial.print("Pin ");
            Serial.print(modules[i].getPin(j));
            Serial.print("\t");
            
            Serial.print(modules[i].potValues[j]);
            Serial.print("\t");
            #endif
          }
          #if DEBUT_POTS == 1
          Serial.println();
          #endif
          
          modules[i].updateColor(colors);
          Serial.print("Module: ");
          Serial.println(i);
          float x = modules[i].potValues[0] / 4096.;
          float y = modules[i].potValues[1] / 819.2;
          float z = modules[i].potValues[2] / 4096.;
    
          uint8_t redtest = uint8_t(((colorXMax[RED] * x) + (colorXMin[RED] * (1 - x)) + (colorZMax[RED] * z) + (colorZMin[RED] * (1 - z))) / 2.);
    
          uint8_t r = modules[i].interpolateColor(RED, x, z);
          uint8_t g = modules[i].interpolateColor(GREEN, x, z);
          uint8_t b = modules[i].interpolateColor(BLUE, x, z);
    
          Serial.print("Vals: ");
          Serial.print(x);
          Serial.print("\t");
          Serial.print(y);
          Serial.print("\t");
          Serial.println(z);
    
          Serial.print("Red: ");
          Serial.println(redtest);
          
          Serial.print("Cols: ");
          Serial.print(r);
          Serial.print("\t");
          Serial.print(g);
          Serial.print("\t");
          Serial.println(b);
    
          for (int i = 0; i < 10 / 2; i++){
            float diff = y - i;
            int mirror = 10 - i - 1;
        
            if (diff > 1){
              Serial.print("LED: ");
              Serial.print(i);
              Serial.print("\t");
              Serial.print("Bright: ");
              Serial.print(1);
            } else
            if (diff > 0){
              Serial.print("LED: ");
              Serial.print(i);
              Serial.print("\t");
              Serial.print("Bright: ");
              Serial.print(diff);
            } else {
              Serial.print("LED: ");
              Serial.print(i);
              Serial.print("\t");
              Serial.print("Bright: ");
              Serial.print(0);
            }
    
              Serial.print("\t");
              Serial.print("Color: ");
              Serial.print(colors[0]);
              Serial.print(", ");
              Serial.print(colors[1]);
              Serial.print(", ");
              Serial.print(colors[2]);
              Serial.println();
          }
        }
      }
      #endif
      break;
    case MODE_MAPPING:
      if (digitalRead(MAPPING_MODE_PIN) == 0 && (millis() - pressTime > 2000)){
        mode = MODE_PLAYING;
        firstPress = false;
      }
      
      if (stepMappingMode){
        _prevPot = currentPot;
        currentPot = (currentPot + 1) % 9;        
        uint8_t currentModule = currentPot / 3;
        uint8_t _module = _prevPot / 3;
        
        #ifdef MODE_SERIAL
        Serial.println("Module: " + String(currentModule) + ", Pot: "+ String(currentPot % 3));
        #endif
        
        if (currentModule != _module){
          modules[_module].clearLEDS();
        }
        
        modules[currentModule].setPotLight(currentPot % 3);
        modules[currentModule].sendSinglePotMidi(currentPot % 3);
        
        stepMappingMode = false;
      }
      break;
    
  }
  delay(15);
}
