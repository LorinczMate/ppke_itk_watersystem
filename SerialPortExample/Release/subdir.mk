################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CC2500.c \
../M_DataLinkLayer.c \
../M_PhysicLayer.c \
../M_main.c \
../TI_CC_spi.c \
../UART.c \
../utility.c 

OBJS += \
./CC2500.o \
./M_DataLinkLayer.o \
./M_PhysicLayer.o \
./M_main.o \
./TI_CC_spi.o \
./UART.o \
./utility.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 C Compiler'
	msp430-gcc -O3 -g0 -Wall -mmcu=msp430f2274 -std=gnu99 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


