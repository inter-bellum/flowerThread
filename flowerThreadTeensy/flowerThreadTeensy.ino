#include "intFunctions.h"
#include "Module.h"
#include <Adafruit_NeoPixel.h>
#include "globals.h"

Adafruit_NeoPixel strip[NUM_MODULES];

Module modules[NUM_MODULES];

play_mode_t mode = play_mode_t::PLAYING;


#ifdef DEBUG_COLOR
uint8_t colors[3];
#endif

uint8_t current_pot = 0, previous_pot = 0;

void
cond_debug_print(String msg, bool cond, bool newline = true)
{
    if (cond) {
        Serial.print(msg);
        if (newline)
          Serial.print("\n");
    }
}

void 
setup()
{
    Serial.begin(115200);
    Serial.println("Num pots: " + String(NUM_POTS));

    analogReadRes(10);

    pinMode(MAPPING_MODE_PIN, INPUT);
    attachInterrupt(MAPPING_MODE_PIN, mappingPinInt, CHANGE);

    for (int i = 0; i < NUM_MODULES; i++) {
        strip[i] = Adafruit_NeoPixel(NUM_LEDS, i + 2, NEO_GRB + NEO_KHZ800);

        uint8_t moduleNumber = i;
        uint8_t pinNumber = i * NUM_POTS_PER_MODULE;

        modules[i].initialize(strip + i, moduleNumber, pinNumber, NUM_LEDS, 0.2);

        Serial.print("Initialized module: ");
        Serial.println(i);
    }
}

void 
loop()
{
    if (instrument_state_change && state == instrument_state_t::MAPPING) {
        Serial.println("GOTO MAPPING MODE");
    }

    if (instrument_state_change) {
        switch (state) {
            case instrument_state_t::PERFORM_CC: {
                for (int i = 0; i < NUM_MODULES; i++){
                    modules[i].switch_play_mode(play_mode::CC);
                }
                mode = play_mode_t::PLAYING;
                Serial.println("Switch to CC");
                break;
            }

            case instrument_state_t::PERFORM_AT: {
                modules[0].switch_play_mode(play_mode::CC_NOTE_TRIG);
                modules[1].switch_play_mode(play_mode::AFTERTOUCH);
                modules[2].switch_play_mode(play_mode::CC_NOTE_TRIG);
                mode = play_mode_t::PLAYING;
                Serial.println("Switch to AT");
                break;
            }

            case instrument_state_t::MAPPING: {
                mode = play_mode_t::MAPPING;
            }
        }
    } 

    switch (mode) {
        case play_mode_t::PLAYING: {
            for (int i = 0; i < NUM_MODULES; i++) {
                uint8_t colors[3] = {0};
                modules[i].read();
                modules[i].sendMidi();
                modules[i].updateColor(colors);

                
                cond_debug_print("R: " + String(colors[0]) + ", G: " + String(colors[1]) + ", B: " + String(colors[2]), DEBUG_COLOR == 1);  
            }
            break;
        }

        
    case play_mode_t::MAPPING:
        uint8_t current_module = current_pot / 3;
        uint8_t _module = previous_pot / 3;

        if (step_mapping_mode) {
            previous_pot = current_pot;
            current_pot = (current_pot + 1) % 9;

            if (current_module != _module) {
              modules[_module].clearLEDS();
            }

            modules[current_module].setPotLight(current_pot % 3);

            step_mapping_mode = false;
       }


      modules[current_module].read();
      modules[current_module].sendSinglePotMidi(current_pot % 3);
      break;
    }
    
    delay(12);
}

void 
debug_colors()
{
    for (int i = 0; i < NUM_MODULES; i++) {
        if (DEBUG_ACTIVE_MODULE && i == DEBUG_ACTIVE_MODULE - 1) {
#if DEBUG_POTS == 1
            Serial.print("Module ");
            Serial.print(i);
            Serial.print("\t");
#endif
            modules[i].read();
            for (int j = 0; j < 3; j++) {
#if DEBUG_POTS == 1
                Serial.print("Pin ");
                Serial.print(modules[i].getPin(j));
                Serial.print("\t");

                Serial.print(modules[i].values[j]);
                Serial.print("\t");
#endif
            }
#if DEBUT_POTS == 1
            Serial.println();
#endif

            modules[i].updateColor(colors);
            Serial.print("Module: ");
            Serial.println(i);
            float x = modules[i].values[0] / 127.;
            float y = modules[i].values[1] / 127.;
            float z = modules[i].values[2] / 127.;

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

            for (int i = 0; i < 5; i++) {
                float diff = y - i;

                if (diff > 1) {
                    Serial.print("LED: ");
                    Serial.print(i);
                    Serial.print("\t");
                    Serial.print("Bright: ");
                    Serial.print(1);
                } else if (diff > 0) {
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
}
