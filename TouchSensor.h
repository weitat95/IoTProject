// 1) TouchSensor.h
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

//------------Touch_Init------------
// Initialize GPIO Port E bit 0 for input
// Input: none
// Output: none
void Touch_Init(void);
