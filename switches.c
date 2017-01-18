// switches.c
// Runs on TM4C123
// Provide functions that interact with external switches
// Corentin Dugue Wei Tat Lee
// September 14, 2016

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "SysTick.h"
#include "switches.h"

#define PD0   (*((volatile unsigned long *)0x40007004))
#define PD1   (*((volatile unsigned long *)0x40007008))
#define PD2   (*((volatile unsigned long *)0x40007010))
#define PD3   (*((volatile unsigned long *)0x40007020))

void Switch_Init(void){ 
  volatile unsigned long delay;
  SYSCTL_RCGCGPIO_R |= 0x00000008;     // 1) activate clock for Port D
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock GPIO Port D
  GPIO_PORTD_AMSEL_R &= ~0x0F;      // 3) disable analog on PD0-3
  GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // 4) PCTL GPIO on PD0-3
  GPIO_PORTD_DIR_R &= ~0x0F;        // 5) direction PD0-3 input
  GPIO_PORTD_AFSEL_R &= ~0x0F;      // 6) PD0-3 regular port function

  GPIO_PORTD_DEN_R |= 0x0F;         // 7) enable PD0-3 digital port
  
  GPIO_PORTD_IS_R &= ~0x0F;         // 8)) configures for edge triggered
  GPIO_PORTD_IBE_R &= ~0x0F;        // 9)) set input rising edge trigger
  GPIO_PORTD_IEV_R |= 0x0F;         //        
  GPIO_PORTD_ICR_R =0x0F;
  GPIO_PORTD_IM_R |= 0x0F;        // 10)) clearing the IME bit to busy wait, set the IME bit for interrupt
  
  NVIC_PRI0_R=(NVIC_PRI0_R&0x00FFFFFF)|0x40FFFFFF; //11)) priority of 4
  NVIC_EN0_R = 0x00000008;              //12)) enable interrupt 3

}


#define DELAY10MS 160000
#define DELAY10US 160
//------------Switch_Debounce------------
// Read and return the status of the switch 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
// debounces switch
uint32_t Switch_Debounce0(void){
  uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = PD0;
  while(time){
    SysTick_Wait(DELAY10US); // 10us
    in = PD0;
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}
uint32_t Switch_Debounce1(void){
  uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = PD1;
  while(time){
    SysTick_Wait(DELAY10US); // 10us
    in = PD1;
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t Switch_Debounce2(void){
  uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = PD2;
  while(time){
    SysTick_Wait(DELAY10US); // 10us
    in = PD2;
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t Switch_Debounce3(void){
  uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = PD3;
  while(time){
    SysTick_Wait(DELAY10US); // 10us
    in = PD3;
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}
