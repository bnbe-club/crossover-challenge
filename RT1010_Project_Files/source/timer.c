/*
 * timer.c
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#ifndef TIMER_C_
#define TIMER_C_

#include "timer.h"


void Timer_Init()
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.enableFreeRun = false;
    gptConfig.clockSource = kGPT_ClockSource_Periph;
    GPT_Init(TIMER_GPT, &gptConfig);

    GPT_SetClockDivider(TIMER_GPT, 25);
}

void Timer_DelayMs(uint16_t delay_time)
{
	GPT_SetOutputCompareValue(TIMER_GPT, kGPT_OutputCompare_Channel1, (2.5/3.0)*1000*delay_time);		//as PWM divides peri clock by 3
	GPT_ClearStatusFlags(TIMER_GPT, kGPT_OutputCompare1Flag);
    GPT_StartTimer(TIMER_GPT);

	while(!GPT_GetStatusFlags(TIMER_GPT, kGPT_OutputCompare1Flag));

	GPT_StopTimer(TIMER_GPT);
}

void Timer_DelayUs(uint16_t delay_time)
{
	GPT_SetOutputCompareValue(TIMER_GPT, kGPT_OutputCompare_Channel1, (2.5/3.0)*delay_time);
	GPT_ClearStatusFlags(TIMER_GPT, kGPT_OutputCompare1Flag);
	GPT_StartTimer(TIMER_GPT);

	while(!GPT_GetStatusFlags(TIMER_GPT, kGPT_OutputCompare1Flag));

	GPT_StopTimer(TIMER_GPT);
}

void Timer_StartCapture()
{
	GPT_StartTimer(TIMER_GPT);
	GPT_SetOutputCompareValue(TIMER_GPT, kGPT_OutputCompare_Channel1, 0xFFFFFFFF);
	GPT_ClearStatusFlags(TIMER_GPT, kGPT_OutputCompare1Flag);
}

//return count in uS
uint32_t Timer_StopCapture()
{
	GPT_StopTimer(TIMER_GPT);
	return GPT_GetCurrentTimerCount(TIMER_GPT)*3.0/2.5;
}
#endif /* TIMER_C_ */
