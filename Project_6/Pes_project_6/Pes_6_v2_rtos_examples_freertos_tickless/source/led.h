/******************************************************************************
 *  					PES PROJECT 5
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 					    	led.h
 ********************************************************************************/

#ifndef LED_H_

#define LED_H_
#include <stdint.h>
#include <stdio.h>
#include "board.h"
//#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "logger.h"
#include "fsl_debug_console.h"
#include "cir_buffer.h"



void init_LED(void);
void BLUE_LED(void);
void green_led(void);
void delay(uint32_t d);
void error_led();
#endif
