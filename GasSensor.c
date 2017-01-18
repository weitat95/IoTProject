// Name: GasSensor.c
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide functions that interact with the gas sensor
// Last modified: 10/24/2016

/*************************START*************************/

#include <stdint.h>
#include <stdio.h>
#include "GasSensor.h"
#include "inc/tm4c123gh6pm.h"
#include <math.h>


/************************Macros************************************/
#define         RL_VALUE                     5.0     //define the load resistance on the board, in kilo ohms
#define         GAS_LPG                      0
#define         GAS_CO                       1
#define         GAS_SMOKE                    2
#define         GAS_ALCOHOL                  3


/*****************************Globals***********************************************/
float           LPGCurve[3]  =  {2.3,0.21,-0.47};   //two points are taken from the curve.
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve.
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15)
float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve.
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve.
float           AlcoholCurve[3] ={2.3,0.48,-0.44};    //two points are taken from the curve.
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)
float       Ro           =  10.0;                 //Ro is initialized to 10 kilo ohms
float rs_ro_ratio=0;


//------------GasSensorInit------------
// Initialize ADC PE3.
// Input: none
// Output: none

void GasSensorInit() {
  SYSCTL_RCGCADC_R |= 0x0002;   // 7) activate ADC1
                                    // 1) activate clock for Port E
    SYSCTL_RCGCGPIO_R |= 0x08;
    while((SYSCTL_PRGPIO_R&0x08) != 0x08){};
    GPIO_PORTE_DIR_R &= ~0x08;      // 2) make PE3 input
    GPIO_PORTE_AFSEL_R |= 0x08;     // 3) enable alternate function on PE3
    GPIO_PORTE_DEN_R &= ~0x08;      // 4) disable digital I/O on PE3
    GPIO_PORTE_AMSEL_R |= 0x08;     // 5) enable analog functionality on PE3

  //  while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator

    ADC1_PC_R &= ~0xF;              // 7) clear max sample rate field
    ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
    ADC1_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
    ADC1_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
    ADC1_SAC_R|= 0x0;                //0xHardware Oversampling
    ADC1_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
    ADC1_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
    ADC1_SSMUX3_R += 8;             //    set channel
    ADC1_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
    ADC1_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
    ADC1_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}


//------------GasSensorGetGasPPM------------
// Function method to get the gas ppm values from the sensor
// Input: none
// Output: uint32_t output


//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC1_InSeq3(void) {
  uint32_t result;
    ADC1_PSSI_R = 0x0008;            // 1) initiate SS3
    while((ADC1_RIS_R&0x08)==0){};   // 2) wait for conversion done
      // if you have an A0-A3 revision number, you need to add an 8 usec wait here
    result = ADC1_SSFIFO3_R&0xFFF;   // 3) read result
    ADC1_ISC_R = 0x0008;             // 4) acknowledge completion
    return result;
}



/////////////////////////// GET VALUES //////////////////////

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/
float MQGetGasPercentage(uint32_t raw_adc, uint32_t gas_id)
{

  rs_ro_ratio = ((100*RL_VALUE*(4095.0-raw_adc))/raw_adc)/Ro;
  //rs_ro_ratio = 4.0;


  if ( gas_id == GAS_LPG ) {
     // return MQGetPercentage(rs_ro_ratio,LPGCurve);
     return (pow(10,( ((log(rs_ro_ratio)-LPGCurve[1])/LPGCurve[2]) + LPGCurve[0])))*100;
  } else if ( gas_id == GAS_CO ) {
     //return MQGetPercentage(rs_ro_ratio,COCurve);
     return (pow(10,( ((log(rs_ro_ratio)-COCurve[1])/COCurve[2]) + COCurve[0])))*100;
  } else if ( gas_id == GAS_SMOKE ) {
     // return MQGetPercentage(rs_ro_ratio,SmokeCurve);
    //return pow(10, 1.28);
    return (pow(10,( ((log(rs_ro_ratio)-SmokeCurve[1])/SmokeCurve[2]) + LPGCurve[0])))*100;
  }
  else if (gas_id==GAS_ALCOHOL){
    return (pow(10,( ((log(rs_ro_ratio)-AlcoholCurve[1])/AlcoholCurve[2]) + AlcoholCurve[0])))*100;
    //return 3.0;
  }

  return 0;
}
