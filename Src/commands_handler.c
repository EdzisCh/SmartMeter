/*
!Обработчик комманд, поступающих по интерфейсу. 
Комнды, которые поставляются по интерфейсу, должны быть представленны в виде "команда\r\n" или
"команда:аргумент\r\n". Таким образом, любая команда должна заканчиваться "\r\n".
Каждой команде ставиться в соответствие функция имя_выполняемой_команды_callback. Все команды 
представленны в виде массива registered_commands[], состоящий из структур command_defenition. 
Структура состоит из строки-команды имя_команды_cmd и ссылки на функцию, которая ей соответствует. 
Для добавдения новой команды необходимо проверить MAX_CMD_COUNT, добавить строку с командой ниже,
создать новую структуру в функции cmd_handler_commands_init() и добавить ее в массив комманд.
Команды с аргументами. Команды, записаные здесь для обработки, должны заканчиваться ":\r\n".
Для таких команд соответствующая функция принимает аргумент в uint32_t.
*/
#include "commands_handler.h"

char *acknowledge = "\r\n";

//Обрабатываемые комманды
const char* connect_cmd = "connect\r\n";
const char* get_time_cmd = "get_time\r\n";
const char* get_data_cmd = "get_data\r\n";
	
//Данные команды идут с аргументом после ':'
const char* set_hour_cmd = "set_hour:\r\n";
const char* set_minute_cmd = "set_minute:\r\n";
const char* set_day_cmd = "set_day:\r\n";
const char* set_month_cmd = "set_month:\r\n";
const char* set_year_cmd = "set_year:\r\n";
const char* set_time_cmd = "set_time:\r\n";
const char* set_date_cmd = "set_date:\r\n";

//Массив зарегистрированных команд
command_defenition registered_commands[MAX_CMD_COUNT];

//Кол-во зарегистрированных команд (инкрменруется вручную в функции cmd_handler_commands_init())
uint8_t cmd_count;

/*
!Инициализация комманд. Для регсрации команд, необходимо создать новую структуру command_defenition,
прописать ей соответствующую команду-строку и функцию для выполнения и закинуть в массив registered_commands
с индексом cmd_count++.
*/
void cmd_handler_commands_init()
{
	cmd_count = 0;
	
	command_defenition set_hour_cmd_def;
	set_hour_cmd_def.command = set_hour_cmd;
	set_hour_cmd_def.callback_func = set_hour_callback;
	registered_commands[cmd_count++] = set_hour_cmd_def;
	
	command_defenition set_time_cmd_def;
	set_time_cmd_def.command = set_time_cmd;
	set_time_cmd_def.callback_func = set_time_callback;
	registered_commands[cmd_count++] = set_time_cmd_def;
	
	command_defenition set_date_cmd_def;
	set_date_cmd_def.command = set_date_cmd;
	set_date_cmd_def.callback_func = set_date_callback;
	registered_commands[cmd_count++] = set_date_cmd_def;
	
	command_defenition set_minute_cmd_def;
	set_minute_cmd_def.command = set_minute_cmd;
	set_minute_cmd_def.callback_func = set_minute_callback;
	registered_commands[cmd_count++] = set_minute_cmd_def;
	
	command_defenition set_day_cmd_def;
	set_day_cmd_def.command = set_day_cmd;
	set_day_cmd_def.callback_func = set_day_callback;
	registered_commands[cmd_count++] = set_day_cmd_def;
	
	command_defenition set_month_cmd_def;
	set_month_cmd_def.command = set_month_cmd;
	set_month_cmd_def.callback_func = set_month_callback;
	registered_commands[cmd_count++] = set_month_cmd_def;
	
	command_defenition set_year_cmd_def;
	set_year_cmd_def.command = set_year_cmd;
	set_year_cmd_def.callback_func = set_year_callback;
	registered_commands[cmd_count++] = set_year_cmd_def;
	
	command_defenition connect_cmd_def;
	connect_cmd_def.command = connect_cmd;
	connect_cmd_def.callback_func = connect_callback;
	registered_commands[cmd_count++] = connect_cmd_def;
	
	command_defenition get_time_cmd_def;
	get_time_cmd_def.command = get_time_cmd;
	get_time_cmd_def.callback_func = get_time_callback;
	registered_commands[cmd_count++] = get_time_cmd_def;
	
	command_defenition get_data_cmd_def;
	get_data_cmd_def.command = get_data_cmd;
	get_data_cmd_def.callback_func = get_data_callback;
	registered_commands[cmd_count++] = get_data_cmd_def;
	
}

