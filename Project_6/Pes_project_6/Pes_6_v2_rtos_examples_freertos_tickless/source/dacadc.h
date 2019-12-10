/******************************************************************************
 *  					PES PROJECT 6
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 						dacadc.h
 ********************************************************************************/

#ifndef DACADC_H_
#define DACADC_H_

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc16.h"
#include "fsl_dac.h"
#include <stdint.h>
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASEADDR ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 0U /* PTE20, ADC0_SE0 */
#define DEMO_DAC_BASEADDR DAC0

#define DEMO_ADC16_IRQn ADC0_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define DAC_1_0_VOLTS 1241U
#define DAC_1_5_VOLTS 1862U
#define DAC_2_0_VOLTS 2482U
#define DAC_2_5_VOLTS 3103U
#define DAC_3_0_VOLTS 3724U




/*******************************************************************************
 * Variables
 ******************************************************************************/


//adc16_channel_config_t g_adc16ChannelConfigStruct;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialize ADC16 & DAC */
void DAC_ADC_Init(void);



#endif
