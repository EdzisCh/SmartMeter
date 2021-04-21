#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include "stm32l4xx_hal.h"
#include "M24M01.h"
#include "RTC.h"
#include "RS485.h"

#define EVENT_COUNT 42

//по 20 записей
#define EVENT_NEW_TIME_DATE 0
#define EVENT_TIME_SINCHRONIZATION 1
#define EVENT_SHEDULES_BACKGROUND_GROUP_CHANGING 2
#define EVENT_METER_POWER_SYPPLY_ON_OFF 3
//по 12 записей
#define EVENT_DAILY_SINCHRONIZATION_LIMIT_EXCEEDING 4
#define EVENT_CRITICAL_TIME_DISCREPANCY 5
#define EVENT_MONTH_YEAR_SINCHRONIZATION_LIMIT_EXCEEDING 6
#define EVENT_WROG_PHASE_SEQUECE 7
#define EVENT_TARIFFICATION_METHOD_CHANGING 8
#define EVENT_SHEDULES_WORKING_GROUP_CHANGING 9
#define EVENT_POWER_CONTROL_ON_OFF 10
#define EVENT_CONSUMPTION_CONTROL_MODIFICATION 11
#define EVENT_NETWORK_CONTROL_LEVELS_CHANGING 12
#define EVENT_LOAD_RELAY_CHANGING_OF_STATE 13
#define EVENT_PORTS_CHANGING_SETTINGS 14
#define EVENT_PHASE_A_VOLTAGE_DIP 15
#define EVENT_PHASE_B_VOLTAGE_DIP 16
#define EVENT_PHASE_C_VOLTAGE_DIP 17
#define EVENT_PHASE_A_OVERVOLTAGE 18
#define EVENT_PHASE_B_OVERVOLTAGE 19
#define EVENT_PHASE_C_OVERVOLTAGE 20
#define EVENT_PHASE_A_CURRENT_OVERSHOT 21
#define EVENT_PHASE_B_CURRENT_OVERSHOT 22
#define EVENT_PHASE_C_CURRENT_OVERSHOT 23
#define EVENT_CURRENT_THRESHOLD 24
#define EVENT_FREQUENCY_THRESHOLD 25
#define EVENT_POWER_LIMIT_EXCEEDING 26
#define EVENT_ENERGY_LIMIT_EXCEEDING 27
#define EVENT_PASSWORD_INCORRECT_LOGIN 28
#define EVENT_MAGNIT_IMPACT 29
#define EVENT_RADIO_IMPACT 30
#define EVENT_SEAL_CAP 31
//по 4 записи
#define EVENT_CLOCK_OFFSET_CHANGING 32
#define EVENT_CHANGING_MODE_OR_DATES_OF_DAYLIGHT_SAVING 33
#define EVENT_RESETTING_TARIFF_ACCUMULATORS 34
#define EVENT_PROFILE_CONFIGURATION_CHANGE 35
#define EVENT_METROLOGY_CHANGE 36
#define EVENT_LOAD_RELAY_CHANGING_OF_SETTINGS 37
#define EVENT_PASSWORD_INCORRECT_BLOCK 38
#define EVENT_SEAL_BODY 39
#define EVENT_METER_OVERHEAT 40
#define EVENT_DAYLIGHT_SAVING_TIME 41

//поле инфо
#define EVENT_INFO_PORT_RS485 0
#define EVENT_INFO_PORT_OPTO 1
#define EVENT_INFO_PORT_PLK 2
#define EVENT_INFO_PORT_RADIO 3

#define EVENT_INFO_PASS_P 0
#define EVENT_INFO_PASS_N 1

#define EVENT_INFO_SIGN_DAYLIGHT_WINTER 1
#define EVENT_INFO_SIGN_DAYLIGHT_SUMMER 2
#define EVENT_INFO_SIGN_DAYLIGHT_ON 1
#define EVENT_INFO_SIGN_DAYLIGHT_OFF 2

#define EVENT_INFO_SIGN_TARRIFICATION_METHOD_SHEDULE 1
#define EVENT_INFO_SIGN_TARRIFICATION_METHOD_EVENT 2
#define EVENT_INFO_SIGN_TARRIFICATION_METHOD_EXTERNAL 3

#define EVENT_INFO_SIGN_POWER_CONTROL_ON 1
#define EVENT_INFO_SIGN_POWER_CONTROL_OFF 2

#define EVENT_INFO_SIGN_CONSUMPTION_CONTROL_ENABLE 1
#define EVENT_INFO_SIGN_CONSUMPTION_CONTROL_CHANGE 2

#define EVENT_INFO_SIGN_NETWORK_CONTROL_V_UP 1
#define EVENT_INFO_SIGN_NETWORK_CONTROL_V_DOWN 2
#define EVENT_INFO_SIGN_NETWORK_CONTROL_F 3
#define EVENT_INFO_SIGN_NETWORK_CONTROL_I_MIN 4

#define EVENT_INFO_SIGN_LOAD_RELAY_SETTINGS 1
#define EVENT_INFO_SIGN_LOAD_RELAY_CONDITIONS 1

#define EVENT_INFO_SIGN_POWER_SUPPLY_ON 1
#define EVENT_INFO_SIGN_POWER_SUPPLY_OFF 2

#define EVENT_INFO_SIGN_SEAL_ON 1
#define EVENT_INFO_SIGN_SEAL_OFF 2
#define EVENT_INFO_SIGN_SEAL_CRACK 3

#define EVENT_NOTE_SIZE 10

#define EVENT_MAX_NOTES_4 4
#define EVENT_MAX_NOTES_12 12
#define EVENT_MAX_NOTES_20 20

#define EVENT_INITIAL_ADDRESS 28618
#define EVENT_CURRENT_ADDRESS_MEMORY_ADRESS 40024

extern uint32_t count_of_all_notes_in_mem;

typedef struct
{
	uint8_t sign;
	uint8_t password;
	uint8_t port;
	uint8_t seconds;
	uint8_t percent;
} info_field;

/**
! Журнал событий(записей)
*/
typedef struct
{
	uint8_t count_of_notes;
	uint8_t max_count_of_notes;
//	uint16_t memory_address; //initial address in mem
	uint8_t note_size; // 10
	//флаги, показывающие необходимость данной инфо-ии в инфополе
	uint8_t sign_flag;
	uint8_t password_flag;
	uint8_t port_flag;
	uint8_t seconds_flag;
	uint8_t percent_flag;
} event_log;
	
void event_handler_init_logs( void );
void event_handler_make_note( uint8_t event, info_field s_info );
uint8_t event_handler_make_info_field( uint8_t event, info_field info );

//===================================================================================

uint32_t get_seconds_from_2000( void );
uint8_t from_BCD_format( uint32_t val );
void event_handler_beep_on( void );
void event_handler_beep_off( void );

//===================================================================================


#endif
