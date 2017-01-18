// Name: barometer.c
// Authors: Corentin Dugue & Wei Tat Lee
// Creation: 10/24/2016
// Description: Provide initialization functions of the barometer BMP280
// Last modified: 10/24/2016

/*************************START*************************/

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "barometer.h"
uint8_t Rx,chipID,Tx;
bmp280_calib_data _bmp280_calib;
uint8_t DEBUG2;
//********BARO_Init*****************
// Initialize BMP280 Barometer
// inputs:  none
// outputs: none
void Baro_Init(void) {
  SYSCTL_RCGCSSI_R |= 0x02; // activate SSI1
  SYSCTL_RCGCGPIO_R |= 0x08; // activate port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){};// ready?

  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0000)+0x00002202;
  GPIO_PORTD_AMSEL_R = 0;         // disable analog functionality on PD
  GPIO_PORTD_AFSEL_R |= 0x0D;    // enable alt funct on PD0,2,3
  GPIO_PORTD_DIR_R |= 0x02;       // make PD1 (FSS/!CS) output
  GPIO_PORTD_DEN_R |= 0x0F;       // configure PD0,1,2,3 as SSI (Digital I/O)

  SSI1_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI1_CPSR_R = 0x08;             // 10 MHz SSIClk (80Mhz Bus/8)
  SSI1_CR0_R = (SSI1_CR0_R&~0x0000FFFF)+ 0x00000007; // SPH = 1, SPO = 1, FRF=Freescale(0), DSS=8Bit
  SSI1_CR1_R |= 0x00000002;       // enable SSI
  PD1=0x02;                       // !CS/FSS = 1;
}

//********sendAndRead*****************
// Send 8-bit control byte to the SSI and return 8-bit Data
// inputs:  1bit (R/W : 1/0) 7bit (Register Address)
// outputs: 8bit data received from BMP280
uint8_t sendAndRead(uint8_t code){
  while((SSI1_SR_R&0x00000001)==0){};//wait until FIFO empty
  SSI1_DR_R=code;                    //data out
  while((SSI1_SR_R&SSI_SR_RNE)==0){};//wait until response
  Rx=SSI1_DR_R;
  return Rx;
}

uint8_t read8(uint8_t reg){
  uint16_t data;
  PD1=0;                  //!CS/FSS=0;
  sendAndRead(reg|0x80);  //ensure READ operation
  data=sendAndRead(0);
  PD1=0x02;               //!CS/FSS=1;
  return data;
}

uint16_t read16(uint8_t reg){
  uint16_t data;
  PD1=0;                  //!CS/FSS=0;
  sendAndRead(reg|0x80);  //ensure READ operation
  data=(sendAndRead(0)<<8)|sendAndRead(0);
  PD1=0x02;               //!CS/FSS=1;
  return data;
}

int16_t readS16(uint8_t reg){
  return (int16_t)read16(reg);
}

uint32_t read24(uint8_t reg){
  uint32_t data;
  PD1 = 0;                //!CS/FSS=0;
  sendAndRead(reg|0x80);  //ensure READ operation
  data = sendAndRead(0);
  data <<= 8;
  data |= sendAndRead(0);
  data <<= 8;
  data |= sendAndRead(0);
  PD1 = 0x02;             //!CS/FSS=1;
  return data;
}


//return result in Little Endian
uint16_t read16_LE(uint8_t reg){
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
}

//read a signed 16 bit in Little Endian
int16_t readS16_LE(uint8_t reg){
  return (int16_t)read16_LE(reg);
}

void write8(uint8_t reg,uint8_t value){
  PD1=0;                  //!CS/FSS=0;
  sendAndRead(reg&~0x80);  //ensure WRITE operation
  sendAndRead(value);
  PD1=0x02;               //!CS/FSS=1;
}

void readCalibrationPara(void){
  _bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
  _bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
  _bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

  _bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
  _bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
  _bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
  _bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
  _bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
  _bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
  _bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
  _bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
  _bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

uint8_t StartBarometer(void){
    DEBUG2=1;
    //write8(BMP280_REGISTER_SOFTRESET, 0xB6);
    chipID=read8(BMP280_REGISTER_CHIPID);
    if(chipID != 0x58){
      return 0;
    }
    DEBUG2=10;
    readCalibrationPara();
    write8(BMP280_REGISTER_CONTROL, 0x3F);
    //write8(BMP280_REGISTER_CONFIG, 0x00);
    return 1;
}
//used to store the fine resolution temperature value for later;
int32_t t_fine;
int32_t temp_var1;
int32_t temp_var2;
int32_t temp_raw;
int32_t calib1;
int32_t calib2;
int32_t calib3;
//resolution of 0.01
int32_t ReadTemperature(void){

  DEBUG2=2;
  int32_t var1, var2,T;

  int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
  temp_raw=adc_T;
  adc_T>>=4;
  calib1=(int32_t)_bmp280_calib.dig_T1;
  calib2=(int32_t)_bmp280_calib.dig_T2;
  calib3=(int32_t)_bmp280_calib.dig_T3;

  var1  = ((((adc_T>>3) - (((int32_t)_bmp280_calib.dig_T1 )<<1))) *
	   ((int32_t)_bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1)) *
	     ((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
	   ((int32_t)_bmp280_calib.dig_T3)) >> 14;
  temp_var1=var1;
  temp_var2=var2;
  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  //return T/100;
  return T;
}

//resolution (1/256)
int64_t ReadPressure(void){
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up
  ReadTemperature();

  int32_t adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)_bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)_bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)_bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmp280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)_bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7)<<4);
  //return (float)p/256;
  return p;
}

//Read Altitude?
