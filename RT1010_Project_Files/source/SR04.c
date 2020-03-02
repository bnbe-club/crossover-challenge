#include "SR04.h"

float echoDuration = 0; 	//range is 20-4000mm

void SR04_Init()
{
	//nothing needs to be done here.
}

void SR04_Trigger()
{
	echoDuration = 0;

	//send trigger
	GPIO_PinWrite(SR04_TRIGGER_PORT, SR04_TRIGGER_PIN, 1U);
	Timer_DelayUs(10);
	GPIO_PinWrite(SR04_TRIGGER_PORT, SR04_TRIGGER_PIN, 0U);
}

uint16_t SR04_GetDistance()
{
	SR04_Trigger();

	while(!GPIO_PinRead(SR04_ECHO_PORT, SR04_ECHO_PIN));
	Timer_StartCapture();

	while(GPIO_PinRead(SR04_ECHO_PORT, SR04_ECHO_PIN));
	echoDuration = (uint16_t) Timer_StopCapture();

	echoDuration = echoDuration*17150*10/1000000.0;

	return (uint16_t) echoDuration;

}
