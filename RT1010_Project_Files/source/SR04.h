#ifndef _SR04_H_
#define _SR04_H_

#include "board.h"
#include "timer.h"

#define SR04_TRIGGER_PORT	GPIO1
#define SR04_TRIGGER_PIN	(01U)		//J57_18

#define SR04_ECHO_PORT		GPIO1
#define SR04_ECHO_PIN		(21U)		//J26_2


extern float echoDuration; 	//range is 20-4000mm

void SR04_Init();
void SR04_Trigger();
uint16_t SR04_GetDistance();

#endif
