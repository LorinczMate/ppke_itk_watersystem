################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BS_DataLinkLayer.c \
../BS_PhysicLayer.c \
../BS_main.c \
../CC2500.c \
../TI_CC_spi.c \
../UART.c \
../utility.c 

OBJS += \
./BS_DataLinkLayer.o \
./BS_PhysicLayer.o \
./BS_main.o \
./CC2500.o \
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


