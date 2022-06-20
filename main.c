#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include "i2c_message.h"
#include "sw_timer.h"






static uint32_t cnt = 0U;
static tdSWTimerConfig gtdEvent1;
static tdSWTimerConfig gtdEvent2;
static tdSWTimerConfig gtdEvent3;


int32_t Time100_Callback(uint32_t *dummy);
int32_t Time500_Callback(uint32_t *dummy);
int32_t Time1000_Callback(uint32_t *dummy);

int32_t Time1000_INTCallback(void);


int main(void) {
	uint8_t buff[3] = {0x00,0x00,0x07};
	Protocol_SupportInit();
	Protocol_OverWrite(DISPLAY_STATUS, buff, 3U);
	//Protocol_GetBit(CORE_ASSEMBLY, DISP_ST);
	Protocol_check();
	
	SWTimer_Init();


	
	gtdEvent1.pCallback = Time100_Callback;
	gtdEvent1.pINTCallback = Time1000_INTCallback;
	gtdEvent1.pINTCallback = NULL;
	gtdEvent1.u32Period = 60000U;
	gtdEvent1.u32Id = SWTimer_Setting(&gtdEvent1);

	if( SWTIMER_INVALID <= gtdEvent1.u32Id)
		printf("timer config invalid\r\n");
	else
		printf("timer : %d progressing\r\n",gtdEvent1.u32Id);

	while(1)
	{
		if( 10000U >= cnt )
		{
			if( ! (cnt % 500) )
				SWTimer_INTStop(gtdEvent1.u32Id);
			
			cnt++;
		}
		else
		{
			SWTimer_Counting((uint32_t *)NULL);
			cnt = 0U;
		}
	};
	
  	return 0;
}


int32_t Time100_Callback(uint32_t *dummy)
{
	printf("SW Timer100 timeout\r\n");
/*
	gtdEvent2.pCallback = Time500_Callback;
	gtdEvent2.u32Period = 500U;
	gtdEvent2.u32Id = SWTimer_Setting(&gtdEvent2);
	printf("timer : %d progressing\r\n",gtdEvent2.u32Id);
*/
	return TRUE;
}

int32_t Time500_Callback(uint32_t *dummy)
{
	printf("SW Timer500 timeout\r\n");
/*
	gtdEvent3.pCallback = Time1000_Callback;
	gtdEvent3.u32Period = 1000U;
	gtdEvent3.u32Id = SWTimer_Setting(&gtdEvent3);
	printf("timer : %d progressing\r\n",gtdEvent3.u32Id);
*/
	return TRUE;
}
int32_t Time1000_Callback(uint32_t *dummy)
{
	printf("SW Timer1000 timeout\r\n");

/*
	gtdEvent1.pCallback = Time100_Callback;
	gtdEvent1.u32Period = 100U;
	gtdEvent1.u32Id = SWTimer_Setting(&gtdEvent1);
	printf("timer : %d progressing\r\n",gtdEvent1.u32Id);
*/
	return TRUE;
}


int32_t Time1000_INTCallback(void)
{
	printf("SW Timer1000 INT callback\r\n");
/*
	gtdEvent1.pCallback = Time100_Callback;
	gtdEvent1.u32Period = 100U;
	gtdEvent1.u32Id = SWTimer_Setting(&gtdEvent1);
	printf("timer : %d progressing\r\n",gtdEvent1.u32Id);
*/
	return TRUE;
}