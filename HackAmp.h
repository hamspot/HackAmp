// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef HackAmp_H_
#define HackAmp_H_
#include "Arduino.h"
//add your includes for the project HackAmp here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project HackAmp here
void lcdBright(int bright);
void lcdWrite(const char* line1, const char* line2);
void lcdAnimateBacklight(void);
void lcdBoot(void);
void mixerSet(int channel, long level);
bool buttonRead(void);
void encoderRead(void);




//Do not add code below this line
#endif /* HackAmp_H_ */
