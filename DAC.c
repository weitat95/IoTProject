// DAC.c
// Runs on TM4C123
// Provide Initialization Function and Output Function to the DAC through port D 
// Corentin Dugue Wei Tat Lee
// September 28, 2016
// Hardware
// SSI1Clk (SCLK, pin 4) connected to PD0
// SSI1Fss (!CS, pin 2) connected to PD1
// SSI1Tx (DIN, pin 3) connected to PD3

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"


//********DAC_Init*****************
// Initialize TLV5616 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
void DAC_Init(uint16_t data) {
  SYSCTL_RCGCSSI_R |= 0x02; // activate SSI1
  SYSCTL_RCGCGPIO_R |= 0x08; // activate port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){};// ready?
  GPIO_PORTD_AFSEL_R |= 0x0B; // enable alt funct on PD0,1,3
  GPIO_PORTD_DEN_R |= 0x0B; // configure PD0,1,3 as SSI
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0F00)+0x00002022;
  GPIO_PORTD_AMSEL_R = 0; // disable analog functionality on PD
  SSI1_CR1_R = 0x00000000; // disable SSI, master mode
  SSI1_CPSR_R = 0x04; // 20 MHz SSIClk
  SSI1_CR0_R &= ~(0x0000FFF0); // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI1_CR0_R |= 0x4F; // DSS = 16-bit data
  SSI1_DR_R = data; // load ‘data’ into transmit FIFO
  SSI1_CR1_R |= 0x00000002; // enable SSI
}

//********DAC_Out*****************
// Send data to TLV5616 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t code) {
  while ((SSI1_SR_R&0x00000002)==0){};
  SSI1_DR_R = code;
}
