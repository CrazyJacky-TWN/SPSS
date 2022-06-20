#ifndef I2C_MESSAGE
#define I2C_MESSAGE

#include <stdint.h>

/* i2c message return code */
#define I2C_MSG_OK		0x00U
#define I2C_MSG_ERR		0xFFU

#define SUPPORT_I2C_NUMBER	13U

enum i2c_message{
	DISPLAY_STATUS = 0x00U,
	DISPLAY_IDENTIFICATION,
	LCD_BACKLIGHT_PWM_VALUE,
	DISPLAY_SCANNING,
	DISPLAY_ENABLE,
	DISPLAY_SHUTDOWN,
	INTERRUPT_STATUS_MESSAGE = 0x30U,
	CORE_ASSEMBLY,
	DELIVERY_ASSEMBLY,
	SOFTWARE_FORD_PART_NUMBER,
	SERIAL_NUMBER,
	MAIN_CALIBRATION_FORD_PART_NUMBER,
	CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE = 0xA3U
};
typedef enum i2c_message en_i2c_msg;

enum i2c_message_length{
	DISPLAY_STATUS_LEN = 3U,
	DISPLAY_IDENTIFICATION_LEN = 2U,
	LCD_BACKLIGHT_PWM_VALUE_LEN = 2U,
	DISPLAY_SCANNING_LEN = 1U,
	DISPLAY_ENABLE_LEN = 1U,
	DISPLAY_SHUTDOWN_LEN = 1U,
	INTERRUPT_STATUS_MESSAGE_LEN = 1U,
	CORE_ASSEMBLY_LEN = 25U,
	DELIVERY_ASSEMBLY_LEN = 25U,
	SOFTWARE_FORD_PART_NUMBER_LEN = 25U,
	SERIAL_NUMBER_LEN = 25U,
	MAIN_CALIBRATION_FORD_PART_NUMBER_LEN = 25U,
	CLIENT_SPECIFIC_DIAGNOSTIC_MESSAGE_LEN = 50U
};
typedef enum i2c_message en_i2c_msg_len;


typedef enum{
	LCDERR 	= 0x00000100U,
	BLERR 	= 0x00000200U,
	TERR 	= 0x00000400U,
	DCERR 	= 0x00000800U,
	RST_RQ 	= 0x00001000U,
	LLOSS 	= 0x00002000U,
	TSCERR 	= 0x00004000U,
	TCERR 	= 0x00008000U,
	DISP_ST	= 0x00000001U,
	TSC_ST 	= 0x00000002U,
	INIT 	= 0x00000004U,
	BL_ST 	= 0x00000040U,
	DISPERR	= 0x00000080U
}te_display_status;




typedef struct msg_content{
	const en_i2c_msg  teId;
	const uint32_t	u32Length;
	uint8_t		*p8Content;
}ts_msg_content;


void Protocol_SupportInit(void);
void Protocol_check(void);
uint32_t Protocol_OverWrite(en_i2c_msg teSubaddr, uint8_t *pu8Data, uint32_t u32InputLen);
uint32_t Protocol_GetBit(en_i2c_msg teSubaddr, te_display_status teBit);
uint32_t Protocol_UpdateBit(en_i2c_msg teSubaddr, te_display_status teBit, uint8_t u8EnableDisable);
#endif