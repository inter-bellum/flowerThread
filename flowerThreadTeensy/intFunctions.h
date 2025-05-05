#ifndef __INT_FUNCTIONS_H
#define __INT_FUNCTIONS_H

#include <elapsedMillis.h>

#include "globals.h"

volatile bool instrument_state_change;
volatile instrument_state_t state, state_before_mapping;

volatile bool switch_performance_mode = false, step_mapping_mode = false;

const uint16_t lower_bound = 200, higher_bound = 500;

struct button_state_t
{
    const uint8_t pin = MAPPING_MODE_PIN;
    button_action_t state = button_action_t::NONE;
    elapsedMillis time = 0;
} button;

void
mappingPinInt()
{
    switch(button.state) {
        case button_action_t::NONE: {
            //we got here because we pressed the button, start the timer
            if (digitalReadFast(button.pin) == PRESSED) {
                button.time = 0;
            } else {
                if (button.time > long_press_time) {
                    switch (state) {
                        case instrument_state_t::MAPPING: {
                            state = state_before_mapping;
                            instrument_state_change = true;
                            break;
                        }

                        case instrument_state_t::PERFORM_AT: {
                            state = instrument_state_t::PERFORM_CC;
                            instrument_state_change = true;
                            break;
                        }

                        case instrument_state_t::PERFORM_CC: {
                            state = instrument_state_t::PERFORM_AT;
                            instrument_state_change = true;
                            break;
                        }
                    }
                    
                } else if (state == instrument_state_t::MAPPING) {
                    step_mapping_mode = true;
                    return;
                }

                button.state = button_action_t::SINGLE;
                button.time = 0;
            }
            break;
        }

        case button_action_t::SINGLE: {
            if (digitalReadFast(button.pin) == LET_GO) {
                if (button.time > lower_bound && button.time < higher_bound) {
                    instrument_state_change = true;
                    state_before_mapping = state;
                    state = instrument_state_t::MAPPING;
                    button.time = 0;
                    button.state = button_action_t::NONE;
                    break;
                } else {
                    button.state = button_action_t::NONE;
                    button.time = 0;
                }
            }
        }

        default:
            break;
    } 
}

#endif