/**
!Обработчик команд. Принимает данные из кольцевого буфера, формирует
строку, вызывает функцию cmd_handler_get_command(), передавая ей команду 
и аргумент
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
!Сравнение двух строк посимвольно. В случае совпадения возвращает 0. Если команда с аргументом -
считывает аргумент и засовывает его в arg
*/
uint8_t cmd_handler_compare_strings(uint8_t str_1[], const char* str_2, uint32_t *arg)
{	
	uint16_t size = strlen(str_2);
	
	for( uint16_t i = 0; i < size; i++)
	{
		if (str_1[i] == ':')
		{
			//Если команда не предпологает наличие аргумента
			if(str_1[i] != str_2[i]) return 1;
			
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
		} else if(str_1[i] != str_2[i])
		{
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
		if(cmd_handler_compare_strings(cmd, registered_commands[i].command, &arg) == 0)
		{
			registered_commands[i].callback_func(arg);
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

/*
! Функция для преобразования десятичного времени в удобочитаемый для человека и
понятный RTC. Если поступет на вход 15 часов (десятичный формат) - на выходе получаем 15
в 16ричном формате (21 в десятичном)
*/
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

uint8_t cmd_handler_get_from_BCD_format( uint32_t val )
{
	if(val > 0x9 && val < 0x20)
	{
		val -= 6;
	} else if (val >= 0x20 && val < 0x30)
	{
		val -= 12;
	} else if (val >= 0x30 && val < 0x40)
	{
		val -= 18;
	} else if (val >= 0x40 && val < 0x50)
	{
		val -= 24;
	} else if(val >= 0x50 && val < 0x60)
	{
		val -= 30;
	}
	
	return val;
}

//===================================================================================

//Функции, соответствующие командам

/*
!
*/
void connect_callback( uint32_t arg )
{	
	char *ack = "ack\r\n";
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, (uint8_t *)ack, 5, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
}

/*
!
*/
void get_time_callback( uint32_t arg )
{
	uint32_t time = rtc_get_time();
	uint32_t date = rtc_get_date();
	
	uint8_t hours = (time & 0x00FF0000) >> 16;
	uint8_t minutes = (time & 0x0000FF00) >> 8;
	uint8_t seconds = (time & 0x000000FF);
	
	uint8_t day = (date & 0x00FF0000) >> 16;
	uint8_t month = (date & 0x0000FF00) >> 8;
	uint8_t year = (date & 0x000000FF);
	
	hours = cmd_handler_get_from_BCD_format(hours);
	minutes = cmd_handler_get_from_BCD_format(minutes);
	seconds = cmd_handler_get_from_BCD_format(seconds);
	
	day = cmd_handler_get_from_BCD_format(day);
	month = cmd_handler_get_from_BCD_format(month);
	year = cmd_handler_get_from_BCD_format(year);
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, &hours, 1, 100);
	HAL_UART_Transmit(&huart5, &minutes, 1, 100);
	HAL_UART_Transmit(&huart5, &seconds, 1, 100);
	HAL_UART_Transmit(&huart5, &day, 1, 100);
	HAL_UART_Transmit(&huart5, &month, 1, 100);
	HAL_UART_Transmit(&huart5, &year, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
}

/*
!
*/
void get_data_callback( uint32_t arg )
{
	char *r = ":";
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.P), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.Q), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.freq), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.I), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.U), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data.cosPhi), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
}

/*
!Меняет часы
*/
void set_hour_callback( uint32_t arg )
{
	if(arg > 23) return;
		
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_hours(arg);
}

/*
!Меняет минут
*/
void set_minute_callback( uint32_t arg )
{
	if(arg > 59) return;
		
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_minutes(arg);
}

/*
!Меняет день
*/
void set_day_callback( uint32_t arg )
{
	if(arg > 31 || arg == 0) return;
		
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_day(arg);
}

/*
!Меняет месяц
*/
void set_month_callback( uint32_t arg )
{
	if(arg > 12 || arg == 0) return;
		
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_month(arg);
}

/*
!Меняет год. Так как год может быть от 00 до 99, проверки нету
*/
void set_year_callback( uint32_t arg )
{
	arg = cmd_handler_to_BCD_format(arg);
	
	rtc_set_year(arg);
}

/*
!Меняет время. На вход должно подаваться время в виде целого числа
hhmmss, где h - часы, m - минуты, s - секунды
*/
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

/*
!Меняет время. На вход должно подаваться время в виде целого числа
ddmmyy, где d - день, m - месяц, y - год
*/
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
