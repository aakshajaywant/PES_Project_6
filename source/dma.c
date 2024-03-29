/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
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
#include "dma.h"
#include "board.h"

#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"
//#include "cir_buffer.h"
//#include "logger.h"
#include <stdio.h>
#include "board.h"
//#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"


//ring_status receive_status,receive_status2;
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFF_LENGTH 4
#define DMA_CHANNEL 0
#define DMA_SOURCE 63
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
dma_handle_t g_DMA_Handle;
volatile bool g_Transfer_Done = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for DMA transfer. */
void DMA_Callback(dma_handle_t *handle, void *param)
{
    g_Transfer_Done = true;
}

/* Main function. Execute DMA transfer with transactional APIs. */
//int main(void)
//{
//    uint32_t srcAddr[BUFF_LENGTH] = {0x01, 0x02, 0x03, 0x04};
//    uint32_t destAddr[BUFF_LENGTH] = {0x00, 0x00, 0x00, 0x00};
//    uint32_t i = 0;
    dma_transfer_config_t transferConfig;

//    BOARD_InitPins();
//    BOARD_BootClockRUN();
//    BOARD_InitDebugConsole();

//    s_buff = (ring_buffer*)malloc(sizeof(ring_buffer));
//    PRINTF("\n\r %d",sizeof(ring_buffer));
//    receive_status = buff_initialize(s_buff, 10);
//    for(int i=0;i<15;i++){
//    char a='5';
//    buff_add_item(s_buff,a);
//    a++;
//    }

//    d_buff = (ring_buffer*)malloc(sizeof(ring_buffer));
//    PRINTF("\n\r %d",sizeof(ring_buffer));
//    receive_status = buff_initialize(d_buff, 10);
//    for(int i=0;i<15;i++){
//    char b='4';
//    buff_add_item(d_buff,b);
//    b++;
//   }
    /* Print source buffer */
//    PRINTF("\n \r DMA memory to memory transfer example begin.\r\n\r\n");
//    PRINTF("Destination Buffer:\r\n");
//    for (i = 0; i < BUFF_LENGTH; i++)
//    {
//        PRINTF("%d\t", destAddr[i]);
//    }
    /* Configure DMAMUX */
    void DMA(void)
    {
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);
    DMA_Init(DMA0);
    }
    /* Configure DMA one shot transfer */
    void transfer_DMA(ring_buffer *src,uint16_t *dest,uint8_t transfer_data)
    {
    DMA_CreateHandle(&g_DMA_Handle, DMA0, DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&transferConfig,src,2,dest,2,transfer_data,kDMA_MemoryToMemory);
    DMA_SubmitTransfer(&g_DMA_Handle, &transferConfig, kDMA_EnableInterrupt);
    DMA_StartTransfer(&g_DMA_Handle);
    /* Wait for DMA transfer finish */
//    }
    while (g_Transfer_Done != true)
    {
    }
    /* Print destination buffer */
//    PRINTF("\r\n\r\nDMA memory to memory transfer example finish.\r\n\r\n");
//    PRINTF("Destination Buffer:\r\n");
//    for(int i=0;i<16;i++)
//    {
//    buff_remove_item(d_buff);
//    d_buff++;
//    }
    }


