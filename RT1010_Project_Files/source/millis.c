#include "millis.h"

volatile uint32_t millis;

void SysTick_Handler(void)
{
	millis++;
}

void Millis_Init()
{
    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
}

uint32_t Millis_Now()
{
	return millis;
}
