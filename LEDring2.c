// Name: LEDring.c
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide functions to interface the LED ring (16 LEDS with RGB)
// Last modified: 10/24/2016

/*************************START*************************/

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "SysTick.h"
#include "LEDring.h"
#include "Colors.h"
#define PE2 (*((volatile uint32_t *)0x40024010))

#define NLEDS 24

#define T1H  56    // Width of a 1 bit in ns
#define T1L  48   // Width of a 1 bit in ns // should be 48
#define T0H  18    // Width of a 0 bit in ns
#define T0L  48    // Width of a 0 bit in ns // should be 64
#define RES 4000    // Width of the low gap between bits to cause a frame to latch


void Timer2A_Init(uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x04;
  TIMER2_CTL_R=0x00000000;
  TIMER2_CFG_R=0x00000000;
  TIMER2_TAMR_R=0x00000001; // one shot
  TIMER2_TAILR_R=period-1;
  TIMER2_TAPR_R=0;
  TIMER2_ICR_R=0x00000001;
  TIMER2_IMR_R=0x00000001;
  TIMER2_CTL_R=0x00000001;
}

//------------LED_Init------------
// Initialize GPIO Port E bit 2 for output.
void LED_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x10;          // 1) Port E clock
   while((SYSCTL_PRGPIO_R&0x0010) == 0){};// ready?
   GPIO_PORTE_AMSEL_R &= ~0x04;     // 3) disable analog
   GPIO_PORTE_PCTL_R &= ~0x00000F00;// 4) configure as GPIO
   GPIO_PORTE_DIR_R |= 0x04;        // 5) PE2 outputs
   GPIO_PORTE_AFSEL_R &= ~0x04;     // 6) normal function
   GPIO_PORTE_DEN_R |= 0x04;        // 7) digital I/O on PE2
}


//------------LED_Main------------
// Light the different LEDS
void LED_Main(void) {
  // show();
}

uint32_t i;
uint32_t j;


// Functions to display different patterns to LED ring
void BlinkingColor(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness){
  AllColor(50,50,50,brightness);
  SysTick_Wait10ms(50);
  Color1_Color2(RColor/brightness, GColor/brightness, BColor/brightness, 50, 50, 50, brightness);
  SysTick_Wait10ms(50);
}

void Blinking_Color1_Color2(unsigned char RColor1, unsigned char GColor1, unsigned char BColor1, unsigned char RColor2, unsigned char GColor2, unsigned char BColor2, uint32_t brightness) {
  AllColor(RColor1, GColor1, BColor1,brightness);
  SysTick_Wait10ms(50);
  Color1_Color2(RColor2, GColor2, BColor2, RColor1, GColor1, BColor1, brightness);
  SysTick_Wait10ms(50);
}

uint8_t BLUE_COLOR[24][3];
BLUE_COLOR[0][0]=; BLUE_COLOR[0][1]=; BLUE_COLOR[0][2]=;
BLUE_COLOR[1][0]=; BLUE_COLOR[1][1]=; BLUE_COLOR[1][2]=;
BLUE_COLOR[2][0]=; BLUE_COLOR[2][1]=; BLUE_COLOR[2][2]=;
BLUE_COLOR[3][0]=; BLUE_COLOR[3][1]=; BLUE_COLOR[3][2]=;
BLUE_COLOR[4][0]=; BLUE_COLOR[4][1]=; BLUE_COLOR[4][2]=;
BLUE_COLOR[5][0]=; BLUE_COLOR[5][1]=; BLUE_COLOR[5][2]=;
BLUE_COLOR[6][0]=; BLUE_COLOR[6][1]=; BLUE_COLOR[6][2]=;
BLUE_COLOR[7][0]=; BLUE_COLOR[7][1]=; BLUE_COLOR[7][2]=;
BLUE_COLOR[8][0]=; BLUE_COLOR[8][1]=; BLUE_COLOR[8][2]=;
BLUE_COLOR[9][0]=; BLUE_COLOR[9][1]=; BLUE_COLOR[9][2]=;
BLUE_COLOR[10][0]=; BLUE_COLOR[10][1]=; BLUE_COLOR[10][2]=;
BLUE_COLOR[11][0]=; BLUE_COLOR[11][1]=; BLUE_COLOR[11][2]=;
BLUE_COLOR[12][0]=; BLUE_COLOR[12][1]=; BLUE_COLOR[12][2]=;
BLUE_COLOR[13][0]=; BLUE_COLOR[13][1]=; BLUE_COLOR[13][2]=;
BLUE_COLOR[14][0]=; BLUE_COLOR[14][1]=; BLUE_COLOR[14][2]=;
BLUE_COLOR[15][0]=; BLUE_COLOR[15][1]=; BLUE_COLOR[15][2]=;
BLUE_COLOR[16][0]=; BLUE_COLOR[16][1]=; BLUE_COLOR[16][2]=;
BLUE_COLOR[17][0]=; BLUE_COLOR[17][1]=; BLUE_COLOR[17][2]=;
BLUE_COLOR[18][0]=; BLUE_COLOR[18][1]=; BLUE_COLOR[18][2]=;
BLUE_COLOR[19][0]=; BLUE_COLOR[19][1]=; BLUE_COLOR[19][2]=;
BLUE_COLOR[20][0]=; BLUE_COLOR[20][1]=; BLUE_COLOR[20][2]=;
BLUE_COLOR[21][0]=; BLUE_COLOR[21][1]=; BLUE_COLOR[21][2]=;
BLUE_COLOR[22][0]=; BLUE_COLOR[22][1]=; BLUE_COLOR[22][2]=;
BLUE_COLOR[23][0]=; BLUE_COLOR[23][1]=; BLUE_COLOR[23][2]=;

