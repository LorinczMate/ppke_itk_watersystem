################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CC2500.c \
../HB_DataLinkLayer.c \
../HB_PhysicLayer.c \
../HB_main.c \
../TI_CC_spi.c \
../UART.c \
../utility.c 

OBJS += \
./CC2500.o \
./HB_DataLinkLayer.o \
./HB_PhysicLayer.o \
./HB_main.o \
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


