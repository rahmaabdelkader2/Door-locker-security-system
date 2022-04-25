################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BUZZER.c \
../DC_motor.c \
../MC2_main.c \
../PWM.c \
../external_eeprom.c \
../functions.c \
../gpio.c \
../i2c.c \
../timer.c \
../uart.c 

OBJS += \
./BUZZER.o \
./DC_motor.o \
./MC2_main.o \
./PWM.o \
./external_eeprom.o \
./functions.o \
./gpio.o \
./i2c.o \
./timer.o \
./uart.o 

C_DEPS += \
./BUZZER.d \
./DC_motor.d \
./MC2_main.d \
./PWM.d \
./external_eeprom.d \
./functions.d \
./gpio.d \
./i2c.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


