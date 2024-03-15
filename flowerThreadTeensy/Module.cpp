#ifndef TEST_LOCAL
#include <MIDI.h>
#else
#include "mock.hpp"
#endif

#include "Module.h"
#include "globals.h"

#define ANALOG_PIN(x)       (x+14)
#define LED_PIN(x)          (x+2)
#define MODULE_MIDI_OFFSET  0

uint8_t colorXMin[3] = {59, 170, 67};
uint8_t colorXMax[3] = {255, 117, 121};
uint8_t colorZMin[3] = {59, 68, 127};
uint8_t colorZMax[3] = {43, 94, 74};

void
Module::initialize(Adafruit_NeoPixel* strip, uint8_t moduleIndex, uint8_t pinNumber, uint8_t numLeds, float filter_param)
{
    this->index = moduleIndex;
    this->ledPin = LED_PIN(moduleIndex);
    this->ledCount = numLeds;
    this->strip = strip;
    this->strip->begin();
    this->strip->show();

    for (int i = 0; i < 3; i++){
        pots[i] = new Pot<EMA>(filter_param);
        pots[i]->initialize(ANALOG_PIN(pinNumber + i), i);
    }
}

Module::~Module()
{
    for (int i = 0; i < NUM_POTS_PER_MODULE; i++){
        delete pots[i];
    }
}


void 
Module::read()
{
    for (int i = 0; i < NUM_POTS_PER_MODULE; i++){
        if (pots[i]->update()){
            potValues_uint8[i] = pots[i]->read();
            potValues_changed[i] = true;
        }
    }
}

void 
Module::sendMidi()
{
    for (int i = 0; i < NUM_POTS_PER_MODULE; i++){
        if (potValues_changed[i]){
            usbMIDI.sendControlChange(102 + i, potValues_uint8[i], this->index + MODULE_MIDI_OFFSET + 1);
            potValues_changed[i] = false;
        }
    }
}

void 
Module::sendSinglePotMidi(uint8_t pot)
{
    usbMIDI.sendControlChange(102 + pot, potValues_uint8[i], this->index + MODULE_MIDI_OFFSET + 1);
}

void 
Module::setPotLight(uint8_t pot)
{
    bool r = pot == 0, 
        g = pot == 1, 
        b = pot == 2;
    
    for (int i = 0; i < ledCount; i++) {
        strip->setPixelColor(i, strip->Color(r * 255, g * 255, b * 255));
    }
    strip->show();
}

void
Module::clearLEDS()
{
    strip->clear();
    strip->show();
}

uint16_t 
Module::get(uint8_t index)
{
    return this->potValues[i];
}

uint8_t 
Module::getPin(uint8_t i)
{
    return this->pots[i]->getPin();
}


void 
Module::updateColor(uint8_t* colors)
{
    interpolateColorSpace(potValues[0], potValues[1], potValues[2], colors);
}


void 
Module::interpolateColorSpace(float xIn, float yIn, float zIn, uint8_t* colors)
{
    float x = xIn / 4096.;
    float y = yIn / 819.2;
    float z = zIn / 4096.;

    uint8_t r = this->interpolateColor(RED, x, z);
    uint8_t g = this->interpolateColor(GREEN, x, z);
    uint8_t b = this->interpolateColor(BLUE, x, z);

#ifdef DEBUG_COLOR
    if (colors != nullptr){
        colors[0] = r;
        colors[1] = g;
        colors[2] = b;
    }
#endif

    for (int i = 0; i < ledCount; i++){
        float diff = y - i;

        if (diff > 1){
            uint32_t c = strip->Color(r, g, b);
            strip->setPixelColor(i, c);
        } else if (diff > 0){
            diff = diff*diff;
            uint32_t c = strip->Color(r * diff, g * diff, b * diff);
            strip->setPixelColor(i, c);
        } else {
            strip->setPixelColor(i, 0);
        }
    }
    strip->show();
}

uint8_t 
Module::interpolateColor(ColorIndex c, float x, float z)
{
    return uint8_t(((colorXMax[c] * x) + (colorXMin[c] * (1 - x)) + (colorZMax[c] * z) + (colorZMin[c] * (1 - z))) / (1 + ((x + z) / 2)));
}
