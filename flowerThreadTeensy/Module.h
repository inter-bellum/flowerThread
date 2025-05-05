#include "globals.h"
#ifndef __MODULE_H
#define __MODULE_H

#include "Pot.hpp"
#include "EMA.hpp"

#ifndef TEST_LOCAL
#include <Adafruit_NeoPixel.h>
#else
#include "mock.hpp"
#endif

#define LPF_OLD_WEIGHT  0.99f
#define LPF_NEW_WEIGHT  1.f - LPF_OLD_WEIGHT
enum ColorIndex{
    RED = 0U,
    GREEN,
    BLUE
};

enum class play_mode : uint8_t
{
    CC,
    CC_NOTE_TRIG,
    AFTERTOUCH
};

extern uint8_t colorXMin[3];
extern uint8_t colorXMax[3];
extern uint8_t colorZMin[3];
extern uint8_t colorZMax[3];


class Module{
public:
    Module() = default;
    ~Module();

    void
    initialize(Adafruit_NeoPixel* strip, uint8_t moduleIndex, uint8_t pinNumber, uint8_t numLeds, float filter_param);
    
    void 
    read();
    
    uint16_t 
    get(uint8_t index);
    
    uint8_t 
    getPin(uint8_t i);
    
    void 
    updateColor(uint8_t* color = nullptr);
    
    void 
    setPotLight(uint8_t pot);
    
    void
    clearLEDS();
    
    void
    sendMidi();
    
    void 
    sendSinglePotMidi(uint8_t pot);
    
    uint8_t 
    interpolateColor(ColorIndex c, float x, float z);

    void 
    interpolateColorSpace(float xIn, float y, float zIn, uint8_t* color);

    void
    switch_play_mode(play_mode mode)
    {
        this->mode = mode;
    }

    play_mode
    get_play_mode()
    {
        return mode;
    }

    uint8_t
    calculate_change_velocity(uint8_t val);

    Adafruit_NeoPixel* strip;
    uint8_t values[3];
    bool values_changed[3];
private:
    uint8_t index;
    
    //Pots are x, y, z (y being vertical)
    Pot<EMA>* pots[3];
    
    uint8_t ledCount = 0;
    float led_step;
    uint8_t ledPin;

    uint8_t i = 0;
    play_mode mode = play_mode::CC;
    uint8_t play_thresh = 3;
    bool AT_active[NUM_POTS_PER_MODULE] = {false};
    float previous_value = 0;
    EMA<float> AT_in_filter{0.99};
    EMA<float> AT_dif_filter{0.99};

    uint8_t previous_slot = 0;
    uint8_t threshold = 16;

};

#endif
