/******************************************************************************
* PES PROJECT 6
* AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
* Cross Platform IDE: MCUXpresso IDE v11
* Cross-Compiler: ARM GCC
* freertos_tickless.c
* REFERENCES:-
* https://www.programiz.com/c-programming/examples/standard-deviation
* MCU SDK FREE RTOS EXAMPLES *
********************************************************************************/
/**************Header files**************/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "logger.h"
#include "dacadc.h"
#include "queue_adc.h"
#include "dma.h"
#include "cir_buffer.h"
#include "led.h"
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
//#include "fsl_dac.h"
//#include "fsl_adc16.h"
#if configUSE_TICKLESS_IDLE
#include "fsl_lptmr.h"
#endif
//#define ON 0
//#define OFF 1
////#define QUE_MODE == ON
//#define QUE_MODE == OFF
//static QueueHandle_t adc_queue = NULL;
/*******************************************************************************
* Definitions
******************************************************************************/
extern unsigned long long timecount;
//static QueueHandle_t adc_queue = NULL;
//static QueueHandle_t dsp_queue = NULL;
//static SemaphoreHandle_t xSemaphore_led;
//static SemaphoreHandle_t xSemaphore_producer;
adc16_channel_config_t g_adc16ChannelConfigStruct;
volatile uint16_t g_Adc16ConversionValue = 0;
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
#define TICKLESS_LPTMR_BASE_PTR LPTMR0 /*!< Tickless timer base address.
*/
#define TICKLESS_LPTMR_IRQn LPTMR0_IRQn /*!< Tickless timer IRQ number. */
/* Task priorities. */
/* clang-format off */
#define dac_task_PRIORITY ( configMAX_PRIORITIES - 4 )
#define adc_task_PRIORITY ( configMAX_PRIORITIES - 4 )
#define dsp_math_PRIORITY ( configMAX_PRIORITIES - 4 )
//#define store_buffer1_PRIORITY ( configMAX_PRIORITIES - 3 )
//#define transfer_buffer2_PRIORITY ( configMAX_PRIORITIES - 3 )
#define TIME_DELAY_SLEEP 100
/* Interrupt priorities. */
#define SW_NVIC_PRIO 2
volatile uint16_t var_sinefunc;
volatile uint8_t n=0,t3flag=0;
volatile uint8_t num_execute=1;
volatile bool g_Adc16ConversionDoneFlag = false;
//uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
TaskHandle_t TaskHandle_1;
ring_buffer *adc_buff;
uint16_t dsp_buff[100];
ring_status receive_status;
/**************of no use ******************/
static const unsigned int sine_values[] =
 {
2482U, 2638U, 2791U, 2939U, 3080U, 3212U, 3332U, 3438U,
3530U, 3605U, 3663U, 3701U, 3721U, 3721U, 3702U, 3663U,
3606U, 3531U, 3440U, 3333U, 3213U, 3082U, 2941U, 2793U,
2640U, 2484U, 2329U, 2176U, 2028U, 1886U, 1755U, 1635U,
1528U, 1436U, 1360U, 1303U, 1264U, 1244U, 1243U, 1263U,
1301U, 1358U, 1433U, 1524U, 1630U, 1750U, 1881U, 2022U,
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
static void dsp_math(void *pvParameters);
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
 g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR,
DEMO_ADC16_CHANNEL_GROUP);
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
 /* Configuration LPTMR */
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
// xSemaphore_led = xSemaphoreCreateMutex();
//#if QUE_MODE == ON
// adc_queue = xQueueCreate(64, 32);
// // dsp_queue = xQueueCreate(64, 32);
//#endif
 /* Print a note to terminal. */
 // PRINTF("Tickless Demo example\r\n");
#ifdef BOARD_SW_NAME
 PRINTF("Press %s to wake up the CPU\r\n", BOARD_SW_NAME);
 /* Init input switch GPIO. */
 PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN,
kPORT_InterruptFallingEdge);
 NVIC_SetPriority(BOARD_SW_IRQ, SW_NVIC_PRIO);
 EnableIRQ(BOARD_SW_IRQ);
 GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
