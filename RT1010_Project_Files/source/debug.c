#include "debug.h"

uint8_t debugTxBuffer[DEBUG_TX_BUFFER_SIZE];
uint8_t debugRxBuffer[DEBUG_RX_BUFFER_SIZE];

volatile uint8_t debugTxReadIndex;
volatile uint8_t debugTxWriteIndex;
volatile uint8_t debugTxCount;

volatile uint8_t debugRxReadIndex;
volatile uint8_t debugRxWriteIndex;
volatile uint8_t debugRxCount;

void DEBUG_LPUART_IRQHandler(void)
{
    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(DEBUG_LPUART))
    {
    	debugRxBuffer[debugRxWriteIndex] = LPUART_ReadByte(DEBUG_LPUART);
    	debugRxCount++;
    	debugRxWriteIndex++;
    	if(debugRxWriteIndex >= DEBUG_RX_BUFFER_SIZE)
    	{
    		debugRxWriteIndex = 0;
    	}
    }

    /* If Tx buffer has data to be sent out */
    if ((kLPUART_TxDataRegEmptyFlag)&LPUART_GetStatusFlags(DEBUG_LPUART))
    {
    	if(debugTxCount > 0)
    	{
    		LPUART_WriteByte(DEBUG_LPUART, debugTxBuffer[debugTxReadIndex]);
    		debugTxCount--;
    	    debugTxReadIndex++;
    	    if(debugTxReadIndex >= DEBUG_TX_BUFFER_SIZE)
    	    {
    	    	debugTxReadIndex = 0;
    	    }
    	}
    	else
    	{
    		LPUART_DisableInterrupts(DEBUG_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
    	}
    }

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void DEBUG_Init()
{
	lpuart_config_t config;
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DEBUG_LPUART, &config, DEBUG_LPUART_CLK_FREQ);

    debugTxReadIndex = 0;
    debugTxWriteIndex = 0;
    debugTxCount = 0;
    debugRxReadIndex = 0;
    debugRxWriteIndex = 0;
    debugRxCount = 0;

    /* Enable Rx full interrupts. */
    LPUART_EnableInterrupts(DEBUG_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(DEBUG_LPUART_IRQn);
}

void DEBUG_Refresh()
{
	uint32_t enabledInterrupts = LPUART_GetEnabledInterrupts(DEBUG_LPUART);

	//PRINTF("DEBUG %d %d \n\r", debugTxCount, debugRxCount);

	if((debugTxCount == 0) && (kLPUART_TxDataRegEmptyInterruptEnable & enabledInterrupts))
	{
		LPUART_DisableInterrupts(DEBUG_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
	}
	else if((debugTxCount > 0) && (!(kLPUART_TxDataRegEmptyInterruptEnable & enabledInterrupts)))
	{
		LPUART_EnableInterrupts(DEBUG_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
	}
}

uint8_t DEBUG_Available()
{
	if(debugRxCount > 0)
		return 1;

	else
		return 0;
}

uint8_t DEBUG_Read()
{
	uint8_t data;

	data = debugRxBuffer[debugRxReadIndex];
	debugRxCount--;
	debugRxReadIndex++;
	if(debugRxReadIndex >= DEBUG_RX_BUFFER_SIZE)
	{
		debugRxReadIndex = 0;
	}
	return data;
}

void DEBUG_Write(uint8_t data)
{
	debugTxBuffer[debugTxWriteIndex] = data;
	debugTxCount++;
	debugTxWriteIndex++;
	if(debugTxWriteIndex >= DEBUG_TX_BUFFER_SIZE)
	{
		debugTxWriteIndex = 0;
	}
}

void DEBUG_Write_String(char data[], uint8_t length)
{
	uint8_t index;

	for(index = 0; index < length; index++)
	{
		DEBUG_Write(data[index]);
	}
}
