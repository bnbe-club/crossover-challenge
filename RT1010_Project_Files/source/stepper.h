/*
 * stepper.h
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "fsl_gpio.h"
#include "timer.h"

#define STEPPER_CONTROL_GPIO				GPIO1
#define STEPPER_CONTROL_PIN_1				(15U)
#define STEPPER_CONTROL_PIN_2				(16U)

#define STEPPER_SLEEP_GPIO					GPIO2
#define STEPPER_SLEEP_PIN 					(02U)

#define STEPPER_SPEED_DEFAULT				15
#define STEPPER_STEPS_PER_REVOLUTION		2038
#define STEPPER_FORWARD						1
#define STEPPER_REVERSE						0

void Stepper_Init();
void Stepper_Enable();
void Stepper_Disable();
void Stepper_Move(uint8_t stepDirection, uint16_t stepSpeed, uint32_t numberOfSteps);
void Stepper_TakeStep(uint8_t stepPhase);


#endif /* STEPPER_H_ */
