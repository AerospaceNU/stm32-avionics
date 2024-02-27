################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.c 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.d 

OBJS += \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/ble/core/auto/%.o Middlewares/ST/STM32_WPAN/ble/core/auto/%.su Middlewares/ST/STM32_WPAN/ble/core/auto/%.cyclo: ../Middlewares/ST/STM32_WPAN/ble/core/auto/%.c Middlewares/ST/STM32_WPAN/ble/core/auto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/devices_common" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/cmsis" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/expressions" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/filtering" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/states" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/tasks" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/utils" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/tasks/cli" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/common/system/states/cli" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/stm32_projects/device_drivers" -I"/Users/iris/Documents/Personal Projects/AeroNU/stm32-avionics/stm32_projects/device_drivers/radioconfig" -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-auto

clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-auto:
	-$(RM) ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_events.su ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.su ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.su ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.su ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.su ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.cyclo ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.d ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.o ./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-ble-2f-core-2f-auto
