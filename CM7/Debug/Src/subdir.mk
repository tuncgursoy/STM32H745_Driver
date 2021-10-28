################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Clock.c \
../Src/Control.c \
../Src/Gpio.c \
../Src/Systick.c \
../Src/Uart.c \
../Src/User_led.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/Clock.o \
./Src/Control.o \
./Src/Gpio.o \
./Src/Systick.o \
./Src/Uart.o \
./Src/User_led.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/Clock.d \
./Src/Control.d \
./Src/Gpio.d \
./Src/Systick.d \
./Src/Uart.d \
./Src/User_led.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DNUCLEO_H745ZI_Q -DSTM32 -DSTM32H745ZITx -DSTM32H7DUAL -DSTM32H7 -c -I../Inc -I"E:/stm32WorkSpace/STM32H745/en.stm32cubeh7_v1-9-0_v1.9.0/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"E:/stm32WorkSpace/STM32H745/en.stm32cubeh7_v1-9-0_v1.9.0/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

