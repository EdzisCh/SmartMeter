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
	UART_HandleTypeDef huart;
	uint32_t data[3]; //data buffer for read and write
	int selectedPage;
	float MCLK;
} CS5490;


void write(CS5490 chip, int page, int address, uint32_t value);
void read(CS5490 chip, int page, int address);
void instruct(CS5490 chip, int instruction);

uint32_t readReg(CS5490 chip, int page, int address);

void reset(CS5490 chip);
void standby(CS5490 chip);
void wakeup(CS5490 chip);

uint32_t getPeakV(CS5490 chip);
uint32_t getPeakI(CS5490 chip);

uint32_t getInstI(CS5490 chip);
uint32_t getInstV(CS5490 chip);
uint32_t getInstP(CS5490 chip);

uint32_t getRmsI(CS5490 chip);
uint32_t getRmsV(CS5490 chip);

uint32_t getAvgP(CS5490 chip);
uint32_t getAvgQ(CS5490 chip);
uint32_t getAvgS(CS5490 chip);

uint32_t getInstQ(CS5490 chip);
uint32_t getPF(CS5490 chip);

uint32_t getTotalP(CS5490 chip);
uint32_t getTotalS(CS5490 chip);
uint32_t getTotalQ(CS5490 chip);

uint32_t getFreq(CS5490 chip);

uint32_t getTime(CS5490 chip);
#endif
