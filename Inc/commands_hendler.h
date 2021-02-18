#ifndef _COMMANDS_HENDLER_H_
#define _COMMANDS_HENDLER_H_

#include "stm32l4xx_hal.h"
#include "string.h"

typedef struct command
{
	uint8_t *command;
	//link to cmd
} command;

void execute_command( uint8_t *command );

#endif
