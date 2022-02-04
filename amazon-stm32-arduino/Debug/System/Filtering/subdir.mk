################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/nathan/Documents/DPF/stm32-avionics/System/Filtering/altitude_kalman.cpp 

OBJS += \
./System/Filtering/altitude_kalman.o 

CPP_DEPS += \
./System/Filtering/altitude_kalman.d 


# Each subdirectory must supply rules for building sources it contributes
System/Filtering/altitude_kalman.o: /home/nathan/Documents/DPF/stm32-avionics/System/Filtering/altitude_kalman.cpp System/Filtering/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../System -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

