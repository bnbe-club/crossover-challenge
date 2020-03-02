#ifndef _MILLIS_H_
#define _MILLIS_H_

#include "board.h"

void Millis_Init();
uint32_t Millis_Now();
void SysTick_Handler(void);

#endif
