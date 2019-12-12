#ifndef FREERTOS_TICKLESS_H_
#define FREERTOS_TICKLESS_H_

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

#if configUSE_TICKLESS_IDLE
#include "fsl_lptmr.h"

#define ON 1
#define OFF 0
#define QUE_MODE ON
//#define QUE_MODE OFF




#endif
#endif
