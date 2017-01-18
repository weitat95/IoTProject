// Name: LEDring.h
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide functions to interface the LED ring (16 LEDS with RGB)
// Last modified: 10/24/2016
// Hardware connections:
// LED ring
// #1  Power: 5V DC
//     Data input: PE2 (DOUT)
//     Ground: GND

/*************************START*************************/

#include <stdint.h>
#include <stdbool.h>
//struct RGB LED_Status={0,0,0};

//typedef int bool;
//#define true 1
//#define false 0


//------------LED_Init------------
// Initialize GPIO Port E bit 2 for input.
// Input: none
// Output: none
void LED_Init(void);

//------------LED_Main------------
// Light the different LEDS
// Input: none
// Output: none
void LED_Main(void);

void BlinkingColor(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness);

void Blinking_Color1_Color2(unsigned char RColor1, unsigned char GColor1, unsigned char BColor1, unsigned char RColor2, unsigned char GColor2, unsigned char BColor2, uint32_t brightness);

void Color1_Color2(unsigned char RColor1, unsigned char GColor1, unsigned char BColor1, unsigned char RColor2, unsigned char GColor2, unsigned char BColor2, uint32_t brightness);

void AllColor(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness);

//------------sendBit------------
// Send a bit to the pixel
// Input: bool bitVal
// Output: none
void sendBit( bool bitVal );

//------------sendByte------------
// Function to send a byte to the LED ring
// Input: unsigned char byte
// Output: none
void sendByte( unsigned char byte );

//------------sendPixel------------
// Send a single pixel to the string
// Input: unsigned char r, g, b
// Output: none
void sendPixel( unsigned char r, unsigned char g , unsigned char b );

void show(void);

void Loading(unsigned char RColor, unsigned char GColor, unsigned char BColor, uint32_t brightness, uint32_t delay);

