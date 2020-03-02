/*
 * stepper.c
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#include "stepper.h"


uint16_t stepNumber = 0;	//keep track of current motor step: 0 - 2038 as motor is geared

void Stepper_Init()
{
	//Setting sleep pin LOW to disable stepper
	GPIO_PinWrite(STEPPER_SLEEP_GPIO, STEPPER_SLEEP_PIN, 0U);
	GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_1, 0U);
	GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_2, 0U);
}

void Stepper_Disable()
{
	GPIO_PinWrite(STEPPER_SLEEP_GPIO, STEPPER_SLEEP_PIN, 0U);
}

void Stepper_Enable()
{
	GPIO_PinWrite(STEPPER_SLEEP_GPIO, STEPPER_SLEEP_PIN, 1U);
}

void Stepper_Move(uint8_t stepDirection, uint16_t stepSpeed, uint32_t numberOfSteps)
{
	uint32_t stepDelay = 60*1000*1000/STEPPER_STEPS_PER_REVOLUTION/stepSpeed;

	Stepper_Enable();
	while(numberOfSteps > 0)
	{
		if(stepDirection == STEPPER_FORWARD)
		{
			stepNumber++;
			if(stepNumber == STEPPER_STEPS_PER_REVOLUTION)
			{
				stepNumber = 0;
			}
		}
		else
		{
			if(stepNumber == 0)
			{
				stepNumber = STEPPER_STEPS_PER_REVOLUTION;
			}
			stepNumber--;
		}
		numberOfSteps--;
		Stepper_TakeStep(stepNumber % 4);
		Timer_DelayUs(stepDelay);
	}
}

void Stepper_TakeStep(uint8_t stepPhase)
{
	switch(stepPhase)
	{
		case 0:		//01
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_1, 0U);
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_2, 1U);
			break;

		case 1:		//11
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_1, 1U);
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_2, 1U);
			break;

		case 2:		//10
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_1, 1U);
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_2, 0U);
			break;

		case 3:		//00
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_1, 0U);
			GPIO_PinWrite(STEPPER_CONTROL_GPIO, STEPPER_CONTROL_PIN_2, 0U);
			break;
	}
}
