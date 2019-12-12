/******************************************************************************
 *  					PES PROJECT 6
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 						dma.h
 ********************************************************************************/
#ifndef DMA_H_
#define DMA_H_


#include "cir_buffer.h"
#include "board.h"
#include "freertos_tickless.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include <stdio.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "queue.h"


void DMA_Callback(dma_handle_t *handle, void *param);
void DMA(void);
void transfer_DMA(uint16_t *src,uint16_t *dest,uint8_t transfer_data);
void transfer_DMA_2(QueueHandle_t *src,uint16_t *dest,uint8_t transfer_data);

#endif /* DMA_H_ */
