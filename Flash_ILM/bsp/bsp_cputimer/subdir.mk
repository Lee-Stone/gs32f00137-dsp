################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/bsp_cputimer/bsp_cputimer.c 

OBJS += \
./bsp/bsp_cputimer/bsp_cputimer.o 

C_DEPS += \
./bsp/bsp_cputimer/bsp_cputimer.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/bsp_cputimer/%.o: ../bsp/bsp_cputimer/%.c bsp/bsp_cputimer/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GS RISC-V C Compiler'
	gs-riscv-elf-gcc -march=rv32imafc_xgsdida -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore --gsdi_ext1p2h -mgs_aux -O1 -fmessage-length=0 -ffunction-sections -fdata-sections -fsingle-precision-constant -Wfatal-errors -funsigned-char --opt_level=off -O1 -g -ggdb -DFLASH_TARGET -DGS32_PART_NUM=0x0C137 -DGS32_PIN_COUNT=80 -DGS32F00xx=0x3000 -DUSING_VECTOR_INTERRUPT=1 -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/dsp" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/sysctl" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/init" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/dsp/NMSIS_core" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/driverlib/device_gs32f00xx" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/common" -I"C:/Users/Xinglei Li/Desktop/Lee/Workspace2/gs32f00137_epwm_adc_cmpss/src" -I"C:/Users/Xinglei Li/Desktop/Lee/Workspace2/gs32f00137_epwm_adc_cmpss/inc" -I"C:\Users\Xinglei Li\Desktop\Lee\GS32_Studio_v1.2.4.2\\packages\GS32-DSPWare\gs32_driver/mathlib" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_can" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_epwm" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_i2c" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_sci" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_gpio" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\test" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_eqep" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_adc" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_cputimer" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_cmpss" -I"C:\Users\Xinglei Li\Desktop\Lee\Workspace2\gs32f00137_epwm_adc_cmpss\bsp\bsp_hrpwm" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


