#include <stdio.h>
#include "config.h"
#include "i2c_message.h"

/* Physical i2c message storage */
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



/* Global i2c message declaration */
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

/* Union of support i2c message */
static ts_msg_content *gptsSupportMsg[] =
{
	&ID_00, &ID_01, &ID_02, &ID_03, &ID_04, &ID_05, &ID_30, &ID_31, &ID_32, 		&ID_33, &ID_34, &ID_35, &ID_A3
};

/* Function declartion */
static ts_msg_content* Get_ProtocolBuf(en_i2c_msg subaddr);


void Protocol_SupportInit(void)
{
	uint32_t loop, init = 0U;

	for(init = 0U; init < SUPPORT_I2C_NUMBER; init++)
	{
		loop = 0U;
		if( CORE_ASSEMBLY > gptsSupportMsg[init]->teId )
		{
			do{
				gptsSupportMsg[init]->p8Content[loop] = I2CMSG_00;
				loop++;
			}
			while(loop < gptsSupportMsg[init]->u32Length);
		}
		else if( CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE > gptsSupportMsg[init]->teId )
		{
			do{
				gptsSupportMsg[init]->p8Content[loop] = I2CMSG_FF;
				loop++;
			}
			while(loop < gptsSupportMsg[init]->u32Length);
		}
		else if( CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE == gptsSupportMsg[init]->teId )
		{
			do{
				gptsSupportMsg[init]->p8Content[loop] = I2CMSG_00;
				loop++;
			}
			while(loop < gptsSupportMsg[init]->u32Length);
		}
		else
		{
			;
		}
	}
	
}

void Protocol_check(void){
	uint32_t loop, init = 0U;


	for(init = 0; init < 13; init++)
	{
		loop = 0U;
		printf("ID_%02x:", gptsSupportMsg[init]->teId);
		do{
			printf("[%02xh]",gptsSupportMsg[init]->p8Content[loop]);
			loop++;
		}
		while(loop < gptsSupportMsg[init]->u32Length);
		printf("\r\n");
	}
}


uint32_t Protocol_OverWrite(en_i2c_msg teSubaddr, uint8_t *pu8Data, uint32_t u32InputLen)
{
	ts_msg_content *ptsMsg = NULL;
	uint32_t u32DataLen = 0U, u32Start = 0U, u32RtCode = TRUE;

	ptsMsg = Get_ProtocolBuf(teSubaddr);
	if( NULL != ptsMsg)
	{
		if( u32InputLen == ptsMsg->u32Length )
		{
			u32DataLen = ptsMsg->u32Length;
	
			/* data overwrite */
			while( 0U < u32DataLen )
			{
				ptsMsg->p8Content[u32Start] = *pu8Data;
				u32Start++;
				pu8Data++;
				u32DataLen--;		
			}
		}
		else
		{
			/* invalid input length */
			printf("ID:[%02x], with error data length:%02x\r\n",teSubaddr, u32InputLen);
			u32RtCode = FALSE;
		}
	}
	else
	{
		/* invalid i2c subaddress */
		printf("Error subaddress:%02x\r\n",teSubaddr);
		u32RtCode = FALSE;
	}

	return u32RtCode;
}


uint32_t Protocol_GetBit(en_i2c_msg teSubaddr, te_display_status teBit)
{
	ts_msg_content *ptsMsg = NULL;
	uint32_t u32RtCode = 0U;
	uint32_t u32TmpReg = 0U;
	uint32_t u32Loop = 0U, u32Cnt = 0U;

	if( INTERRUPT_STATUS_MESSAGE >= teSubaddr )
	{
		ptsMsg = Get_ProtocolBuf(teSubaddr);
		if( NULL != ptsMsg )
		{
			/* Valid subaddress */
			u32Cnt = 0U;
			for( u32Loop = 0U ; u32Loop < ptsMsg->u32Length ; u32Loop++ )
			{
				u32TmpReg <<= 8U;
				u32TmpReg = ptsMsg->p8Content[u32Cnt]; u32Cnt++;
			}

			if( u32TmpReg & (uint32_t)teBit )
			{
				u32RtCode = 1U;
			}
			else
			{
				u32RtCode = 0U;
			}
				
			printf("Memory ID:%02x [%08xh]\r\n",teSubaddr, u32TmpReg);
			printf("Bit:%s\r\n",u32RtCode == 1U? "TRUE":"FALSE");
		}
		else
		{
			/* Invalid subaddress */
			printf("Invlaid subaddress:%02x\r\n", teSubaddr);	
		}
	}
	else
	{
		/* After 0x31 didn't support get bit feature */
		printf("Didn't support get bit feature:%02x\r\n", teSubaddr);
	}

	return u32RtCode;
}


uint32_t Protocol_UpdateBit(en_i2c_msg teSubaddr, te_display_status teBit, uint8_t u8EnableDisable)
{
	
}


static ts_msg_content* Get_ProtocolBuf(en_i2c_msg subaddr)
{
	uint8_t loop;
	ts_msg_content *ptsGot = NULL;
	
	/* search */
	for(loop = 0U; loop < SUPPORT_I2C_NUMBER; loop++)
	{
		if(subaddr == gptsSupportMsg[loop]->teId)
		{
			/* found it */
			ptsGot = gptsSupportMsg[loop];
			break;
		}	
		else
		{
			;
		}
	}

	if(NULL == ptsGot)
	{
		printf("not found in support message array:%02x\r\n", subaddr);
	}
	else
	{
		;
	}

	return ptsGot;
}