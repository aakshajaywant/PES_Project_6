/******************************************************************************
 *  					PES PROJECT 6
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 						 logger.h
 ********************************************************************************/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "cir_buffer.h"
#include "board.h"
//#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "freertos_tickless.h"


typedef enum
{
	debug,
	normal
}log_mode;

typedef enum
{
	buffinitialize = 0,
	buffcheck_full,
	buffcheck_empty,
	buffadd_item,
	buffremove_item,
	buffptr_valid,
	buffdestroy,
	buffresize,
	DACconverter,
	ADCconverter,
	dspmath,
	init_dma,
	dmatransfer
}log_func;


#define mode debug //      /**************CHANGE HERE*******************/
//#define mode normal

void timestamps(unsigned long long timer);
void log_level(log_mode mode);
void logger_func(log_func func_nm);
void log_messages(log_mode mode,log_func func_nm);
//void putstr(unsigned char *string);
#endif
