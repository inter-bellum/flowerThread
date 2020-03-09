/*
 * LEDs seem to react slowly to pots, maybe double analogReads for each pot is too many, considering we're reading 9
 * each read takes ~ 111us, so at 18 reads, that's well over a millisecond each loop.
 * 
 * AnalogRead can be sped up by changing the ADC prescaler to 16 (instead of 128) to increase its speed to 1MHz
 * This hardly effects resolution.
 * 
 * It's also possible to read the registers directly, rather than use the analogRead, which has a bunch of checks and calculations built in
 * try these to speed up reads? 
 * 
 * Serial prints also negatively effect performance, try with all prints disabled.
 */
