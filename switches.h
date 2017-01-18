// switches.h
// Runs on TM4C123
// Provide functions that interact with external switches
// Corentin Dugué, Wei Tat Lee
// September 14, 2016

#include <stdint.h>

//------------Switch_Init------------
// Initialize GPIO Port D bit 0-3 for inputs.
// Input: none
// Output: none
void Switch_Init(void);

//------------Switch_Input0------------
// Read and return the status of GPIO Port D bit 0.
// Input: none
// Output: 0x01 if PD0 is high
//         0x00 if PD0 is low
uint32_t Switch_Input0(void);

//------------Switch_Input1------------
// Read and return the status of GPIO Port D bit 1.
// Input: none
// Output: 0x02 if PD1 is high
//         0x00 if PD1 is low
uint32_t Switch_Input1(void);

//------------Switch_Input2------------
// Read and return the status of GPIO Port D bit 2.
// Input: none
// Output: 0x04 if PD2 is high
//         0x00 if PD2 is low
uint32_t Switch_Input2(void);

//------------Switch_Input3------------
// Read and return the status of GPIO Port D bit 2.
// Input: none
// Output: 0x08 if PD3 is high
//         0x00 if PD3 is low
uint32_t Switch_Input3(void);


//------------Switch_Debounce0------------
// Read and return the status of the switch 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
// debounces switch
uint32_t Switch_Debounce0(void);
uint32_t Switch_Debounce1(void);
uint32_t Switch_Debounce2(void);
uint32_t Switch_Debounce3(void);

void sw0_pressed(void);
