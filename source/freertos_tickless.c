/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

//#include "fsl_dac.h"
//#include "fsl_adc16.h"
#include "dacadc.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#if configUSE_TICKLESS_IDLE
#include "fsl_lptmr.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/

adc16_channel_config_t g_adc16ChannelConfigStruct;
volatile uint32_t g_Adc16ConversionValue = 0;

#define VREF_BRD 3.300
#define SE_12BIT 4096.0

#define BOARD_SW_GPIO BOARD_SW2_GPIO
#define BOARD_SW_PORT BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW2_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW2_NAME

/* @brief FreeRTOS tickless timer configuration. */
#define BOARD_LPTMR_IRQ_HANDLER LPTMR0_IRQHandler /*!< Timer IRQ handler. */
#define TICKLESS_LPTMR_BASE_PTR LPTMR0            /*!< Tickless timer base address. */
#define TICKLESS_LPTMR_IRQn LPTMR0_IRQn           /*!< Tickless timer IRQ number. */

/* Task priorities. */
/* clang-format off */
#define dac_task_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define adc_task_PRIORITY   ( configMAX_PRIORITIES - 2 )

//#define store_buffer1_PRIORITY ( configMAX_PRIORITIES - 3 )
//#define transfer_buffer2_PRIORITY ( configMAX_PRIORITIES - 3 )

#define TIME_DELAY_SLEEP      100

/* Interrupt priorities. */
#define SW_NVIC_PRIO 2

volatile uint16_t var_sinefunc;
volatile uint8_t n=0;

volatile bool g_Adc16ConversionDoneFlag = false;
/**************of no use ******************/
 static const unsigned int sine_values[] =
        {

				2482U, 2638U, 2791U, 2939U, 3080U, 3212U, 3332U, 3438U, 3530U, 3605U, 3663U, 3701U, 3721U, 3721U, 3702U, 3663U,
				3606U, 3531U, 3440U, 3333U, 3213U, 3082U, 2941U, 2793U, 2640U, 2484U, 2329U, 2176U, 2028U, 1886U, 1755U, 1635U,
				1528U, 1436U, 1360U, 1303U, 1264U, 1244U, 1243U, 1263U, 1301U, 1358U, 1433U, 1524U, 1630U, 1750U, 1881U, 2022U,
				2170U, 2323U, 2478U
        };


/* clang-format on */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void vPortLptmrIsr(void);
LPTMR_Type *vPortGetLptrmBase(void);
IRQn_Type vPortGetLptmrIrqn(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
void sine_func(void);
static void DAC_converter(void *pvParameters);
static void ADC_converter(void *pvParameters);
static void SW_task(void *pvParameters);

SemaphoreHandle_t xSWSemaphore = NULL;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */


void DEMO_ADC16_IRQ_HANDLER_FUNC(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
}


int main(void)
{
/* Define the init structure for the input switch pin */
#ifdef BOARD_SW_NAME
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
    };
#endif
#if configUSE_TICKLESS_IDLE
    lptmr_config_t lptmrConfig;

    CLOCK_EnableClock(kCLOCK_Lptmr0);
    /* Configuration LPTMR  */
    /*
     * lptmrConfig->timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig->pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig->pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig->enableFreeRunning = false;
     * lptmrConfig->bypassPrescaler = true;
     * lptmrConfig->prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig->value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(TICKLESS_LPTMR_BASE_PTR, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(TICKLESS_LPTMR_BASE_PTR, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(TICKLESS_LPTMR_IRQn);
#endif
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    EnableIRQ(DEMO_ADC16_IRQn);
    DAC_ADC_Init();
    /* Print a note to terminal. */
    PRINTF("Tickless Demo example\r\n");
#ifdef BOARD_SW_NAME
    PRINTF("Press %s to wake up the CPU\r\n", BOARD_SW_NAME);
    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
    NVIC_SetPriority(BOARD_SW_IRQ, SW_NVIC_PRIO);
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
#endif
    /*Create tickless task*/
    xTaskCreate(DAC_converter, "DAC_converter", configMINIMAL_STACK_SIZE + 38, NULL, dac_task_PRIORITY, NULL);
    xTaskCreate(ADC_converter, "ADC_converter", configMINIMAL_STACK_SIZE + 38, NULL, adc_task_PRIORITY, NULL);
  //  xTaskCreate(STORE_BUFFER_1, "STORE_BUFFER_1", configMINIMAL_STACK_SIZE + 38, NULL, store_buffer1_PRIORITY, NULL);
  //  xTaskCreate(TRANS_BUFFER_2, "TRANS_BUFFER_2", configMINIMAL_STACK_SIZE + 38, NULL, transfer_buffer2_PRIORITY, NULL);
    xTaskCreate(SW_task, "SW_task", configMINIMAL_STACK_SIZE + 38, NULL, dac_task_PRIORITY, NULL);
    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}