void Slick(uint32_t brightness) {
  for (i=0; i<NLEDS; i++) {
      sendPixel(BLUE_COLOR[i][0]/brightness, BLUE_COLOR[i][1]/brightness, BLUE_COLOR[i][2]/brightness); }
   if (i==NLEDS-1) {SysTick_Wait(50000);}
}

void SlickLoading(uint32_t brightness) {
  for (i=0; i<NLEDS; i++) {
    for (j=0; j<NLEDS; j++) {
      if (j<=i) {sendPixel(BLUE_COLOR[j][0]/brightness, BLUE_COLOR[j][1]/brightness, BLUE_COLOR[j][2]/brightness);}
      //else if (j==i) {sendPixel(BLUE_COLOR[i][0]/brightness, BLUE_COLOR[i][1]/brightness, BLUE_COLOR[i][2]/brightness);}
      else {sendPixel(0, 0, 0);}
    }
    SysTick_Wait10ms(delay);
  }
  if (i==NLEDS-1) {SysTick_Wait(50000);}
}

/*
void Blinking_Color1_Color2(unsigned char RColor1, unsigned char GColor1, unsigned char BColor1, unsigned char RColor2, unsigned char GColor2, unsigned char BColor2, uint32_t brightness) {
  Color1_Color2(RColor1, GColor1, BColor1, RColor2, GColor2, BColor2, brightness);
  SysTick_Wait10ms(50);
  Color1_Color2(RColor2, GColor2, BColor2, RColor1, GColor1, BColor1, brightness);
  SysTick_Wait10ms(50);
}
*/


void Color1_Color2(unsigned char RColor1, unsigned char GColor1, unsigned char BColor1, unsigned char RColor2, unsigned char GColor2, unsigned char BColor2, uint32_t brightness) {
  for (i=0; i<NLEDS; i++) {
      if (i%2==1) { sendPixel(RColor1/brightness, GColor1/brightness, BColor1/brightness); }
      else sendPixel(RColor2/brightness, GColor2/brightness, BColor2/brightness);
  }
  if (i==NLEDS-1) {SysTick_Wait(50000);}
}

void AllColor(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness){
  //sendPixel(LED_Status.red,LED_Status.green,LED_Status.blue);
  //sendPixel(LED_R,LED_G, LED_B);
  for (i=0; i<NLEDS; i++) {
      sendPixel(RColor/brightness, GColor/brightness, BColor/brightness); }
   if (i==NLEDS-1) {SysTick_Wait(50000);}
}

/*
void Loading(void) {
   for (i=0; i<NLEDS; i++) {
     for (j=0; j<NLEDS; j++) {
       if (i==j) {sendPixel(37, 0, 52);}
       else {sendPixel(0, 0, 0);}
     }
   SysTick_Wait10ms(8);
   }
   if (i==NLEDS-1) {SysTick_Wait(50000);}
}
*/

void Loading(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness, uint32_t delay) {
   for (i=0; i<NLEDS; i++) {
     for (j=0; j<NLEDS; j++) {
       if (j<=i) {sendPixel(RColor/brightness, GColor/brightness, BColor/brightness);}
       else {sendPixel(0, 0, 0);}
     }
   SysTick_Wait10ms(delay);
   }
   if (i==NLEDS-1) {SysTick_Wait(50000);}
}


//------------sendBit------------
// Send a bit to the pixel
void sendBit( bool bitVal ) {
  if (bitVal) {      // 1-bit
    PE2 = 0x04;
    SysTick_Wait(T1H);
    PE2 = 0;
    SysTick_Wait(T1L);
  }

  else {             // 0-bit
    // cli();                                       // We need to protect this bit from being made wider by an interrupt
    PE2 = 0x04;
    SysTick_Wait(T0H);
    PE2 = 0;
    SysTick_Wait(T0L);
    // sei();
    //DELAY_CYCLES( NS_TO_CYCLES( T0L ) - 10 ); // 0-bit gap less overhead of the loop
  }
}

//------------sendByte------------
// Function to send a byte to the LED ring
void sendByte( unsigned char byte ) {
  sendBit((byte&0x80)>>7);
  sendBit((byte&0x40)>>6);
  sendBit((byte&0x20)>>5);
  sendBit((byte&0x10)>>4);
  sendBit((byte&0x08)>>3);
  sendBit((byte&0x04)>>2);
  sendBit((byte&0x02)>>1);
  sendBit((byte&0x01));
}

//------------sendPixel------------
// Send a single pixel to the string
void sendPixel( unsigned char r, unsigned char g , unsigned char b ) {

  sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
  sendByte(r);
  sendByte(b);
}

// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame
void show(void) {
  SysTick_Wait(RES);
}
