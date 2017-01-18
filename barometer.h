// Name: barometer.h
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide initialization functions of the barometer BMP280
// Last modified: 10/24/2016
// Hardware connections:
//     Barometer
// #1  Power: 3.3V DC
//     Ground: GND
//   SSI1Clk (SCK) connected to PD0
//   SSI1Fss (!CS) connected to PD1 (GPIO controlled)
//   SSI1Rx  (SDO) connected to PD2
//   SSI1Tx  (SDI) connected to PD3

/*************************START*************************/

#include <stdint.h>

#define PD1 (*((volatile unsigned long *) 0x40007008))


#define BMP280_REGISTER_DIG_T1               0x88
#define BMP280_REGISTER_DIG_T2               0x8A
#define BMP280_REGISTER_DIG_T3               0x8C

#define BMP280_REGISTER_DIG_P1               0x8E
#define BMP280_REGISTER_DIG_P2               0x90
#define BMP280_REGISTER_DIG_P3               0x92
#define BMP280_REGISTER_DIG_P4               0x94
#define BMP280_REGISTER_DIG_P5               0x96
#define BMP280_REGISTER_DIG_P6               0x98
#define BMP280_REGISTER_DIG_P7               0x9A
#define BMP280_REGISTER_DIG_P8               0x9C
#define BMP280_REGISTER_DIG_P9               0x9E

#define BMP280_REGISTER_CHIPID               0xD0
#define BMP280_REGISTER_VERSION              0xD1
#define BMP280_REGISTER_SOFTRESET            0xE0

#define BMP280_REGISTER_CAL26                0xE1  // R calibration stored in 0xE1-0xF0

#define BMP280_REGISTER_CONTROL              0xF4
#define BMP280_REGISTER_CONFIG               0xF5
#define BMP280_REGISTER_PRESSUREDATA         0xF7
#define BMP280_REGISTER_TEMPDATA             0xFA

/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
    typedef struct
    {
      uint16_t dig_T1;
      int16_t  dig_T2;
      int16_t  dig_T3;

      uint16_t dig_P1;
      int16_t  dig_P2;
      int16_t  dig_P3;
      int16_t  dig_P4;
      int16_t  dig_P5;
      int16_t  dig_P6;
      int16_t  dig_P7;
      int16_t  dig_P8;
      int16_t  dig_P9;

      uint8_t  dig_H1;
      int16_t  dig_H2;
      uint8_t  dig_H3;
      int16_t  dig_H4;
      int16_t  dig_H5;
      int8_t   dig_H6;
    } bmp280_calib_data;
/*=========================================================================*/

//********sendAndRead*****************
// Send 8-bit control byte to the SSI and return 8-bit Data
// inputs:  1bit (R/W : 1/0) 7bit (Register Address)
// outputs: 8bit data received from BMP280
uint8_t sendAndRead(uint8_t code);

//********SSI helper Functions*****************
uint8_t read8(uint8_t reg);
uint16_t read16(uint8_t reg);
int16_t readS16(uint8_t reg);
uint32_t read24(uint8_t reg);
uint16_t read16_LE(uint8_t reg);
int16_t readS16_LE(uint8_t reg);
void write8(uint8_t reg,uint8_t value);
//**********************************************

//********readCalibrationPara*******************
// Reading the calibration data from the BMP280 chip
// inputs:  none
// outputs: none
void readCalibrationPara(void);


//*********Baro_Init*************************
// Initialize the Ports for BmP280 sensor Using SSI (PD0,PD1,PD2,PD3)
// Input: none
// Output: none
void Baro_Init(void);

//*********StartBarometer********************
// Initialize the Barometer BMP280
// Input: none
// Output: none
uint8_t StartBarometer(void);


int32_t ReadTemperature(void);
int64_t ReadPressure(void);
