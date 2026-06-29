################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include test/subdir.mk
-include src/subdir.mk
-include gs32_driver/mathlib/subdir.mk
-include gs32_driver/init/subdir.mk
-include gs32_driver/driverlib/sysctl/subdir.mk
-include gs32_driver/driverlib/gpio/subdir.mk
-include gs32_driver/driverlib/dsp/startup_dsp/subdir.mk
-include gs32_driver/driverlib/dsp/subdir.mk
-include gs32_driver/driverlib/subdir.mk
-include gs32_driver/common/subdir.mk
-include bsp/bsp_sci/subdir.mk
-include bsp/bsp_i2c/subdir.mk
-include bsp/bsp_hrpwm/subdir.mk
-include bsp/bsp_gpio/subdir.mk
-include bsp/bsp_eqep/subdir.mk
-include bsp/bsp_epwm/subdir.mk
-include bsp/bsp_cputimer/subdir.mk
-include bsp/bsp_cmpss/subdir.mk
-include bsp/bsp_can/subdir.mk
-include bsp/bsp_adc/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := gs32f00137_epwm_adc_cmpss
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
gs32f00137_epwm_adc_cmpss.bin \

SECONDARY_LIST += \
gs32f00137_epwm_adc_cmpss.lst \

SECONDARY_SIZE += \
gs32f00137_epwm_adc_cmpss.siz \


# All Target
all: main-build

# Main-build Target
main-build: gs32f00137_epwm_adc_cmpss.elf secondary-outputs

# Tool invocations
gs32f00137_epwm_adc_cmpss.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GS RISC-V C Linker'
	gs-riscv-elf-gcc -march=rv32imafc_xgsdida -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore --gsdi_ext1p2h -mgs_aux -O1 -fmessage-length=0 -ffunction-sections -fdata-sections -fsingle-precision-constant -Wfatal-errors -funsigned-char --opt_level=off -O1 -g -ggdb -T "C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/dsp/ld_scripts_dsp/gs32f00xx_dsp_flashilm.ld" -T "C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/dsp/ld_scripts_dsp/GS32F00137_mem.ld" -T "C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/dsp/ld_scripts_dsp/gs32f00xx_dsp_3.0_regs.ld" -nostartfiles -Xlinker --gc-sections -L"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\libraries\HRPWM_Calibration" -Xlinker --no-warn-rwx-segments -Xlinker --print-memory-usage -Xlinker --orphan-handling=warn -Wl,-Map,"gs32f00137_epwm_adc_cmpss.map" --specs=nano.specs --specs=nosys.specs -lc -lgcc -o "gs32f00137_epwm_adc_cmpss.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

gs32f00137_epwm_adc_cmpss.bin: gs32f00137_epwm_adc_cmpss.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GS RISC-V Create Flash Image'
	gs-riscv-elf-objcopy -O binary "gs32f00137_epwm_adc_cmpss.elf"  "gs32f00137_epwm_adc_cmpss.bin"
	@echo 'Finished building: $@'
	@echo ' '

gs32f00137_epwm_adc_cmpss.lst: gs32f00137_epwm_adc_cmpss.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GS RISC-V Create Listing'
	gs-riscv-elf-objdump --source --all-headers --demangle --line-numbers --wide "gs32f00137_epwm_adc_cmpss.elf" > "gs32f00137_epwm_adc_cmpss.lst"
	@echo 'Finished building: $@'
	@echo ' '

gs32f00137_epwm_adc_cmpss.siz: gs32f00137_epwm_adc_cmpss.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GS RISC-V Print Size'
	gs-riscv-elf-size --format=berkeley "gs32f00137_epwm_adc_cmpss.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SX_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) gs32f00137_epwm_adc_cmpss.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
