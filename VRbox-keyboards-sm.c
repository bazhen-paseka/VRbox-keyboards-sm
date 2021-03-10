/**
* \version 1.0
* \author bazhen.levkovets
* \date 2019 
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary
* \copyright	Ukraine
*************************************************************************************
*/

/*
**************************************************************************
*							INCLUDE FILES
**************************************************************************
*/
	#include "VRbox-keyboards-sm.h"
/*
**************************************************************************
*							LOCAL DEFINES
**************************************************************************
*/
	#define		DEBUG_CHARS_SIZE	100
/*
**************************************************************************
*							LOCAL CONSTANTS
**************************************************************************
*/


/*
**************************************************************************
*						    LOCAL DATA TYPES
**************************************************************************
*/


/*
**************************************************************************
*							  LOCAL TABLES
**************************************************************************
*/

/*
**************************************************************************
*								 MACRO'S
**************************************************************************
*/


/*
**************************************************************************
*						 LOCAL GLOBAL VARIABLES
**************************************************************************
*/
//		  PCF8574_Struct  hpcf0;
//		  PCF8574_Struct  hpcf1;
/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/
void VRbox_Debug_Init(	Debug_Struct 		*_debug ,
						UART_HandleTypeDef	*_uart ,
						I2C_HandleTypeDef	*_i2c	) {

	_debug->uart	=	*_uart	;
	_debug->i2c 	=	*_i2c 	;

	char Debug_Char[DEBUG_CHARS_SIZE] = { 0 }	;
	int soft_version_arr_int[3];
	soft_version_arr_int[0] = ((SOFT_VERSION) / 100) %10 ;
	soft_version_arr_int[1] = ((SOFT_VERSION) /  10) %10 ;
	soft_version_arr_int[2] = ((SOFT_VERSION)      ) %10 ;

	int16_t version_year_i16	= VERSION_YEAR	;
	int16_t version_month_i16 	= VERSION_MONTH	;
	int16_t version_day_i16		= VERSION_DAY	;

	sprintf(Debug_Char,"\r\n\r\n\tVR-box. Keyboards over PCF-8574" ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	sprintf(Debug_Char,"\r\n\tVersion: v%d.%d.%d %02d/%02d/%d" ,
			soft_version_arr_int[0] , soft_version_arr_int[1] , soft_version_arr_int[2] ,
			version_day_i16 , version_month_i16 , version_year_i16 ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	sprintf(Debug_Char,"\r\n\tfor debug: UART1 115200/8-N-1" ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;


	#define DATE_as_int_str 	(__DATE__)
	#define TIME_as_int_str 	(__TIME__)
	sprintf(Debug_Char,"\r\n\tBuild: %s, time: %s." , DATE_as_int_str , TIME_as_int_str ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	I2C_ScanBusFlow(&_debug->i2c, &_debug->uart) ;
}
//***************************************************************************

void VRbox_Main(	Debug_Struct 		*_debug ,
					PCF8574_Struct  	*_pcf0	,
					PCF8574_Struct  	*_pcf1	)  {

	static 	uint8_t previous_key[VR_KeyBoard_QNT] = { 5, 5 }	;
//---------------------------------------------------------------
	if ( PCF8574_get_IRQ_flag( _pcf0 ) == SET ) 			{
		uint8_t key0 = PCF8574_scan_keyboard( _pcf0 ) 	;
		if ( previous_key[ _pcf0->channel ] != key0 )		{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) 		;
			PCF8574_print_key( _pcf0 , key0 )			;
			Debug_print_key(  _debug , _pcf0,  key0 )	;
			previous_key[_pcf0->channel] = key0 		;
		}
		PCF8574_update_IRQ_flag( _pcf0 , RESET );
		PCF8574_IRQ_enable( _pcf0 )				;
	}
//---------------------------------------------------------------
	if ( PCF8574_get_IRQ_flag( _pcf1 ) == SET ) 			{
		uint8_t key1 = PCF8574_scan_keyboard( _pcf1 )	;
		if ( previous_key[ _pcf0->channel ] != key1 )		{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) 		;
			PCF8574_print_key( _pcf1 , key1 )			;
			Debug_print_key(  _debug , _pcf1 , key1 )	;
			previous_key[ _pcf0->channel ] = key1		;
		}
		PCF8574_update_IRQ_flag( _pcf1 , SET )	;
		PCF8574_IRQ_enable( _pcf1 )				;
	}
//---------------------------------------------------------------
}
/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/

void Debug_print_key(	Debug_Struct 	*_debug 	,
						PCF8574_Struct	*_pcf		,
						uint8_t 		_button_u8	) {

	char 	Debug_Char[DEBUG_CHARS_SIZE]	= { 0 }	;
	char 	keyboard_char[4][4]	= { { '1', '2', '3', 'a' } ,
									{ '4', '5', '6', 'b' } ,
									{ '7', '8', '9', 'c' } ,
									{ 'e', '0', 'f', 'd' } } ;
	uint8_t 	keyboard_Row_u8 = 0 ;
	uint8_t 	keyboard_Col_u8 = 0 ;
	keyboard_Col_u8 = _button_u8 % 10 ;
	keyboard_Row_u8 = _button_u8 / 10 ;
	snprintf(Debug_Char, 4, "%d%c ", (int)_pcf->channel, keyboard_char[keyboard_Row_u8][keyboard_Col_u8] ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	snprintf(Debug_Char, 6, "%d%02d\r\n", (int)_pcf->channel , _button_u8 ) ;
	HAL_UART_Transmit(&_debug->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;
}

/*
**************************************************************************
*                              		  END
**************************************************************************
*/
