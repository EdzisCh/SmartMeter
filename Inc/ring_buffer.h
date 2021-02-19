#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include "stm32l4xx_hal.h"

typedef struct ring_buffer
{
	uint8_t *buffer;
	uint16_t in_index;
	uint16_t out_index;
	uint16_t size;
} ring_buf;


void ring_buff_put( uint8_t symbol, ring_buf *ring);
uint8_t ring_buff_pop( ring_buf *ring );
uint16_t ring_buff_get_count( ring_buf *ring );
int32_t ring_buff_show_symbol( uint16_t symbol_num, ring_buf *ring );
void ring_buff_clear( ring_buf *ring );
uint8_t ring_buff_init( ring_buf *ring, uint8_t *buf, uint16_t size );


#endif
