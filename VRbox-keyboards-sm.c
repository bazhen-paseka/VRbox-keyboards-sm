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

/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/
	void VRbox_Send(	VRbox_Struct	 	*_vrbox			,
						PCF8574_Struct		*_pcf			) ;
/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/
void VRbox_Init(	VRbox_Struct 		*_vrbox ,
					UART_HandleTypeDef	*_debug_uart ,
					UART_HandleTypeDef	*_main_uart ,
					I2C_HandleTypeDef	*_i2c	) {

	_vrbox->debug_uart	=	*_debug_uart	;
	_vrbox->main_uart	=	*_main_uart	;
	_vrbox->i2c 	=	*_i2c 	;

	#define		DEBUG_CHARS_SIZE	100
	char Debug_Char[DEBUG_CHARS_SIZE] = { 0 }	;
	int soft_version_arr_int[3];
	soft_version_arr_int[0] = ((SOFT_VERSION) / 100) %10 ;
	soft_version_arr_int[1] = ((SOFT_VERSION) /  10) %10 ;
	soft_version_arr_int[2] = ((SOFT_VERSION)      ) %10 ;

	int16_t version_year_i16	= VERSION_YEAR	;
	int16_t version_month_i16 	= VERSION_MONTH	;
	int16_t version_day_i16		= VERSION_DAY	;

	sprintf(Debug_Char,"\r\n\r\n\tVR-box. Keyboards over PCF-8574" ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	sprintf(Debug_Char,"\r\n\tVersion: v%d.%d.%d %02d/%02d/%d" ,
			soft_version_arr_int[0] , soft_version_arr_int[1] , soft_version_arr_int[2] ,
			version_day_i16 , version_month_i16 , version_year_i16 ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	sprintf(Debug_Char,"\r\n\tfor debug: UART1 115200/8-N-1" ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;


	#define DATE_as_int_str 	(__DATE__)
	#define TIME_as_int_str 	(__TIME__)
	sprintf(Debug_Char,"\r\n\tBuild: %s, time: %s." , DATE_as_int_str , TIME_as_int_str ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	I2C_ScanBusFlow(&_vrbox->i2c , &_vrbox->debug_uart) ;
}
//***************************************************************************

void VRbox_Send(	VRbox_Struct 	*_vrbox 	,
					PCF8574_Struct	*_pcf		) {

	#define		DEBUG_CHARS_SIZE	100
	char 	Debug_Char[DEBUG_CHARS_SIZE]	= { 0 }	;
	char 	keyboard_char[4][4]	= { { '1', '2', '3', 'a' } ,
									{ '4', '5', '6', 'b' } ,
									{ '7', '8', '9', 'c' } ,
									{ 'e', '0', 'f', 'd' } } ;

	uint8_t		_button_u8 = _pcf->key_current ;
	uint8_t 	keyboard_Row_u8 = 0 ;
	uint8_t 	keyboard_Col_u8 = 0 ;
	keyboard_Col_u8 = _button_u8 % 10 ;
	keyboard_Row_u8 = _button_u8 / 10 ;
	snprintf(Debug_Char, 4, "%d%c ", (int)_pcf->channel, keyboard_char[keyboard_Row_u8][keyboard_Col_u8] ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	snprintf(Debug_Char, 6, "%d%02d\r\n", (int)_pcf->channel , _button_u8 ) ;
	HAL_UART_Transmit(&_vrbox->debug_uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;


	#define DEBUG_CHAR_SIZE		6
	char 	Main_Char[DEBUG_CHARS_SIZE]	= { 0 }	;
	snprintf(Main_Char, DEBUG_CHAR_SIZE, "%d%02d\r\n", (int)_pcf->channel , _button_u8 ) ;
	HAL_UART_Transmit( &_vrbox->main_uart , (uint8_t *)Main_Char , strlen(Main_Char) , 100 ) ;

	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)	;
}
//***************************************************************************

void VRbox_Check(	VRbox_Struct	*_vrbox	,
					PCF8574_Struct 	*_pcf	) {

	if ( PCF8574_get_IRQ_flag( _pcf ) == SET ) {
	_pcf->key_current = PCF8574_scan_keyboard( _pcf ) ;
	if ( _pcf->key_previous != _pcf->key_current ) {
		VRbox_Send( _vrbox , _pcf )					;
		_pcf->key_previous = _pcf->key_current	;
	}
	PCF8574_update_IRQ_flag( _pcf , RESET )	;
	PCF8574_IRQ_enable( _pcf )				;
	}
}

/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/

/*
**************************************************************************
*                              		  END
**************************************************************************
*/
