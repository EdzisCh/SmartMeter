#include "commands_hendler.h"

const char* led_on_0 = "led_on_0\r\n";
const char* led_on_1 = "led_on_1\r\n";
const char* led_off = "led_off\r\n";
const char* set_i = "set_i\r\n";
const char* set_hour_cmd = "set_hour\r\n";
const char* set_time_cmd = "set_time\r\n";
const char* set_date_cmd = "set_date\r\n";

command_defenition regitered_commands[MAX_CMD_COUNT];
uint8_t cmd_count;

/*
!Инициализация комманд
*/
void cmd_handler_commands_init()
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
	
	command_defenition set_hour_cmd_def;
	set_hour_cmd_def.command = set_hour_cmd;
	set_hour_cmd_def.callback_func = set_hour_callback;
	regitered_commands[3] = set_hour_cmd_def;
	cmd_count++;
	
	command_defenition set_time_cmd_def;
	set_time_cmd_def.command = set_time_cmd;
	set_time_cmd_def.callback_func = set_time_callback;
	regitered_commands[4] = set_time_cmd_def;
	cmd_count++;
	
	command_defenition set_date_cmd_def;
	set_date_cmd_def.command = set_date_cmd;
	set_date_cmd_def.callback_func = set_date_callback;
	regitered_commands[5] = set_date_cmd_def;
	cmd_count++;
	
}

/**
!Обработчик команд. Принимает данные из кольцевого буфера, формирует
стоку, проверяет наличие команды и вызывает соответствующую функцию
*/
void cmd_handler_execute_command( ring_buf *ring, uint8_t count )
{
	
	uint8_t cmd[count+1];
	uint8_t i = 0;
	while(ring->out_index < ring->in_index)
	{
		cmd[i] = ring_buff_pop(ring);
		i++;
	}
	
	uint32_t arg;
	
	cmd_handler_get_command(cmd, arg);
}

/*
!Сравнение двух строк посимвольно 
*/
uint8_t cmd_handler_compare_strings(uint8_t str_1[], const char* str_2, uint32_t *arg)
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
uint8_t cmd_handler_get_command(uint8_t cmd[], uint32_t arg)
{
		uint8_t flag = 0;
	uint8_t i = 0;
	
	while(flag == 0)
	{
		if(cmd_handler_compare_strings(cmd, regitered_commands[i].command, &arg) == 0)
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
	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
}

void led_on_react_1( uint32_t arg )
{
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_SET);
}

void led_off_1( uint32_t arg )
{
	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
}

void set_i_1( uint32_t arg )
{
	
}

void set_hour_callback( uint32_t arg )
{
	if(arg > 23) return;
		
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_hours(arg);
}

uint8_t cmd_handler_to_BCD_format( uint8_t arg )
{
	if(arg > 9 && arg < 20)
	{
		arg += 6;
	} else if (arg >= 20 && arg < 30)
	{
		arg += 12;
	} else if (arg >= 30 && arg < 40)
	{
		arg += 18;
	} else if (arg >= 40 && arg < 50)
	{
		arg += 24;
	} else if(arg >= 50 && arg < 60)
	{
		arg += 30;
	}
	
	return arg;
}

void set_time_callback( uint32_t arg )
{
	uint8_t hour = arg / 10000;
	if(hour > 23) return;
	uint8_t minute = (arg / 100) % 100;
	if(minute > 59) return;
	uint8_t second = arg % 100;
	if(second > 59) return;
	
	hour = cmd_handler_to_BCD_format(hour);
	minute = cmd_handler_to_BCD_format(minute);
	second = cmd_handler_to_BCD_format(second);
	
	uint32_t time = 0;
	time += hour << 16;
	time += minute << 8;
	time += second;
	
	rtc_set_time(time);
}

void set_date_callback( uint32_t arg )
{
	uint8_t day = arg / 10000;
	if(day > 31 || day == 0) return;
	uint8_t month = (arg / 100) % 100;
	if(month > 12 || month == 0) return;
	uint8_t year = arg % 100;
	
	day = cmd_handler_to_BCD_format(day);
	month = cmd_handler_to_BCD_format(month);
	year = cmd_handler_to_BCD_format(year);
	
	uint32_t date = 0;
	date += day << 16;
	date += month << 8;
	date += year;
	
	rtc_set_date(date);
}
