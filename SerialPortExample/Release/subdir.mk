################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UART.c \
../main.c 

OBJS += \
./UART.o \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 C Compiler'
	msp430-gcc -O3 -g0 -Wall -mmcu=msp430f2274 -std=gnu99 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


