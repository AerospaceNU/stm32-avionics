################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/nathan/Documents/DPF/stm32-avionics/System/cli.c \
/home/nathan/Documents/DPF/stm32-avionics/System/data_log.c \
/home/nathan/Documents/DPF/stm32-avionics/System/data_transmission.c \
/home/nathan/Documents/DPF/stm32-avionics/System/hardware_manager.c \
/home/nathan/Documents/DPF/stm32-avionics/System/state_log.c 

CPP_SRCS += \
/home/nathan/Documents/DPF/stm32-avionics/System/filters.cpp \
/home/nathan/Documents/DPF/stm32-avionics/System/scheduler.cpp 

C_DEPS += \
./System/cli.d \
./System/data_log.d \
./System/data_transmission.d \
./System/hardware_manager.d \
./System/state_log.d 

OBJS += \
./System/cli.o \
./System/data_log.o \
./System/data_transmission.o \
./System/filters.o \
./System/hardware_manager.o \
./System/scheduler.o \
./System/state_log.o 

CPP_DEPS += \
./System/filters.d \
./System/scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
System/cli.o: /home/nathan/Documents/DPF/stm32-avionics/System/cli.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/data_log.o: /home/nathan/Documents/DPF/stm32-avionics/System/data_log.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/data_transmission.o: /home/nathan/Documents/DPF/stm32-avionics/System/data_transmission.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/filters.o: /home/nathan/Documents/DPF/stm32-avionics/System/filters.cpp System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../System -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/hardware_manager.o: /home/nathan/Documents/DPF/stm32-avionics/System/hardware_manager.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/scheduler.o: /home/nathan/Documents/DPF/stm32-avionics/System/scheduler.cpp System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../System -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/state_log.o: /home/nathan/Documents/DPF/stm32-avionics/System/state_log.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

