/*
 * MOT.h
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "board.h"
#include "debug.h"

#define RIGHT_POSITIVE		1
#define RIGHT_NEGATIVE		2
#define LEFT_POSITIVE		3
#define LEFT_NEGATIVE		4

#define DUTY_CYCLE_HIGH		60
#define DUTY_CYCLE_MID		30
#define DUTY_CYCLE_LOW		0

#define MOTOR_INPUT_PORT	GPIO1
#define MOTOR_INPUT_PIN_D1	(23U)		//J26_4
#define MOTOR_INPUT_PIN_D2	(24U)		//J26_6
#define MOTOR_INPUT_PIN_D3	(28U)		//J26_8

#define MOTOR_STATE_STOP		0		//000 - D3D2D1
#define MOTOR_STATE_RIGHT		1		//001 - D3D2D1
#define MOTOR_STATE_LEFT		2		//010 - D3D2D1
#define MOTOR_STATE_FORWARD		3		//011 - D3D2D1
#define MOTOR_STATE_REVERSE		4		//100 - D3D2D1
#define MOTOR_STATE_HARD_RIGHT	5		//101 - D3D2D1
#define MOTOR_STATE_HARD_LEFT	6		//110 - D3D2D1

/* The PWM base address */
#define BOARD_PWM_BASEADDR PWM1
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

void Motor_Init();
void Motor_Update(uint8_t channel , uint8_t dutyCyclePercent);

void Motor_UpdateState();

#endif /* MOTOR_H_ */
