#ifndef _DEBUG_H
#define _DEBUG_H

#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"

#define DEBUG_LPUART 				LPUART1
#define DEBUG_LPUART_CLK_FREQ 		BOARD_DebugConsoleSrcFreq()
#define DEBUG_LPUART_IRQn 			LPUART1_IRQn
#define DEBUG_LPUART_IRQHandler 	LPUART1_IRQHandler

#define DEBUG_TX_BUFFER_SIZE		128
#define DEBUG_RX_BUFFER_SIZE		128

extern uint8_t debugTxBuffer[DEBUG_TX_BUFFER_SIZE];
extern uint8_t debugRxBuffer[DEBUG_RX_BUFFER_SIZE];

extern volatile uint8_t debugTxReadIndex;
extern volatile uint8_t debugTxWriteIndex;
extern volatile uint8_t debugTxCount;

extern volatile uint8_t debugRxReadIndex;
extern volatile uint8_t debugRxWriteIndex;
extern volatile uint8_t debugRxCount;

void DEBUG_LPUART_IRQHandler(void);
void DEBUG_Init();
void DEBUG_Refresh();
uint8_t DEBUG_Available();
uint8_t DEBUG__Read();
void DEBUG_Write(uint8_t data);
void DEBUG_Write_String(char data[], uint8_t length);


#endif
