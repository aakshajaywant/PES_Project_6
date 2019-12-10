# PES_Project_6
PES_PROJECT_6
Author: AAKSHA JAYWANT & RUCHA BORWANKAR 
Project Name: FreeRTOS, ADC, DAC, DMA, DSP 

|| FILE CONTENTS:- ||
PES_Project_6

	-Includes
	-CMSIS
	-board
	-drivers
		-fsl_adc16.c
		-fsl_adc16.h
		-fsl_dma.c
		-fsl_dma.h
		-fsl_dmamux.c
		-fsl_dmamux.h
	-freeRTOS
	-source
		-dacadc.c
		-dacadc.h
		-cir_buffer.c
		-cir_buffer.h
		-dma.c
		-dma.h
		-led.c
		-led.h
		-logger.c
		-logger.h
		-freertos_tickless.c
		-FreeRTOSConfig.h
	-startup
	-utilities
	-Debug
	-README.md

|| OBSERVATIONS :- ||
1) Generated sine wave from the DAC and fed them to ADC to generate a 12bit digital output.
2) Transferred digital data to circular buffer.
3) Executed DMA transfer to dsp buffer once the circular buffer gets filled.
4) Performed standard deviation once the data.


|| EXECUTION INSTRUCTIONS :- ||
Following listed steps should be used for running the program in MCUXPresso IDE:-

1)To choose amongst test, debug and normal mode, change the "#define mode debug //normal" in the "logger.h" file.

2)Build the program

3)Debug as GDB PEMicro Interface Debugger

4)For the extra credit queues,please change the "#define QUE_MODE ON//OFF" in the "freertos_tickless.c".

|| TASKS UNDERTAKEN :- ||
Part 1. Lookup table creation and Validation
To Calculate and create a lookup table to represent the values in a sine wave in a code that runs from 1V to 3V.
Period from peak to peak should be 5 seconds with a .1 second step. 
In code, we mathematically determine the voltage values at each .1 second step and then convert the voltages to DAC register values representing the voltages at each .1 second
step.

Part 2. Capture the output from DAC0 on ADC0 and Creation of Free RTOS tasks

Task 1:-To Create a FreeRTOS Task to periodically (every .1 seconds) change the value on DAC0 from the lookup table
buffer of DAC register values.

1)DAC_ADC Initialization Function-Sine wave generation.

Task 2:-Create a FreeRTOS Task to periodically (every .1 seconds) read the DAC0 value via ADC0 and store it in a circular
buffer. When the buffer is full initiate a DMA transfer from the ADC buffer to a second buffer.Also we Clear (or overwrite) the ADC buffer with incoming DAC values and continue
sampling until the next series of samples are collected.

1)Circular Buffer c-Function to add a new item to a buffer

	-Function to remove oldest item from a buffer
	-Function to check is buffer full
	-Function to check is buffer empty
	-Function to verify buffer is initialized
	-Function to verify the buffer pointer is valid
	-Function to initialize a buffer
	-Function to destroy a buffer

2)Timestamps-Used at the start and end of DMA transfer.

3)Logging – added timestamps using systick handler that is used to run Free RTOS. 

4)LEDs-Controlled using Semaphores

5)DMA transfer-Functions to transfer adc buffer to dsp buffer.
	-Function to initialize DMA transfer 
	-Function to transfer using DMA

6)Extra Credit (10 points) –  5 points to use a FreeRTOS Mutex or Semaphore to control access
to the shared LED and 5 points to use a FreeRTOS Queue in place of one of the use of your custom buffers.

Task 3-To Create a Task that is triggered by an interrupt from completing the DMA transfer of data into the DSP
buffer and to Calculate the maximum, minimum,average, and standard deviation of voltage levels. 
All the tasks run 5 times and then the process is stopped.

1)Floating Point Math operation-Math operation function which calculates the  maximum, minimum,average, and standard deviation of voltage levels.
