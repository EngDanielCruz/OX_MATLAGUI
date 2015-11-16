################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/system_LM4F.obj: ../src/system_LM4F.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=5 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Dany/Documents/ti/cmsisproj/inc" --include_path="C:/Users/Dany/workspace_v6_1/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" -g --gcc --define="ccs" --define=PART_LM4F120H5QR --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="src/system_LM4F.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


