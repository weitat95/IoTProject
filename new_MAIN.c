// Name: new_MAIN.c
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Main function of our IOT system
// Last modified: 10/24/2016

/*************************START*************************/

#include <stdint.h>
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "LEDring.h"
#include "SysTick.h"
#include "barometer.h"
#include "LightSensor.h"
#include "UART.h"
#include "esp8266.h"
#include "GasSensor.h"
#include "TouchSensor.h"
#include "Colors.h"

/*
struct RGB LED_BOTH={SKY_BLUE};
struct RGB LED_WIFI={SKY_BLUE};
struct RGB LED_BT={SKY_BLUE};
struct RGB LED_NO={SKY_BLUE};
*/

// Functions declaration
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
// WIFI
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#define PE0   (*((volatile unsigned long *)0x40024004))

// Constant
#define GAS_THRESHOLD 1000
#define TEMP_THRESHOLD 750
#define TEMP1 735
#define TEMP2 745
#define PERIOD  400000000 // 5 sec

// Variables declaration
uint32_t temperature=0;
uint32_t light=500;
uint32_t touch_wifi=0;
uint32_t periodic_wifi=0;
uint32_t uploading=0;
uint32_t brightness=0;
uint32_t gas=0;
uint32_t lightPercentage=0;
float gasi=0;
uint32_t WIFI_ON=0;
uint32_t BLUETOOTH_ON=0;

/*
uint8_t LED_R=0;
uint8_t LED_G=0;
uint8_t LED_B=0;
*/


// Debugging
uint32_t START=0;
uint32_t Touch=0;

void Timer0A_Init(void){
  volatile uint32_t delay;
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
  // 1shot mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = PERIOD;         // for 10seconds timer; changed to 5 seconds for debugging
  TIMER0_IMR_R = 0x00000001;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
  // Timer0A=priority 3
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x60000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}

// Periodic timer handler
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  //AllColor(WHITE_LOW, 1);
  temperature=1400-(uint32_t)ReadTemperature();
  light = ADC0_InSeq3();
  gas=ADC1_InSeq3();
  if ((gas>=GAS_THRESHOLD) || (temperature >= TEMP_THRESHOLD)) {
  periodic_wifi=1;
  touch_wifi=0;
  }
}

// Capacitive touch sensor handler
void GPIOPortE_Handler(void){
  if(GPIO_PORTE_RIS_R&0x01){
    GPIO_PORTE_ICR_R=0x01;
    Touch ^= 1;
    //AllColor(WHITE_LOW, 1);
    // Do functions to get temperature and log on server
    temperature=1400-(uint32_t)ReadTemperature();
    light = ADC0_InSeq3();
    gas=ADC1_InSeq3();
    touch_wifi=1;
  }
}

int main(void){
  PLL_Init(Bus80MHz);
  LED_Init();
  SysTick_Init();
  Touch_Init();
  GasSensorInit();
  Baro_Init();
  ADC0_InitSWTriggerSeq3_Ch9();
  EnableInterrupts();
  StartBarometer();
  connect_esp_fetch_data_test();
  Timer0A_Init();

  while(1){
  lightPercentage=(light*100)/4095;
  brightness = GetBrightness(lightPercentage);
  //gasi=MQGetGasPercentage(gas,3);



  /*
    // LED_Status
  if (WIFI_ON==1) {
    if (BLUETOOTH_ON==1) { LED_R=0; LED_G=191; LED_B=255; } // WIFI & BT
    else { LED_R=137; LED_G=218; LED_B=89; } } // ONLY WIFI
  else if (BLUETOOTH_ON==1) { LED_R=0; LED_G=255; LED_B=255; } // Only BT
  else { LED_R=20; LED_G=20; LED_B=20; } // NO CONNECTION (but power)
  */

  // Capacitive Wifi
  if((touch_wifi==1)&&(periodic_wifi==0)){
    if (uploading==0) {Loading(GLACIER_BLUE, brightness, 7);}
    uploading=1;
    AllColor(GLACIER_BLUE, brightness);
    logOwnServer(lightPercentage, temperature, gas);
    uploading=0;
    touch_wifi=0;
  }

  // Periodic Wifi
  if((touch_wifi==0)&&(periodic_wifi==1)){
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
    Loading(BURNT_ORANGE, brightness, 7);
    AllColor(BURNT_ORANGE, brightness);
    logOwnServer(lightPercentage, temperature, gas);
    periodic_wifi=0;
    Timer0A_Init();
  }

  if (gas>=GAS_THRESHOLD) {
    AllColor(RED, brightness);
  }
  else if (temperature<TEMP1) {
    Blinking_Color1_Color2(SKY_BLUE, CADET_BLUE, brightness);
  }
  else if (temperature<TEMP2) {
    Blinking_Color1_Color2(SPRING_GREEN, FOREST_GREEN, brightness);
  }
  else if (temperature<TEMP_THRESHOLD) {
    Blinking_Color1_Color2(SPICES, RED, brightness);
  }
  else {
    AllColor(RED, brightness);
  }

  SysTick_Wait10ms(50);

  } // end while

} // end main
