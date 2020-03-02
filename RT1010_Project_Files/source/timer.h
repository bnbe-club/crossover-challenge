/*
 * timer.h
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "fsl_gpt.h"

#define TIMER_GPT GPT2

void Timer_Init();
void Timer_DelayMs(uint16_t delay_time);
void Timer_DelayUs(uint16_t delay_time);
void Timer_StartCapture();
uint32_t Timer_StopCapture();

#endif /* TIMER_H_ */
