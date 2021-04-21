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
#include "stdlib.h"

char *acknowledge = "\r\n";

//Обрабатываемые комманды
const char* get_time_cmd = "get_time\r\n";
const char* get_data_cmd = "get_data\r\n";
const char* calibrate_cmd = "calibrate\r\n";
const char* end_connection_cmd = "end_connection\r\n";
const char* reset_meters_cmd = "reset_meters\r\n";
const char* get_event_notes_cmd = "get_event_notes\r\n";
const char* get_ter_data_cmd = "get_ter_data\r\n";
	
//Данные команды идут с аргументом после ':'
const char* connect_cmd = "connect:\r\n";
const char* set_first_pass_cmd = "set_first_pass:\r\n";
const char* set_second_pass_cmd = "set_second_pass:\r\n";
const char* set_time_cmd = "set_time:\r\n";
const char* set_date_cmd = "set_date:\r\n";
const char* set_daylight_cmd = "set_daylight:\r\n";
const char* change_daylight_cmd = "change_daylight:\r\n";

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
	
	//1
	command_defenition set_time_cmd_def;
	set_time_cmd_def.command = set_time_cmd;
	set_time_cmd_def.callback_func = set_time_callback;
	registered_commands[cmd_count++] = set_time_cmd_def;
	
	//2
	command_defenition set_date_cmd_def;
	set_date_cmd_def.command = set_date_cmd;
	set_date_cmd_def.callback_func = set_date_callback;
	registered_commands[cmd_count++] = set_date_cmd_def;
	
	//3
	command_defenition connect_cmd_def;
	connect_cmd_def.command = connect_cmd;
	connect_cmd_def.callback_func = connect_callback;
	registered_commands[cmd_count++] = connect_cmd_def;
	
	//4
	command_defenition get_time_cmd_def;
	get_time_cmd_def.command = get_time_cmd;
	get_time_cmd_def.callback_func = get_time_callback;
	registered_commands[cmd_count++] = get_time_cmd_def;
	
	//5
	command_defenition get_data_cmd_def;
	get_data_cmd_def.command = get_data_cmd;
	get_data_cmd_def.callback_func = get_data_callback;
	registered_commands[cmd_count++] = get_data_cmd_def;
	
	//6
	command_defenition calibrate_cmd_def;
	calibrate_cmd_def.command = calibrate_cmd;
	calibrate_cmd_def.callback_func = calibrate_callback;
	registered_commands[cmd_count++] = calibrate_cmd_def;
	
	//8
	command_defenition end_connection_def;
	end_connection_def.command = end_connection_cmd;
	end_connection_def.callback_func = end_connection_callback;
	registered_commands[cmd_count++] = end_connection_def;
	
	//9
	command_defenition set_first_pass_def;
	set_first_pass_def.command = set_first_pass_cmd;
	set_first_pass_def.callback_func = set_first_pass_callback;
	registered_commands[cmd_count++] = set_first_pass_def;
	
	//10
	command_defenition set_second_pass_def;
	set_second_pass_def.command = set_second_pass_cmd;
	set_second_pass_def.callback_func = set_second_pass_callback;
	registered_commands[cmd_count++] = set_second_pass_def;
	
	//11
	command_defenition reset_meters_def;
	reset_meters_def.command = reset_meters_cmd;
	reset_meters_def.callback_func = reset_meters_callback;
	registered_commands[cmd_count++] = reset_meters_def;
	
	//12
	command_defenition set_daylight_def;
	set_daylight_def.command = set_daylight_cmd;
	set_daylight_def.callback_func = set_daylight_callback;
	registered_commands[cmd_count++] = set_daylight_def;
	
	//13
	command_defenition change_daylight_def;
	change_daylight_def.command = change_daylight_cmd;
	change_daylight_def.callback_func = change_daylight_callback;
	registered_commands[cmd_count++] = change_daylight_def;
	
	//14
	command_defenition get_event_notes_def;
	get_event_notes_def.command = get_event_notes_cmd;
	get_event_notes_def.callback_func = get_event_notes_callback;
	registered_commands[cmd_count++] = get_event_notes_def;
	
	//15
	command_defenition get_ter_data_def;
	get_ter_data_def.command = get_ter_data_cmd;
	get_ter_data_def.callback_func = get_ter_data_callback;
	registered_commands[cmd_count++] = get_ter_data_def;
}

