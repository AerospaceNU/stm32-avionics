################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/lpm/tiny_lpm/stm32_lpm.c 

C_DEPS += \
./Utilities/lpm/tiny_lpm/stm32_lpm.d 

OBJS += \
./Utilities/lpm/tiny_lpm/stm32_lpm.o 


# Each subdirectory must supply rules for building sources it contributes
Utilities/lpm/tiny_lpm/%.o Utilities/lpm/tiny_lpm/%.su Utilities/lpm/tiny_lpm/%.cyclo: ../Utilities/lpm/tiny_lpm/%.c Utilities/lpm/tiny_lpm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/devices_common" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/cmsis" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/expressions" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/filtering" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/states" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/tasks" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/utils" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/tasks/cli" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/states/cli" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/stm32_projects/device_drivers" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/stm32_projects/device_drivers/radioconfig" -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-lpm-2f-tiny_lpm

clean-Utilities-2f-lpm-2f-tiny_lpm:
	-$(RM) ./Utilities/lpm/tiny_lpm/stm32_lpm.cyclo ./Utilities/lpm/tiny_lpm/stm32_lpm.d ./Utilities/lpm/tiny_lpm/stm32_lpm.o ./Utilities/lpm/tiny_lpm/stm32_lpm.su

.PHONY: clean-Utilities-2f-lpm-2f-tiny_lpm

