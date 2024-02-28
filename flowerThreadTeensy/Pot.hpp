#pragma once 

#include <cstdint>

#include "Arduino.h"

#include "filter.hpp"
#include "hysteresis.hpp"

#define HISTORY_SIZE  100

template <template <typename> class F_t, typename T = uint16_t, uint8_t MaxIn = 10, uint8_t MaxOut = 7>
class Pot {
    //I2C uses A4 and A5, skip those
    uint8_t analogPin;
    uint8_t index;
    F_t<T> filter;
    hysteresis<T, MaxIn, MaxOut> hyst;
    T last_value;
public:

    Pot(float filter_length_or_scaling);
    ~Pot() = default;

    void 
    initialize(uint8_t analog_in, uint8_t index);

    bool
    update();

    T 
    read();

    uint16_t 
    getValue();

    uint8_t 
    getPin();
};

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
Pot<F_t, T, MaxIn, MaxOut>::Pot(float filter_length_or_scaling)
    : filter(filter_length_or_scaling)
{}

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
void
Pot<F_t, T, MaxIn, MaxOut>::initialize(uint8_t analogPin, uint8_t index)
{
    this->analogPin = analogPin;
    this->index = index;
}

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
bool 
Pot<F_t, T, MaxIn, MaxOut>::update()
{
    uint16_t read_val = analogRead(analogPin);

    #ifdef DEBUG_POT_PRINT
    debugPrint();
    #endif
    #ifdef DEBUG_POT_PLOT
    debugPlot();
    #endif

    if (hyst.update(filter.update(read_val))) {
        last_value = hyst.get();
        return true;
    }

    return false;
}

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
T 
Pot<F_t, T, MaxIn, MaxOut>::read()
{
    return last_value;
}

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
uint16_t 
Pot<F_t, T, MaxIn, MaxOut>::getValue()
{
    return filter.get();
}

template <template <typename> class F_t, typename T, uint8_t MaxIn, uint8_t MaxOut>
uint8_t
Pot<F_t, T, MaxIn, MaxOut>::getPin()
{
    return this->analogPin;
}