/**
!Обработчик команд. Принимает данные из кольцевого буфера, формирует
строку, вызывает функцию cmd_handler_get_command(), передавая ей команду 
и аргумент
*/
void cmd_handler_execute_command( ring_buf *ring, uint8_t count )
{
	uint8_t cmd[40];

	uint8_t i = 0;
	while(ring->out_index < ring->in_index)
	{
		cmd[i] = ring_buff_pop(ring);
		i++;
	}
	
	uint32_t arg = 0;
	
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
!Начало работы по rs485. Проверка наличия входа в систему,
проверка пороля и регистрация события
*/
void connect_callback( uint32_t arg )
{	
	if(pass_handler_is_any_pass_on())
	{
		return;
	}
	
	char * ack;
	
	uint8_t res = pass_handler_check_pass(arg);
	
	if(res == 0)
	{
		display_clear_pass_symbols();
		ack = "!ack\r\n";
		display_P();
	} else if (res == 1){
		display_clear_pass_symbols();
		ack = "1ack\r\n";
		display_N();
	}	else if (res == 2){
		display_clear_pass_symbols();
		ack = "2ack\r\n";
		display_N();
	}
	
	pass_handler_set_pass_on(res);
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, (uint8_t *)ack, 6, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_off();
}

/*
!Завершение работы по rs485.
*/
void end_connection_callback( uint32_t arg )
{	
	pass_handler_set_pass_off();
	display_clear_pass_symbols();
}

/*
!Смена первого пороля
*/
void set_first_pass_callback( uint32_t arg )
{	
	if(!pass_handler_is_first_pass_on())
	{
		return;
	}

	if(arg == pass_handler_get_second_pass())
	{
		return;
	}
	
	arg = 123456;
	pass_handler_set_first_pass(arg);
}

/*
!Смена второго пороля
*/
void set_second_pass_callback( uint32_t arg )
{	
	if(!pass_handler_is_first_pass_on())
	{
		return;
	}
	
	if(arg == pass_handler_get_first_pass())
	{
		return;
	}
	
	arg = 654321;
	pass_handler_set_second_pass(arg);
}

/*
!Перезагрузка измерителей (init функция)
*/
void reset_meters_callback( uint32_t arg )
{
	if(!pass_handler_is_any_pass_on())
	{
		return;
	}
	
	phase_1_enable = 0;
	phase_2_enable = 0;
	phase_3_enable = 0;
	
	display_energy_clear();
	display_clear_units();
	display_clear_main_numbers();
	display_clear_phases();
	
	uint8_t init = cs5490_init(&chip_L1, &chip_L2, &chip_L3);
	if(init == 0)
	{
		display_L1();
		display_L2();
		display_L3();
		phase_1_enable = 1;
		phase_2_enable = 1;
		phase_3_enable = 1;
	} else if(init == 2)
	{
		display_L1();
		phase_1_enable = 1;
	} else if(init == 3)
	{
		display_L1();
		display_L2();
		phase_1_enable = 1;
		phase_2_enable = 1;
	} 
}

/*
!Отправка текущего времени/даты счетчика
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
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, &hours, 1, 100);
	HAL_UART_Transmit(&huart5, &minutes, 1, 100);
	HAL_UART_Transmit(&huart5, &seconds, 1, 100);
	HAL_UART_Transmit(&huart5, &day, 1, 100);
	HAL_UART_Transmit(&huart5, &month, 1, 100);
	HAL_UART_Transmit(&huart5, &year, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_on();
}

/*
!Функция отправки данных из РОН
*/
void get_data_callback( uint32_t arg )
{
	char *r = ":";
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.P), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.Q), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.F), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.I), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.U), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_1.PF), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.P), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.Q), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.F), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.I), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.U), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_2.PF), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.P), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.Q), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.F), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.I), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.U), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *)(&current_data_3.PF), 8, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_off();
}

/*
!Меняет время. На вход должно подаваться время в виде целого числа
hhmmss, где h - часы, m - минуты, s - секунды
*/
void set_time_callback( uint32_t arg )
{
	if(!pass_handler_is_any_pass_on())
	{
		return;
	}
	
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
	
	if(rtc_set_time(time) == 0)
	{
		info_field info;
		info.password = 1;
		event_handler_make_note(EVENT_NEW_TIME_DATE, info);
	}
}

/*
!Меняет время. На вход должно подаваться время в виде целого числа
ddmmyy, где d - день, m - месяц, y - год
*/
void set_date_callback( uint32_t arg )
{
	if(!pass_handler_is_any_pass_on())
	{
		return;
	}
	
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
	
	if(rtc_set_date(date) == 0)
	{
		info_field info;
		info.password = 1;
		event_handler_make_note(EVENT_NEW_TIME_DATE, info);
	}
}

/*
!Вызывает функцию калибровки
*/
void calibrate_callback( uint32_t arg )
{
	if(!pass_handler_is_any_pass_on())
	{
		return;
	}
	
	uint8_t output = cs5490_full_callibration(&chip_L1, &chip_L2, &chip_L3);
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, &output, 1, 100);
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_off();
}

