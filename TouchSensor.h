// Name: TouchSensor.c
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide functions to interface the capacitive touch sensor
// Last modified: 10/24/2016
// Hardware connections:
//    Capacitive touch sensor
//    Ground: GND
//    Out: PE0 (DOUT)
//    Time: unconnected but used for RC circuit
//    Vdd: 3.3V DC
// #1 LEDA: PE1 (DIN)

/*************************START*************************/

#include <stdint.h>

//------------Touch_Init------------
// Initialize GPIO Port E bit 0 for input
// Input: none
// Output: none
void Touch_Init(void);
