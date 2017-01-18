// DAC.h
// Runs on TM4C123
// Provide Initialization Function and Output Function to the DAC through port D 
// Corentin Dugue Wei Tat Lee
// September 28, 2016

#include <stdint.h>

// SSI1Clk (SCLK, pin 4) connected to PD0
// SSI1Fss (!CS, pin 2) connected to PD1
// SSI1Tx (DIN, pin 3) connected to PD3

//********DAC_Init*****************
// Initialize TLV5616 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
void DAC_Init(uint16_t data);

//********DAC_Out*****************
// Send data to TLV5616 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t code);


