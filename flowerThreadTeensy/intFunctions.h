#ifndef __INT_FUNCTIONS_H
#define __INT_FUNCTIONS_H

volatile bool firstPress, goToMappingMode, stepMappingMode;
volatile uint32_t pressTime;

const uint16_t lowerBound = 200, higherBound = 500;

void
mappingPinInt()
{
    uint32_t currentTime = millis();
    
    if (firstPress && currentTime - pressTime > lowerBound){
        if (currentTime - pressTime < higherBound){
            goToMappingMode = true;
        }
        
        firstPress = false;
    } else if (!firstPress){
        firstPress = true;
        stepMappingMode = true;
        pressTime = millis();
    }  
}

#endif
