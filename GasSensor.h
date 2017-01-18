// 1) GasSensor.h
// 2) Corentin Dugue & Wei Tat Lee
// 3) 10/24/2016
// 4) Provide functions to interface with the gas sensor
// 5) Lab 8
// 6) MAHESH SRINIVASAN
// 7) 10/24/2016
// 8) Hardware connections
//     Gas Sensor
// #1  Power: 5V DC
//     Data out: PE3 (ADC)
//     Ground: GND

#include <stdint.h>
#include <stdio.h>

//------------GasSensorInit------------
// Initialize ADC PE3.
// Input: none
// Output: none

void GasSensorInit();

uint32_t ADC1_InSeq3(void);



//------------GasSensorGetGasPPM------------
// Function method to get the gas ppm values from the sensor
// Input: none
// Output: uint32_t output


void ADC1Seq3_Handler(void);

float MQGetGasPercentage(uint32_t raw_adc, uint32_t gas_id);
