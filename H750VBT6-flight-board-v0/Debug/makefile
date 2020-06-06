################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
H750VBT6-flight-board-v0.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
H750VBT6-flight-board-v0.list \

OBJCOPY_BIN += \
H750VBT6-flight-board-v0.bin \


# All Target
all: H750VBT6-flight-board-v0.elf secondary-outputs

# Tool invocations
H750VBT6-flight-board-v0.elf: $(OBJS) $(USER_OBJS) C:\Users\amirz\Documents\AeroNU\DPF\Avionics\stm32-avionics\H750VBT6-flight-board-v0\STM32H750VBTX_FLASH.ld
	arm-none-eabi-gcc -o "H750VBT6-flight-board-v0.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\amirz\Documents\AeroNU\DPF\Avionics\stm32-avionics\H750VBT6-flight-board-v0\STM32H750VBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="H750VBT6-flight-board-v0.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

H750VBT6-flight-board-v0.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "H750VBT6-flight-board-v0.list"
	@echo 'Finished building: $@'
	@echo ' '

H750VBT6-flight-board-v0.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "H750VBT6-flight-board-v0.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
