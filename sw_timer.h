#ifndef _SW_TIMER_H_
#define _SW_TIMER_H_

#include <stdint.h>

/* configuration */
#define SWTIMER_BLOCK		10U
#define SWTIMER_INVALID		SWTIMER_BLOCK + 1U

#define SWTIMER_IDLE		0x01U
#define SWTIMER_PROGRESS	0x02U
#define SWTIMER_TIMEOUT		0x04U



typedef struct{
	uint32_t u32Id;
	uint32_t u32Period;
	int32_t (* pCallback)(uint32_t *); 
	int32_t (* pINTCallback)(void);
}tdSWTimerConfig;


typedef struct{
	/* public variables */	
	uint32_t u32Status;
	uint32_t u32Period;
	int32_t (* pCallback)(uint32_t *); 
	int32_t (* pINTCallback)(void);
}tdTimerInterface;

uint32_t SWTimer_Init(void);
uint32_t SWTimer_Counting(uint32_t u32Dummy);
uint32_t SWTimer_Setting(tdSWTimerConfig * tdTimerConfig);
uint32_t SWTimer_INTStop(uint32_t u32Id);
uint32_t SWTimer_Status(uint32_t u32Id);
#endif