/*
! Он/офф переход на сезонное время (летнее по умолчанию)
*/
void set_daylight_callback( uint32_t arg )
{
	if( arg != 1 && arg != 0)
	{
		return;
	}
	
	rtc_enable_daylight_saving(arg);
}

/*
! Изменение сезонного перехода на определенное время
1 - летнее
2 - зимнее
*/
void change_daylight_callback( uint32_t arg )
{
	if( arg != 1 && arg != 2)
	{
		return;
	}
	
	rtc_change_mode_daylight_saving(arg);	
}

/*
! Передача по интерфейсу данных по событиям
*/
void get_event_notes_callback( uint32_t arg )
{
	if(count_of_all_notes_in_mem == 0)
	{
		return;
	}
	
	char *r = ":";
	
	uint8_t count = 0;
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	
	uint32_t address = EVENT_INITIAL_ADDRESS;
	
	while(count != count_of_all_notes_in_mem)
	{
		uint32_t seconds = 0;
		m24m01_get_from_mem( address, (uint8_t*)&seconds, 4);
		HAL_UART_Transmit(&huart5, (uint8_t *)&seconds, 4, 100);
		address += 4;
		
		uint8_t info = 0;
		m24m01_get_from_mem( address, &info, 1);
		HAL_UART_Transmit(&huart5, &info, 1, 100);
		address += 1;
		
		uint32_t duration = 0;
		m24m01_get_from_mem( address, (uint8_t*)&duration, 4);
		HAL_UART_Transmit(&huart5, (uint8_t *)&duration, 4, 100);
		address += 4;
		
		uint8_t event_number = 0;
		m24m01_get_from_mem( address, &event_number, 1);
		HAL_UART_Transmit(&huart5, &event_number, 1, 100);
		
		HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
		count++;
		address += 1;
	}
	
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_off();
}

/*
! Передача по интерфейсу общих накопиелей
*/
void get_ter_data_callback( uint32_t arg )
{
	char *r = ":";
	uint32_t daily_address = MEM_INIT_ADDRESS_OF_DAY_RETROSPEC;
	uint32_t month_address = MEM_INIT_ADDRESS_OF_MONTH_RETROSPEC;
	uint32_t year_address = MEM_INIT_ADDRESS_OF_YEAR_RETROSPEC;
	uint32_t timestamp[2];
	double data[4];
	
	display_WIFI_on();
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	
	while(daily_address < current_address_of_day_retrosective)
	{
		m24m01_get_from_mem(daily_address, (uint8_t*)&(timestamp[0]), 4);
		daily_address += 4;
		m24m01_get_from_mem(daily_address, (uint8_t*)&(timestamp[1]), 4);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[0]), 4, 100);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[1]), 4, 100);
		daily_address += 4;
		for(uint8_t i = 0; i < 4; i++)
		{
			m24m01_get_from_mem(daily_address, (uint8_t*)&(data[i]), 8);
			HAL_UART_Transmit(&huart5, (uint8_t*)&(data[i]), 8, 100);
			daily_address += 8;
		}
		HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	}
	
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	while(month_address < current_address_of_month_retrosective)
	{
		m24m01_get_from_mem(month_address, (uint8_t*)&(timestamp[0]), 4);
		month_address += 4;
		m24m01_get_from_mem(month_address, (uint8_t*)&(timestamp[1]), 4);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[0]), 4, 100);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[1]), 4, 100);
		month_address += 4;
		for(uint8_t i = 0; i < 4; i++)
		{
			m24m01_get_from_mem(month_address, (uint8_t*)&(data[i]), 8);
			HAL_UART_Transmit(&huart5, (uint8_t*)&(data[i]), 8, 100);
			month_address += 8;
		}
		HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	}
	
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	while(year_address < current_address_of_year_retrosective)
	{
		m24m01_get_from_mem(year_address, (uint8_t*)&(timestamp[0]), 4);
		year_address += 4;
		m24m01_get_from_mem(year_address, (uint8_t*)&(timestamp[1]), 4);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[0]), 4, 100);
		HAL_UART_Transmit(&huart5, (uint8_t*)&(timestamp[1]), 4, 100);
		year_address += 4;
		for(uint8_t i = 0; i < 4; i++)
		{
			m24m01_get_from_mem(year_address, (uint8_t*)&(data[i]), 8);
			HAL_UART_Transmit(&huart5, (uint8_t*)&(data[i]), 8, 100);
			year_address += 8;
		}
		HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	}
	
	HAL_UART_Transmit(&huart5, (uint8_t *) r, 1, 100);
	
	HAL_UART_Transmit(&huart5, (uint8_t *) acknowledge, 2, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	display_WIFI_off();
}