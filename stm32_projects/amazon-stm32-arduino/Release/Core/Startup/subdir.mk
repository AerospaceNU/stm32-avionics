################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f401ccux.s 

S_DEPS += \
./Core/Startup/startup_stm32f401ccux.d 

OBJS += \
./Core/Startup/startup_stm32f401ccux.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -I"D:/Documents/GitHub/stm32-avionics/common/devices_common" -I"D:/Documents/GitHub/stm32-avionics/common/altos" -I"D:/Documents/GitHub/stm32-avionics/common/system" -I"D:/Documents/GitHub/stm32-avionics/common/system/cmsis" -I"D:/Documents/GitHub/stm32-avionics/common/system/expressions" -I"D:/Documents/GitHub/stm32-avionics/common/system/filtering" -I"D:/Documents/GitHub/stm32-avionics/common/system/states" -I"D:/Documents/GitHub/stm32-avionics/common/system/tasks" -I"D:/Documents/GitHub/stm32-avionics/common/utils" -I"D:/Documents/GitHub/stm32-avionics/stm32_projects/device_drivers" -I"D:/Documents/GitHub/stm32-avionics/stm32_projects/device_drivers/radioconfig" -I"D:/Documents/GitHub/stm32-avionics/common/system/tasks/cli" -I"D:/Documents/GitHub/stm32-avionics/common/system/states/cli" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f401ccux.d ./Core/Startup/startup_stm32f401ccux.o

.PHONY: clean-Core-2f-Startup

