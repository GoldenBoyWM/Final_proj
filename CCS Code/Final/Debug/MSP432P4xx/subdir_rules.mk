################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MSP432P4xx/%.obj: ../MSP432P4xx/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/tirex-product-tree/msp432_driverlib_3_21_00_05/driverlib/MSP432P4xx" --include_path="D:/CCS/ccs/ccs_base/arm/include" --include_path="D:/CCS/ccs/ccs_base/arm/include/CMSIS" --include_path="D:/CCS2/Final" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="MSP432P4xx/$(basename $(<F)).d_raw" --obj_directory="MSP432P4xx" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


