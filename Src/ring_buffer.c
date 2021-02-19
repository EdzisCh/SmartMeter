#include "ring_buffer.h"

void ring_buff_put( uint8_t symbol, ring_buf* ring)
{
	ring->buffer[ring->in_index++] = symbol;
	if(ring->in_index >= ring->size)
	{
		ring->in_index = 0;
	}
}

uint8_t ring_buff_pop( ring_buf *ring )
{
	uint8_t value = ring->buffer[ring->out_index++];
	
	if(ring->out_index >= ring->size)
	{
		ring->out_index = 0;
	}
	
	return value;
}

uint16_t ring_buff_get_count( ring_buf *ring )
{
	uint16_t count = 0;
	
	if(ring->in_index < ring->out_index)
	{
		count = ring->size + ring->in_index - ring->out_index;
	} else {
		count = ring->in_index - ring->out_index;
	}
	
	return count;
}

void ring_buff_clear( ring_buf *ring )
{
	ring->in_index = 0;
	ring->out_index = 0;
}

uint8_t ring_buff_init( ring_buf *ring, uint8_t *buf, uint16_t size )
{
	ring->size = size;
	ring->buffer = buf;
	
	ring_buff_clear(ring);
	
	return (ring->buffer ? HAL_OK : HAL_ERROR);
}