#endif
 /*Create tickless task*/
 xTaskCreate(DAC_converter, "DAC_converter", configMINIMAL_STACK_SIZE + 90, NULL,
dac_task_PRIORITY, NULL);
 xTaskCreate(ADC_converter, "ADC_converter", configMINIMAL_STACK_SIZE + 90, NULL,
adc_task_PRIORITY, NULL);
 // xTaskCreate(dsp_math,"dsp_math", configMINIMAL_STACK_SIZE + 38, NULL,dsp_math_PRIORITY,TaskHandle_1);
 // xTaskCreate(STORE_BUFFER_1, "STORE_BUFFER_1", configMINIMAL_STACK_SIZE + 38,NULL, store_buffer1_PRIORITY, NULL);
 // xTaskCreate(TRANS_BUFFER_2, "TRANS_BUFFER_2", configMINIMAL_STACK_SIZE + 38,NULL, transfer_buffer2_PRIORITY, NULL);
 // xTaskCreate(SW_task, "SW_task", configMINIMAL_STACK_SIZE + 90, NULL,dac_task_PRIORITY, NULL);
 /*Task Scheduler*/
 PRINTF("/*****************************************TASKS BEGIN************************************/");
 vTaskStartScheduler();
 for (;;)
 ;
}
/* Tickless Task */
/******************DAC_converter**********************************
This task gives the mathematical values that generate the sine wave with
sine wave that runs from 1V to 3V. Period from peak to peak should
be 5 seconds with a .1 second step.
************************************************************************/
static void DAC_converter(void *pvParameters)
{
log_messages(mode,DACconverter);
uint16_t val_sinefunc;
if(mode == debug)
{
 PRINTF("\r\nTick count : ");
}
 for (;;)
 {
 if(mode == debug)
 {
 PRINTF("%d \n\r", xTaskGetTickCount());
 }
 //PRINTF("\r\n my sine count :\r\n");
 /******************Semaphore mutex implementation**********************************
 mutex based semaphore is used to share LED between the tasks.
 ************************************************************************/
// if( xSemaphore_led != NULL )
// { /* See if the mutex can be obtained. If the mutex is not available wait 10 ticks to see if it becomes free. */
// if( xSemaphoreTake( xSemaphore_led, 0 ) == pdTRUE )
// {
 init_LED();
 green_led();
//Green LED is toggled
// xSemaphoreGive( xSemaphore_led );
// }
// }
 // sine_func();
 var_sinefunc = ((2 + sin((2*3.14*n)/(float)50)) / 3.3)*4096;
 if(mode == debug)
 {
 PRINTF("\n \r SIN FUNCTION VALUES ARE %d",var_sinefunc);
 }
 DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, var_sinefunc);
 vTaskDelay(TIME_DELAY_SLEEP);
// j++;
// if (j == 51)
// {
// j = 0;
// }
 n++;
 if( n == 51 )
 {
 n = 0;
 }
 }
}
/******************ADC_converter**********************************
This task is used to store the ADC values in circular buffer/queues .
Since we have a buffer of capacity 64,after 64 values it will
initiate DMA transfer
************************************************************************/
static void ADC_converter(void *pvParameters)
{
log_messages(mode,ADCconverter);
uint8_t count=0;
// float voltRead;
uint16_t voltRead;
//#if QUE_MODE == OFF
adc_buff = (ring_buffer*)malloc(sizeof(ring_buffer));
receive_status = buff_initialize(adc_buff, 64);
//#endif
if(mode == debug)
 {
PRINTF("RECEIVE STATUS for buff initialization is %d",receive_status);
 }
for (;;)
{
g_Adc16ConversionDoneFlag = false;
ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP,
&g_adc16ChannelConfigStruct);
g_Adc16ConversionDoneFlag = true;
/* Read conversion result to clear the conversion completed flag. */
g_Adc16ConversionValue =
ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
while (!g_Adc16ConversionDoneFlag)
{
}
PRINTF("\r\n\r\nADC Value: %d\r\n", g_Adc16ConversionValue);
/* Convert ADC value to a voltage based on 3.3V VREFH on board */
voltRead = (uint16_t)(g_Adc16ConversionValue * (VREF_BRD / SE_12BIT));
// PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
PRINTF("\r\n ADC Voltage: %d\r\n", voltRead);
// xQueueSend(adc_queue, &voltRead, 10);
if(count==64)
{
PRINTF("\n \r DMA transfer done from ADC TO DSP");
PRINTF("\n \r DMA Start time is:");
timestamps(timecount);
DMA();
//#if QUE_MODE == OFF
transfer_DMA(adc_buff->buffer,dsp_buff,64*2);
//#endif
// #if QUE_MODE == ON
// transfer_DMA_2(adc_queue,dsp_buff,64*2); //for queue transfer to array
// #endif
// #if QUE_MODE == OFF
/******************Semaphore mutex
implementation**********************************
 mutex based semaphore is used to share LED between the
tasks.

************************************************************************/
// if( xSemaphore_led != NULL )
// { /* See if the mutex can be obtained. If the mutex is not available wait 10 ticks to see if it becomes free. */
// if( xSemaphoreTake( xSemaphore_led, 0 ) == pdTRUE )
// {
uint8_t m;
 init_LED();
 BLUE_LED();
 for(m=0;m<250;m++);
// xSemaphoreGive( xSemaphore_led );
// }
// }
// PRINTF("ADC_BUFFER %p",adc_buff->buffer);
PRINTF("\n \r DMA End time is:");
timestamps(timecount);
uint8_t i;
// for(i=0;i<64;i++)
// {
// PRINTF("\n \r DSP buffer values are %d",dsp_buff[i]);
// }
count=0;
// xTaskCreate(dsp_math,"dsp_math", configMINIMAL_STACK_SIZE +38, NULL, 0,TaskHandle_1);
// xQueueReset(adc_queue);
xTaskCreate(dsp_math, "dsp_math", configMINIMAL_STACK_SIZE + 38,
NULL, dsp_math_PRIORITY, NULL);
// xTaskResumeFromISR( TaskHandle_1 );
}
else
{
if(mode==debug){
PRINTF("\n\r COUNT LESS THAN 64: %d",count);
}
count++;
//#if QUE_MODE == OFF
receive_status = buff_add_item(adc_buff,g_Adc16ConversionValue);
//#endif
// PRINTF("RECEIVE STATUS for add itemmmm of adcccc bufferrrrr ISSS %d",receive_status);
// #if QUE_MODE == ON
// printf("\n \r QUEUE mode on");
// timestamps(timecount);
// uint16_t receive;
// xQueueSend(adc_queue, &g_Adc16ConversionValue, 10);
// //xQueueReceive( adc_queue, &receive, portMAX_DELAY );
// //PRINTF("queue has value %d",receive);
// #endif
}
vTaskDelay(TIME_DELAY_SLEEP);
}
}
/* Switch Task */
static void SW_task(void *pvParameters)
{
 xSWSemaphore = xSemaphoreCreateBinary();
 for (;;)
 {
 PRINTF("IN SW TASK LOOP");
 if (xSemaphoreTake(xSWSemaphore, portMAX_DELAY) == pdTRUE)
 {
 PRINTF("CPU woke up by EXT interrupt\r\n");
 }
 }
}
/******************DSP_MATH**********************************
This task is used to calculate minimum,maximum,standard deviation and average
of all the values in the dsp buffer.
After 4 cycles,the tasks are terminated.
************************************************************************/
static void dsp_math(void *pvParameters)
{
log_messages(mode,dspmath);
for(;;)
{
float max=0,min=4096,avg=0,sd=0,cal=0,buff_val[64];
cal = 3.3/4096; //step
uint8_t count;
for( count=0;count<64;count++)
{
if((*dsp_buff+count) < min)
{
min = (*dsp_buff+count);
}
else if((*dsp_buff+count)>max)
{
max= (*dsp_buff+count);
}
avg = avg + (*dsp_buff+count);
//adc_buffer++;
}
min = min*cal;
max = max*cal;
avg = avg*cal;
avg = avg/64.00;
for (uint8_t count2 = 0; count2 < 64;count2++)
{
buff_val[count2] = (*dsp_buff+count)*cal;
sd = sd + pow(((*dsp_buff+count) - avg), 2);
}
sd=sqrt(sd/64.00);
PRINTF("\n \r Min=%f,Max=%f,Avg=%f,sd=%f",min,max,avg,sd);
PRINTF("\n \r /********************NUM COUNT IS: %d***********************/",num_execute);
num_execute++;
if(num_execute >4)
{
PRINTF("\n \r *****************************************TASKS SUSPENDED**************************/");
PRINTF("\n \r*************************************************************************************************************************************************");
//vTaskSuspendAll(TaskHandle_1);
vTaskEndScheduler();
}
vTaskSuspend(TaskHandle_1);
}
//num_execute++;
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










