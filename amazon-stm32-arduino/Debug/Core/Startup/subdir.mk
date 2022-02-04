################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
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
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/nathan/Documents/DPF/stm32-avionics/System" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Tasks" -I"/home/nathan/Documents/DPF/stm32-avionics/Utils" -I"/home/nathan/Documents/DPF/stm32-avionics/Devices" -I"/home/nathan/Documents/DPF/stm32-avionics/System/States/Cli" -I"/home/nathan/Documents/DPF/stm32-avionics/System/Filtering" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