/* Tickless Task */
/* Tickless Task */
static void DAC_converter(void *pvParameters)
{
	uint16_t val_sinefunc;
    PRINTF("\r\nTick count :\r\n");
    for (;;)
    {
        PRINTF("\n\r%d", xTaskGetTickCount());
        //PRINTF("\r\n my sine count :\r\n");
    //	sine_func();

    	var_sinefunc = ((2 + sin((2*3.14*n)/(float)50)) / 3.3)*4096;
    		PRINTF("sine functionnnn %d",var_sinefunc);

        DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, var_sinefunc);
        vTaskDelay(TIME_DELAY_SLEEP);
//        j++;
//        if (j == 51)
//        {
//        j = 0;
//        }

    	n++;
        		if( n == 51 )
        		{
        			n = 0;
        		}

    }
}


static void ADC_converter(void *pvParameters)
{

	 float voltRead;
	 for (;;)
	 {
	 g_Adc16ConversionDoneFlag = false;
	 ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);
	 g_Adc16ConversionDoneFlag = true;
	 /* Read conversion result to clear the conversion completed flag. */
	 g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
	 while (!g_Adc16ConversionDoneFlag)
	  {
	  }
	 PRINTF("\r\n\r\nADC Value: %d\r\n", g_Adc16ConversionValue);

	 /* Convert ADC value to a voltage based on 3.3V VREFH on board */
	 voltRead = (float)(g_Adc16ConversionValue * (VREF_BRD / SE_12BIT));
	 PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
	 vTaskDelay(TIME_DELAY_SLEEP);
	 }
}


//static void STORE_BUFFER_1(void *pvParameters)
//{
//	 g_Adc16ConversionDoneFlag = false;
//	 ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);
//	 g_Adc16ConversionDoneFlag = true;
//	 /* Read conversion result to clear the conversion completed flag. */
//	 g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
//	 while (!g_Adc16ConversionDoneFlag)
//	  {
//	  }
//	 PRINTF("\r\n\r\nADC Value: %d\r\n", g_Adc16ConversionValue);
//
//	 /* Convert ADC value to a voltage based on 3.3V VREFH on board */
//	 voltRead = (float)(g_Adc16ConversionValue * (VREF_BRD / SE_12BIT));
//	 PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
//}
//
//static void TRANS_BUFFER_2(void *pvParameters)
//{
//	 g_Adc16ConversionDoneFlag = false;
//	 ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);
//	 g_Adc16ConversionDoneFlag = true;
//	 /* Read conversion result to clear the conversion completed flag. */
//	 g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
//	 while (!g_Adc16ConversionDoneFlag)
//	  {
//	  }
//	 PRINTF("\r\n\r\nADC Value: %d\r\n", g_Adc16ConversionValue);
//
//	 /* Convert ADC value to a voltage based on 3.3V VREFH on board */
//	 voltRead = (float)(g_Adc16ConversionValue * (VREF_BRD / SE_12BIT));
//	 PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
//}


/* Switch Task */
static void SW_task(void *pvParameters)
{
    xSWSemaphore = xSemaphoreCreateBinary();
    for (;;)
    {
    	PRINTF("IN SW TASKKKK LOOP");
        if (xSemaphoreTake(xSWSemaphore, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("CPU woke up by EXT interrupt\r\n");
        }

    }
}


/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function to wake up CPU
 */
#ifdef BOARD_SW_NAME
void BOARD_SW_IRQ_HANDLER(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Clear external interrupt flag. */
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

    xSemaphoreGiveFromISR(xSWSemaphore, &xHigherPriorityTaskWoken);
}
#endif
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
#if configUSE_TICKLESS_IDLE == 1
void BOARD_LPTMR_IRQ_HANDLER(void)
{
    vPortLptmrIsr();
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer base address
 */

LPTMR_Type *vPortGetLptrmBase(void)
{
    return TICKLESS_LPTMR_BASE_PTR;
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return TICKLESS_LPTMR_IRQn;
}
#endif /* configUSE_TICKLESS_IDLE */
