// Name: Colors.h
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 12/22/2016
// Description: Provide RGB code for different colors
// Last modified: 12/22/2016

/*************************START*************************/

#include <stdint.h>


struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// White
#define WHITE_LOW 20, 20, 20


// Blue
#define SKY_BLUE 0, 191, 255
#define CADET_BLUE 0, 68, 69
#define GLACIER_BLUE 25,149,173

// Red
#define RED 198, 0, 0
#define BURNT_ORANGE 222,122,34
#define SPICES 233,79,8



// Green
#define SPRING_GREEN 137,218,89
#define FOREST_GREEN 46,70,0

