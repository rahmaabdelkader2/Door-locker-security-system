################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD.c \
../MC1_main.c \
../functions.c \
../gpio.c \
../keypad.c \
../timer.c \
../uart.c 

OBJS += \
./LCD.o \
./MC1_main.o \
./functions.o \
./gpio.o \
./keypad.o \
./timer.o \
./uart.o 

C_DEPS += \
./LCD.d \
./MC1_main.d \
./functions.d \
./gpio.d \
./keypad.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


