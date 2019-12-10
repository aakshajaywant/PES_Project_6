/******************************************************************************
 *  					PES PROJECT 6
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 					    	led.h
 ********************************************************************************/

#include "led.h"

void init_LED(void);
void start(void);
void error(void);
void _end(void);

void init_LED(void)
{
      LED_BLUE_INIT(1);
      LED_RED_INIT(1);
      LED_GREEN_INIT(1);
}

void BLUE_LED(void)		//wait to receive data
{
	LED_RED_OFF();
	LED_GREEN_OFF();
	LED_BLUE_TOGGLE();
	delay(10);
}

void error_led(void)	//error or disconnected state
{
	LED_GREEN_OFF();
	LED_BLUE_OFF();
	LED_RED_ON();
	delay(10);
}

void green_led(void)	//when transmitting data
{
	LED_RED_OFF();
	LED_BLUE_OFF();
	//LED_GREEN_ON();
	LED_GREEN_TOGGLE();
	delay(10);
}


void delay(uint32_t d)
{
	uint32_t count = d*7000;	/****** As clock is 8MHz *****/
	while(count!=0)
	{
count--;/***** Wasting MCU cycles to get the desired delay ******/
	}
}


