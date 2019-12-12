################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/cir_buffer.c \
../source/dacadc.c \
../source/dma.c \
../source/freertos_tickless.c \
../source/led.c \
../source/logger.c \
../source/mtb.c \
../source/queue_adc.c \
../source/semihost_hardfault.c 

OBJS += \
./source/cir_buffer.o \
./source/dacadc.o \
./source/dma.o \
./source/freertos_tickless.o \
./source/led.o \
./source/logger.o \
./source/mtb.o \
./source/queue_adc.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/cir_buffer.d \
./source/dacadc.d \
./source/dma.d \
./source/freertos_tickless.d \
./source/led.d \
./source/logger.d \
./source/mtb.d \
./source/queue_adc.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../freertos -I../startup -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


