################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/system_LM4F.obj: ../src/system_LM4F.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Dany/Documents/ti/cmsisproj/inc" --include_path="C:/Users/Dany/Desktop/texas instruments/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" -g --gcc --define="ccs" --define=PART_LM4F120H5QR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/system_LM4F.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


