#include "pass_handler.h"

uint32_t first_pass;
uint32_t second_pass;

uint8_t first_pass_on = 0;
uint8_t second_pass_on = 0;

/*
!��������� true, ���� ���� � ������� �������� �� 1 ������
*/
uint8_t pass_handler_is_first_pass_on( void )
{
	return first_pass_on;
}

/*
!��������� true, ���� ���� � ������� �������� �� 2 ������
*/
uint8_t pass_handler_is_second_pass_on( void )
{
	return second_pass_on;
}

/*
!��������� true, ���� ���� � ������� �������� �� ������ ������
*/
uint8_t pass_handler_is_any_pass_on( void )
{
	if(first_pass_on || second_pass_on)
	{
		return 1;
	}
	
	return 0;
}

/*
!������������� �������, ����������� � ������
*/
void pass_handler_passwords_init( void )
{
	uint16_t first_pass_addr = 33099;
	uint16_t second_pass_addr = 33103;
	m24m01_get_from_mem(first_pass_addr, (uint8_t*)&(first_pass), 4);
	m24m01_get_from_mem(second_pass_addr, (uint8_t*)&(second_pass), 4);
	
	//first_pass = 123456;
	//second_pass = 654321;
}

/*
!��������� ������������ ��������� ��������� ������ �� �������
*/
uint8_t pass_handler_check_pass( uint32_t pass )
{
	if(pass == first_pass)
	{
		return 1;
	} else if( pass == second_pass)
	{
		return 2;
	}
	
	return 0;
}

/*
!����� ������ 2
*/
void pass_handler_set_second_pass( uint32_t pass )
{
	uint16_t second_pass_addr = 33103;
	
	second_pass = pass;
	
	m24m01_save_to_mem(second_pass_addr, (uint8_t*)&(pass), 4);
}

/*
!����� ������ 1
*/
void pass_handler_set_first_pass( uint32_t pass )
{
	uint16_t first_pass_addr = 33099;
	
	first_pass = pass;
	
	m24m01_save_to_mem(first_pass_addr, (uint8_t*)&(pass), 4);
}

/*
!���� � �������. �������� ����� ������ �� �������
*/
void pass_handler_set_pass_on( uint8_t number_of_pass )
{
	if(number_of_pass == 1)
	{
		first_pass_on = 1;
	} else if (number_of_pass == 2)
	{
		second_pass = 1;
	}
}

/*
!����� ������ ����� - ����� �� �������
*/
void pass_handler_set_pass_off( void )
{
	first_pass_on = 0;
	second_pass_on = 0;
}

/*
!������ 1 ������
*/
uint32_t pass_handler_get_first_pass( void )
{
	return first_pass;
}

/*
!������ 2 ������
*/
uint32_t pass_handler_get_second_pass( void )
{
	return second_pass;
}