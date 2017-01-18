//***********************  main.c  ***********************
// Program written by:
// - Steven Prickett  steven.prickett@gmail.com
//
// Brief desicription of program:
// - Initializes an ESP8266 module to act as a WiFi client
//   and fetch weather data from openweathermap.org
//
//*********************************************************
/* Modified by Jonathan Valvano
 Sept 14, 2016
 Out of the box: to make this work you must
 Step 1) Set parameters of your AP in lines 59-60 of esp8266.c
 Step 2) Change line 39 with directions in lines 40-42
 Step 3) Run a terminal emulator like Putty or TExasDisplay at
         115200 bits/sec, 8 bit, 1 stop, no flow control
 Step 4) Set line 50 to match baud rate of your ESP8266 (9600 or 115200)
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/tm4c123gh6pm.h"

#include "pll.h"
#include "UART.h"
#include "esp8266.h"
#include "LED.h"
#include "ST7735.h"
#include "switches.h"
#include "ADCSWTrigger.h"
#include "timer.h"
// prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void logInfoIntoServer(void);     //Logging info into the embedded server
void logOwnServer(void);
char Fetch[] = "GET /data/2.5/weather?q=Austin%20Texas&APPID=eb229d5312ae3c059b5ab5cea1cc36e0 HTTP/1.1\r\nHost:api.openweathermap.org\r\n\r\n";
// 1) go to http://openweathermap.org/appid#use 
// 2) Register on the Sign up page
// 3) get an API key (APPID) replace the 1234567890abcdef1234567890abcdef with your APPID
char temp[10];
uint32_t Before;
uint32_t Elapsed;
uint32_t Current;
char * ptr;
char sendTimeString[4];
uint32_t sendTime;
char dataString[200];
char buffPtr[4];
uint32_t sw0=0;
uint32_t DEBUG=0;
uint32_t ADCvalue;
void GPIOPortD_Handler(void){
  GPIO_PORTD_ICR_R=0x01;
  DEBUG=1;
  sw0 =Switch_Debounce0();
  //}
  // DEBUG=0;

}
int main(void){  
  
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_WHITE); 
  LED_Init();  
  Switch_Init();
  SysTick_Init();
  ADC0_InitSWTriggerSeq3_Ch9();
  Timer2A_Init();
  Output_Init();       // UART0 only used for debugging
  printf("\n\r-----------\n\rSystem starting...\n\r");
  ESP8266_Init(115200);      // connect to access point, set up as client
  ESP8266_GetVersionNumber();
  while(1){
    ESP8266_GetStatus();
    Before=TIMER2_TAR_R;

    if(ESP8266_MakeTCPConnection("api.openweathermap.org")){ // open socket in server
    LED_GreenOn();
    // ESP8266_SendTCP(Fetch);
      Current=TIMER2_TAR_R;
      Elapsed=(Before-Current-4)&0xFFFFFFFF;
//ptr=returnRXBuffer();
      //strcpy=(ststring,ptr);

      //parse_string(ptr,"{}",buf);
    }
    ESP8266_CloseTCPConnection();

    ptr=returnRXBuffer();
    for(int i=0;i<2000;i++){
      
      if(ptr[i]=='t'){
        
        if(ptr[i+1]=='e'){
          if(ptr[i+2]=='m'){
            if(ptr[i+3]=='p'){
              if(ptr[i+4]=='\"'){
                for(int k=0;k<8;k++){
                  temp[k]=ptr[i+k+6];
                }
              }
            }
          }
        }
      }
    }
    ST7735_OutString("Temp: ");
    for(int i=0;i<10;i++){
      int a=temp[i];
      if((a>=48 && a<=57)|| a==46){
        ST7735_OutChar(temp[i]);
      }
    }

    ST7735_SetCursor(0,1);
    ST7735_OutString("Time: ");
    sendTime= Elapsed*1000/80000000;
    sprintf(sendTimeString,"%d",sendTime);
    ST7735_OutChar(sendTimeString[0]);
    ST7735_OutChar('.');
    ST7735_OutChar(sendTimeString[1]);
    ST7735_OutChar(sendTimeString[2]);
   // ST7735_OutString(temp);

  
    //
    while(Board_Input()==0){// wait for touch
      if(sw0==0x01){
       // ADCvalue=ADC0_InSeq3();
        ADCvalue = (ADC0_InSeq3()*74-849)*100/92375;
        DEBUG=2;
        
        sprintf(buffPtr,"%d",ADCvalue);
        ST7735_SetCursor(0,2);
        ST7735_SetTextColor(ST7735_BLACK);
        ST7735_OutString("Voltage~");
        if(buffPtr[2]!=NULL){
          ST7735_OutChar(buffPtr[0]);
          ST7735_OutChar('.');
          ST7735_OutChar(buffPtr[1]);
          ST7735_OutChar(buffPtr[2]);
        }else if(buffPtr[1]!=NULL){
          ST7735_OutString("0.");
          ST7735_OutChar(buffPtr[0]);
          ST7735_OutChar(buffPtr[1]);
        }else {
          ST7735_OutString("0.0");
          ST7735_OutChar(buffPtr[0]);
        }
        ST7735_OutString(" V");
       // logInfoIntoServer();
        logOwnServer();
        ST7735_SetCursor(0,3);
        ST7735_OutString("Time: ");
        ST7735_SetCursor(0,1);
        sendTime= Elapsed*100/80000000;
        sprintf(sendTimeString,"%d",sendTime);
        ST7735_OutChar(sendTimeString[0]);
        ST7735_OutChar('.');
        ST7735_OutChar(sendTimeString[1]);
        ST7735_OutChar(sendTimeString[2]);
        sw0=0;
      }
    }; 
    //ptr=returnRXBuffer();

    LED_GreenOff();
    LED_RedToggle();
    
  }
}

// transparent mode for testing
void ESP8266SendCommand(char *);
int main2(void){  char data;
  DisableInterrupts();
  PLL_Init(Bus80MHz);

  LED_Init();  
  Output_Init();       // UART0 as a terminal
  printf("\n\r-----------\n\rSystem starting at 9600 baud...\n\r");
//  ESP8266_Init(38400);
  ESP8266_InitUART(9600,true);
  ESP8266_EnableRXInterrupt();
  EnableInterrupts();
  ESP8266SendCommand("AT+RST\r\n");
  data = UART_InChar();
//  ESP8266SendCommand("AT+UART=115200,8,1,0,3\r\n");
//  data = UART_InChar();
//  ESP8266_InitUART(115200,true);
//  data = UART_InChar();
  
  while(1){
// echo data back and forth
    data = UART_InCharNonBlock();
    if(data){
      ESP8266_PrintChar(data);
    }
  }
}

char data[6];
char fullData[200];
char data2[20];
void logInfoIntoServer(void){ 
  char endOfData[]="%20V&edxcode=8086 HTTP/1.1\r\nUser-Agent: Keil\r\nHost: embedded-systems-server.appspot.com\r\n\r\n";
  strcpy(dataString,"GET /query?city=Austin%20Texas&id=Wei%20and%20Corentin&greet=Voltage%3D");
  //strcat(dataString,data);
  //strcat(dataString,endOfData);
  if(buffPtr[2]!=NULL){
    data[0]=buffPtr[0];
    data[4]=buffPtr[1];
    data[5]=buffPtr[2];
  }else if(buffPtr[1]!=NULL){
    data[0]='0';
    data[4]=buffPtr[0];
    data[5]=buffPtr[1];
  }else{
    data[0]='0';
    data[4]='0';
    data[5]=buffPtr[1];
  }
  data[1]='%';
  data[2]='2';
  data[3]='E';
  
  
  sprintf(fullData,"%s%s%s",dataString,data,endOfData); 
  if(ESP8266_MakeTCPConnection("embedded-systems-server.appspot.com")){
    
    LED_RedOn();
    
   // ESP8266_SendTCP("GET /query?city=Austin%20Texas&id=Jonathan%20Valvano&greet=Int%20Temp%3D21C&edxcode=8086 HTTP/1.1\r\nUser-Agent: Keil\r\nHost: embedded-systems-server.appspot.com\r\n\r\n");
    Before=TIMER2_TAR_R;
    ESP8266_SendTCP(fullData);
    Current=TIMER2_TAR_R;
    Elapsed=(Before-Current-4)&0xFFFFFFFF;
  }
  ESP8266_CloseTCPConnection();
}

void logOwnServer(void){
  char endOfData[]="%20C&edxcode=8086 HTTP/1.1\r\nUser-Agent: Keil\r\nHost: emb445l.appspot.com\r\n\r\n";
  strcpy(dataString,"GET /query?city=Austin%20Texas&id=Wei%20and%20Corentin&data1=Voltage%3D");
//strcat(dataString,data);
//strcat(dataString,endOfData);
  if(buffPtr[2]!=NULL){
    data[0]=buffPtr[0];
    data[4]=buffPtr[1];
    data[5]=buffPtr[2];
  }else if(buffPtr[1]!=NULL){
    data[0]='0';
    data[4]=buffPtr[0];
    data[5]=buffPtr[1];
  }else{
    data[0]='0';
    data[4]='0';
    data[5]=buffPtr[1];
  }
  data[1]='%';
  data[2]='2';
  data[3]='E';
  
  char data2Header[]="%20V&data2=Temp%3D";
  data2[0]=temp[0];
  data2[1]=temp[1];
  data2[2]=temp[2];
  data2[3]='%';
  data2[4]='2';
  data2[5]='E';
  data2[6]=temp[4];
  data2[7]=temp[5];
  
  sprintf(fullData,"%s%s%s%s%s",dataString,data,data2Header,data2,endOfData);  
  if(ESP8266_MakeTCPConnection("emb445l.appspot.com")){
    
    LED_BlueToggle();
    
   // ESP8266_SendTCP("GET /query?city=Austin%20Texas&id=Jonathan%20Valvano&data1=Int%20Temp%3D21C&data2=Int%20Temp%3D55C&edxcode=8086 HTTP/1.1\r\nUser-Agent: Keil\r\nHost: emb445l.appspot.com\r\n\r\n");
    ESP8266_SendTCP(fullData);
  }
  ESP8266_CloseTCPConnection();
  
}