#ifndef __CS5490_H__
#define __CS5490_H__

#include "stm32l4xx_hal.h"
#include "math.h"
#include "stdint.h"

//Type
#define DCoffset          0x00
#define ACoffset          0x10
#define Gain              0x18
//Channel
#define Current           0x01
#define Voltage           0x02
#define CurrentAndVoltage 0x06

/* Default values */
#define MCLK_default 4.096
#define baudRate_default 600

// all comands templates
#define readByte        0x00
#define writeByte       0x40
#define pageByte        0x80
#define instructionByte 0xC0


typedef struct CS5490
{
	uint8_t data[3]; //data buffer for read and write
	int selectedPage;
	float MCLK;
} CS5490;


void write(CS5490 chip, int page, int address, uint32_t value);
void read(CS5490 chip, int page, int address);
void instruct(CS5490 chip, int instruction);
uint8_t readReg(CS5490 chip, int page, int address);
void setBaudRate(CS5490 chip, uint16_t value);
void getFreq(CS5490 chip);

#endif
