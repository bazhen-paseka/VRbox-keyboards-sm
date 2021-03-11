/**
* \version 1.0
* \author Bazhen Levkovets
* \date 2021/MAR/10
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary
* \copyright	Ukraine
*************************************************************************************
*/

#ifndef VR_B0X_KEYBOARDS_SM_H_INCLUDED
#define VR_B0X_KEYBOARDS_SM_H_INCLUDED

/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/
	#include <string.h>
	#include <stdio.h>

	#include "main.h"
	#include "i2c.h"
	#include "usart.h"

	#include "pcf8574_local_config.h"
	#include "3Dbox-keyboards-local-config.h"
	#include "keyb_4x4_pcf8574_i2c_sm.h"


/*
**************************************************************************
*								    DEFINES                     
**************************************************************************
*/

/*
**************************************************************************
*								   DATA TYPES
**************************************************************************
*/

/*
**************************************************************************
*								   	ENUM
**************************************************************************
*/
	typedef enum
	{
		VR_KeyBoard_0	,
		VR_KeyBoard_1	,
		VR_KeyBoard_QNT	,
	} VR_Keyboard_enum ;
/*
**************************************************************************
*								GLOBAL VARIABLES
**************************************************************************
*/
	typedef struct {
		I2C_HandleTypeDef  	i2c		;
		UART_HandleTypeDef 	uart	;
	} 		Debug_Struct			;
/*
**************************************************************************
*									 MACRO'S                     
**************************************************************************
*/

/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/
	void VRbox_Debug_Init(	Debug_Struct	 	*_debug	,
							UART_HandleTypeDef	*_uart ,
							I2C_HandleTypeDef	*_i2c	) ;

	void VRbox_Main(		Debug_Struct 		*_debug ,
							PCF8574_Struct  	*_pcf0	,
							PCF8574_Struct  	*_pcf1	) ;

	void Debug_print_key(	Debug_Struct 		*_debug 	,
							PCF8574_Struct		*_pcf		,
							uint8_t 			_button_u8	) ;
/*
**************************************************************************
*                              		  END
**************************************************************************
*/
#endif /* VR_B0X_KEYBOARDS_SM_H_INCLUDED */
