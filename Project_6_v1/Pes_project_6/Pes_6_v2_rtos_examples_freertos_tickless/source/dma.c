/******************************************************************************
 *  					PES PROJECT 6
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 						dma.c
 ********************************************************************************/
#include "dma.h"

//#include "cir_buffer.h"
//#include "logger.h"
dma_handle_t g_DMA_Handle;


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
    	log_messages(mode,init_dma);
    	DMAMUX_Init(DMAMUX0);
		DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, DMA_SOURCE);
		DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);
		DMA_Init(DMA0);
    }
    /* Configure DMA one shot transfer */
    void transfer_DMA(uint16_t *src,uint16_t *dest,uint8_t transfer_data)
    {
		log_messages(mode,dmatransfer);
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


#if QUE_MODE == ON
    void transfer_DMA_2(QueueHandle_t *src,uint16_t *dest,uint8_t transfer_data)
    {
    	log_messages(mode,dmatransfer);
    DMA_CreateHandle(&g_DMA_Handle, DMA0, DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&transferConfig,src,2,dest,2,transfer_data,kDMA_MemoryToMemory);
    DMA_SubmitTransfer(&g_DMA_Handle, &transferConfig, kDMA_EnableInterrupt);
    DMA_StartTransfer(&g_DMA_Handle);
    /* Wait for DMA transfer finish */
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
    #endif

