// Name: LightSensor.h
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide functions to interface the light sensor
// Last modified: 10/24/2016
// Hardware connections:
//     Light sensor
// #1  Power: 3.3V DC
//     Data out: PE4 (ADC)
//     Ground: GND

/*************************START*************************/

#include <stdint.h>
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

//------------ADC0_InitSWTriggerSeq3_Ch9------------
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain9 (PE4)
// SS3 interrupts: enabled but not promoted to controller
void ADC0_InitSWTriggerSeq3_Ch9(void);

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC0_InSeq3(void);

uint32_t GetBrightness(uint32_t light);
