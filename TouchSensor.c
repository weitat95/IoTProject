// 1) TouchSensor.c
// 2) Corentin Dugue & Wei Tat Lee
// 3) 10/24/2016
// 4) Provide functions to interface the capacitive touch sensor
// 5) Lab 8
// 6) MAHESH SRINIVASAN
// 7) 10/24/2016
// 8) Hardware connections
//    Capacitive touch sensor
//    Ground: GND
//    Out: PE0 (DOUT)
//    Time: unconnected but used for RC circuit
//    Vdd: 3.3V DC
// #1 LEDA: PE1 (DIN)

#include <stdint.h>
#include "TouchSensor.h"
#include "../inc/tm4c123gh6pm.h"

//------------Touch_Init------------
// Initialize GPIO Port E bit 0 for input
// Input: none
// Output: none
void Touch_Init(void) {
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R |= 0x00000010;     // 1) activate clock for Port E
delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                  // 2) no need to unlock GPIO Port E
GPIO_PORTE_AMSEL_R &= ~0x01;      // 3) disable analog on PE0
GPIO_PORTE_PCTL_R &= ~0x0000000F; // 4) PCTL GPIO on PE0
GPIO_PORTE_DIR_R &= ~0x0A;        // 5) direction PE0 input
GPIO_PORTE_AFSEL_R &= ~0x01;      // 6) PE0 regular port function

GPIO_PORTE_DEN_R |= 0x01;         // 7) enable PE0 digital port

GPIO_PORTE_IS_R &= ~0x01;         // 8)) configures for edge triggered
GPIO_PORTE_IBE_R |= 0x01;        // 9)) set input rising edge trigger //change back to invert
GPIO_PORTE_IEV_R |= 0x01;         //
GPIO_PORTE_ICR_R =0x01;
GPIO_PORTE_IM_R |= 0x01;        // 10)) clearing the IME bit to busy wait, set the IME bit for interrupt

NVIC_PRI1_R=(NVIC_PRI1_R&0xFFFFFF00)|0xFFFFFF20; //11)) priority of 1
NVIC_EN0_R = 1 << 4;              //12)) enable interrupt 4
}
