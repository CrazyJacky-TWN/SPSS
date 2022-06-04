#include <stdio.h>
#include "i2c_message.h"


static uint8_t gu8_ID_00[DISPLAY_STATUS_LEN];
static uint8_t gu8_ID_01[DISPLAY_IDENTIFICATION_LEN];
static uint8_t gu8_ID_02[LCD_BACKLIGHT_PWM_VALUE_LEN];
static uint8_t gu8_ID_03[DISPLAY_SCANNING_LEN];
static uint8_t gu8_ID_04[DISPLAY_ENABLE_LEN];
static uint8_t gu8_ID_05[DISPLAY_SHUTDOWN_LEN];
static uint8_t gu8_ID_30[INTERRUPT_STATUS_MESSAGE_LEN];
static uint8_t gu8_ID_31[CORE_ASSEMBLY_LEN];
static uint8_t gu8_ID_32[DELIVERY_ASSEMBLY_LEN];
static uint8_t gu8_ID_33[SOFTWARE_FORD_PART_NUMBER_LEN];
static uint8_t gu8_ID_34[SERIAL_NUMBER_LEN];
static uint8_t gu8_ID_35[MAIN_CALIBRATION_FORD_PART_NUMBER_LEN];
static uint8_t gu8_ID_A3[CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE_LEN];



/* i2c message declaration */
static ts_msg_content ID_00 = 
{
	.teId = DISPLAY_STATUS,
	.u32Length = (uint32_t)DISPLAY_STATUS_LEN,
	.p8Content = gu8_ID_00
};
static ts_msg_content ID_01 = 
{
	.teId = DISPLAY_IDENTIFICATION,
	.u32Length = (uint32_t)DISPLAY_IDENTIFICATION_LEN,
	.p8Content = gu8_ID_01
};
static ts_msg_content ID_02 = 
{
	.teId = LCD_BACKLIGHT_PWM_VALUE,
	.u32Length = (uint32_t)LCD_BACKLIGHT_PWM_VALUE_LEN,
	.p8Content = gu8_ID_02
};
static ts_msg_content ID_03 = 
{
	.teId = DISPLAY_SCANNING,
	.u32Length = (uint32_t)DISPLAY_SCANNING_LEN,
	.p8Content = gu8_ID_03
};
static ts_msg_content ID_04 = 
{
	.teId = DISPLAY_ENABLE,
	.u32Length = (uint32_t)DISPLAY_ENABLE_LEN,
	.p8Content = gu8_ID_04
};
static ts_msg_content ID_05 = 
{
	.teId = DISPLAY_SHUTDOWN,
	.u32Length = (uint32_t)DISPLAY_SHUTDOWN_LEN,
	.p8Content = gu8_ID_05
};
static ts_msg_content ID_30 = 
{
	.teId = INTERRUPT_STATUS_MESSAGE,
	.u32Length = (uint32_t)INTERRUPT_STATUS_MESSAGE_LEN,
	.p8Content = gu8_ID_30
};
static ts_msg_content ID_31 = 
{
	.teId = CORE_ASSEMBLY,
	.u32Length = (uint32_t)CORE_ASSEMBLY_LEN,
	.p8Content = gu8_ID_31
};
static ts_msg_content ID_32 = 
{
	.teId = DELIVERY_ASSEMBLY,
	.u32Length = (uint32_t)DELIVERY_ASSEMBLY_LEN,
	.p8Content = gu8_ID_32
};
static ts_msg_content ID_33 =
{
	.teId = SOFTWARE_FORD_PART_NUMBER,
	.u32Length = (uint32_t)SOFTWARE_FORD_PART_NUMBER_LEN,
	.p8Content = gu8_ID_33
};
static ts_msg_content ID_34 = 
{
	.teId = SERIAL_NUMBER,
	.u32Length = (uint32_t)SERIAL_NUMBER_LEN,
	.p8Content = gu8_ID_34
};
static ts_msg_content ID_35 =
{
	.teId = MAIN_CALIBRATION_FORD_PART_NUMBER,
	.u32Length = (uint32_t)MAIN_CALIBRATION_FORD_PART_NUMBER_LEN,
	.p8Content = gu8_ID_35
};
static ts_msg_content ID_A3 = 
{
	.teId = CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE,
	.u32Length = (uint32_t)CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE_LEN,
	.p8Content = gu8_ID_A3
};



void Protocol_SupportInit(void)
{
	uint32_t loop, init = 0U;
	uint32_t dummy = 0;
	ts_msg_content *tmpArray[] = { &ID_00, &ID_01, &ID_02, &ID_03, &ID_04, &ID_05, &ID_30, &ID_31, &ID_32, &ID_33, &ID_34, &ID_35, &ID_A3};

	for(init = 0; init < 13; init++)
	{
		loop = 0U;
		do{
			tmpArray[init]->p8Content[loop] = dummy++;
			loop++;
		}
		while(loop < tmpArray[init]->u32Length);
	}
	
}

void Protocol_check(void){
	uint32_t loop, init = 0U;
	ts_msg_content *RegArray[] = { &ID_00, &ID_01, &ID_02, &ID_03, &ID_04, &ID_05, &ID_30, &ID_31, &ID_32, &ID_33, &ID_34, &ID_35, &ID_A3};


	for(init = 0; init < 13; init++)
	{
		loop = 0U;
		printf("ID_%02x", RegArray[init]->teId);
		do{
			printf("[%x]",RegArray[init]->p8Content[loop]);
			loop++;
		}
		while(loop < RegArray[init]->u32Length);
		printf("\r\n");
	}
}
