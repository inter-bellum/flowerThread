#include <stdint.h>
#pragma once 

#include "EMA.hpp"

#define MIDI_CHANNEL 0
#define CC_START 32

#define NUM_MODULES 3
#define NUM_POTS_PER_MODULE 3
#define NUM_POTS NUM_MODULES *NUM_POTS_PER_MODULE
#define NUM_LEDS 5

#define MAPPING_MODE_PIN 6

// #define MODE_SERIAL
#define DEBUG_ACTIVE_MODULE 0
#define DEBUG_POTS 0
// #define DEBUG_POT_PLOT
#define DEBUG_COLOR 0
#define MODE_MIDI

template <typename T>
using filter_type = EMA<T>;
constexpr static uint16_t long_press_time = 2000;


enum class button_action_t : uint8_t 
{
    NONE,
    SINGLE,
    DOUBLE,
    LONG
};


enum button_press_t
{
    PRESSED = 0,
    LET_GO
};

enum class instrument_state_t : uint8_t
{
    PERFORM_CC,
    PERFORM_AT,
    MAPPING
};

enum class play_mode_t : uint8_t
{
    PLAYING,
    MAPPING
};