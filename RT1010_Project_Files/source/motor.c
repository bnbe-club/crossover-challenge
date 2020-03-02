/*
 * MOT.c
 *
 *  Created on: 5 Feb 2020
 *      Author: frenoy
 */

#include <motor.h>

uint8_t motorState = 0;		//curent state of motor

void Motor_Init()
{
	  /* Structure of PWM config */
	  pwm_config_t pwmConfig;

	  uint16_t deadTimeVal;
	  pwm_signal_param_t pwmSignal[2];
	  uint32_t pwmSourceClockInHz;
	  uint32_t pwmFrequencyInHz = 1000;

	  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
	  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divide by 4 */

	  /* Set the PWM Fault inputs to a low value */
	  XBARA_Init(XBARA);
	  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
	  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
	  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault2);
	  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault3);

	  /*
	   * pwmConfig.enableDebugMode = false;
	   * pwmConfig.enableWait = false;
	   * pwmConfig.reloadSelect = kPWM_LocalReload;
	   * pwmConfig.faultFilterCount = 0;
	   * pwmConfig.faultFilterPeriod = 0;
	   * pwmConfig.clockSource = kPWM_BusClock;
	   * pwmConfig.prescale = kPWM_Prescale_Divide_1;
	   * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
	   * pwmConfig.forceTrigger = kPWM_Force_Local;
	   * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
	   * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
	   * pwmConfig.pairOperation = kPWM_Independent;
	   */
	  PWM_GetDefaultConfig(&pwmConfig);

	  /* Use full cycle reload */
	  pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
	  pwmConfig.enableDebugMode = true;

	  /* Initialise submodule 2 */
	  PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_2, &pwmConfig);

	  /* Initialise submodule 3 */
	  PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_3, &pwmConfig);

	  pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

	  /* Set dead time count, we set this to about 650ns */
	  deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

	  pwmSignal[0].pwmChannel       = kPWM_PwmA;
	  pwmSignal[0].level            = kPWM_HighTrue;
	  pwmSignal[0].dutyCyclePercent = 0;
	  pwmSignal[0].deadtimeValue    = deadTimeVal;

	  pwmSignal[1].pwmChannel 		= kPWM_PwmB;
	  pwmSignal[1].level      		= kPWM_HighTrue;
	  pwmSignal[1].dutyCyclePercent = 0;
	  pwmSignal[1].deadtimeValue    = deadTimeVal;

	  // set PWM A2, B2
	  PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz, pwmSourceClockInHz);

	  // set PWM A3, B3
	  PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_3, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz, pwmSourceClockInHz);

	  /* Set the load okay bit for all submodules to load registers from their buffer */
	  PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_2 | kPWM_Control_Module_3, true);

	  /* Start the PWM generation from Submodules 2 and 3 */
	  PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_2 | kPWM_Control_Module_3);
}

void Motor_Update(uint8_t channel , uint8_t dutyCyclePercent)
{
	if((dutyCyclePercent >= 0) && (dutyCyclePercent <= 100))
	{
		switch(channel)
		{
			case RIGHT_POSITIVE:
				PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, dutyCyclePercent);
				PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_2, true);
				break;

			case RIGHT_NEGATIVE:
				PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_3, kPWM_PwmB, kPWM_SignedCenterAligned, dutyCyclePercent);
				PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_3, true);
				break;

			case LEFT_POSITIVE:
				PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_3, kPWM_PwmA, kPWM_SignedCenterAligned, dutyCyclePercent);
				PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_3, true);
				break;

			case LEFT_NEGATIVE:
				PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB, kPWM_SignedCenterAligned, dutyCyclePercent);
				PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_2, true);
				break;
		}
	}
}

void Motor_UpdateState()
{
	uint8_t inputState = 4*GPIO_PinRead(MOTOR_INPUT_PORT, MOTOR_INPUT_PIN_D3) + 2*GPIO_PinRead(MOTOR_INPUT_PORT, MOTOR_INPUT_PIN_D2) + GPIO_PinRead(MOTOR_INPUT_PORT, MOTOR_INPUT_PIN_D1);

	//inputState = MOTOR_STATE_REVERSE;
	//if(motorState != inputState)
	{
		if(inputState == MOTOR_STATE_STOP)
		{
			//DEBUG_Write_String("stop\r", 5);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_LOW);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_LOW);
			motorState = MOTOR_STATE_STOP;

		}
		else if(inputState == MOTOR_STATE_RIGHT)
		{
			//DEBUG_Write_String("right\r", 6);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_LOW);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_LOW);

			motorState = MOTOR_STATE_RIGHT;
		}
		else if(inputState == MOTOR_STATE_LEFT)
		{
			//DEBUG_Write_String("left\r", 5);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_LOW);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_LOW);

			motorState = MOTOR_STATE_LEFT;
		}
		else if(inputState == MOTOR_STATE_FORWARD)
		{
			//DEBUG_Write_String("forward\r", 8);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_LOW);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_LOW);

			motorState = MOTOR_STATE_FORWARD;
		}
		else if(inputState == MOTOR_STATE_REVERSE)
		{
			//DEBUG_Write_String("reverse\r", 8);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_HIGH);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_HIGH);

			motorState = MOTOR_STATE_REVERSE;
		}
		else if(inputState == MOTOR_STATE_HARD_RIGHT)
		{
			//DEBUG_Write_String("hard right\r", 11);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_HIGH);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_LOW);

			motorState = MOTOR_STATE_HARD_RIGHT;
		}
		else if(inputState == MOTOR_STATE_HARD_LEFT)
		{
			//DEBUG_Write_String("hard left\r", 10);
			Motor_Update(RIGHT_POSITIVE, DUTY_CYCLE_HIGH);
			Motor_Update(RIGHT_NEGATIVE, DUTY_CYCLE_LOW);

			Motor_Update(LEFT_POSITIVE, DUTY_CYCLE_LOW);
			Motor_Update(LEFT_NEGATIVE, DUTY_CYCLE_HIGH);

			motorState = MOTOR_STATE_HARD_LEFT;
		}
	}
}
