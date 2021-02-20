#include "commands_hendler.h"

const char* led_on_0 = "led_on_0\r\n";
const char* led_on_1 = "led_on_1\r\n";
const char* led_off = "led_off\r\n";
const char* set_i = "set_i\r\n";
const char* set_hour = "set_hour\r\n";

command_defenition regitered_commands[MAX_CMD_COUNT];
uint8_t cmd_count;

/*
!Инициализация комманд
*/
void commands_init()
{
	cmd_count = 0;
	
	command_defenition led_on_act;
	led_on_act.command = led_on_0;
	led_on_act.callback_func = led_on_act_1;
	regitered_commands[0] = led_on_act;
	cmd_count++;
	
	command_defenition led_on_react;
	led_on_react.command = led_on_1;
	led_on_react.callback_func = led_on_react_1;
	regitered_commands[1] = led_on_react;
	cmd_count++;
	
	command_defenition led_off_;
	led_off_.command = led_off;
	led_off_.callback_func = led_off_1;
	regitered_commands[2] = led_off_;
	cmd_count++;
	
	command_defenition set_i_;
	set_i_.command = set_i;
	set_i_.callback_func = set_i_1;
	regitered_commands[3] = set_i_;
	cmd_count++;
	
	command_defenition set_hour_;
	set_i_.command = set_hour;
	set_i_.callback_func = set_hour_1;
	regitered_commands[4] = set_hour_;
	cmd_count++;
	
}

/**
!Обработчик команд. Принимает данные из кольцевого буфера, формирует
стоку, проверяет наличие команды и вызывает соответствующую функцию
*/
void execute_command( ring_buf *ring, uint8_t count )
{
	
	uint8_t cmd[count+1];
	uint8_t i = 0;
	while(ring->out_index < ring->in_index)
	{
		cmd[i] = ring_buff_pop(ring);
		i++;
	}
	
	uint32_t arg;
	
	get_command(cmd, arg);
}

/*
!Сравнение двух строк посимвольно 
*/
uint8_t compare_strings(uint8_t str_1[], const char* str_2, uint32_t *arg)
{
	// валидация 1 строки - размер 0, нет CR
	uint8_t length = sizeof(str_1)/sizeof(uint8_t);
	if(length == 0) return 0x01;
	//if(str_1[length-2] != '\r' || str_1[length-1] != '\n') return 0x01;
	
	uint16_t size = strlen(str_2);
	uint16_t i = 0;
	for( ; i < size; i++)
	{
		if(str_1[i] == ':')
		{
			
			uint8_t k = ++i;
			int tmp = 0;
			
			while(str_1[k] != '\r')
			{
				tmp *= 10;
				tmp += (int)str_1[k] - 48;
				k++;
			}
			
			*arg = tmp;
			
			return 0;
		} else if (str_1[i] != str_2[i]){
			return 1;	
		}
	}
	
	return 0;
}

/*
!Поиск нужной комманды в массиве комманд и вызов соответствующей команде функции
*/
uint8_t get_command(uint8_t cmd[], uint32_t arg)
{
		uint8_t flag = 0;
	uint8_t i = 0;
	
	while(flag == 0)
	{
		if(compare_strings(cmd, regitered_commands[i].command, &arg) == 0)
		{
			regitered_commands[i].callback_func(arg);
			return 0;
		}
		i++;
		if( i == MAX_CMD_COUNT || i == cmd_count )
		{
			flag = 1;
		} 
	}
	return 0x01;
}

//test
void led_on_act_1( uint32_t arg )
{
	
}

void led_on_react_1( uint32_t arg )
{
	
}

void led_off_1( uint32_t arg )
{
	
}

void set_i_1( uint32_t arg )
{
	
}

void set_hour_1( uint32_t arg )
{
	if(arg > 23) return;
	
	if(arg > 9 && arg < 16)
	{
		arg += 6;
		rtc_set_hours(arg);
	}
}
