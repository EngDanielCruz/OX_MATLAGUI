################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Butterword_filter.obj" \
"./FIFO.obj" \
"./FIR_filter.obj" \
"./Filters.obj" \
"./I2C.obj" \
"./MAX30100.obj" \
"./MA_filter.obj" \
"./NRF24.obj" \
"./Periferal_Init.obj" \
"./Process_bio.obj" \
"./SPI.obj" \
"./UART.obj" \
"./main.obj" \
"./startup_ccs.obj" \
"./src/system_LM4F.obj" \
"../lm4f120h5qr.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
GIT_UART_OXIMETER_filter.out \

EXE_OUTPUTS__QUOTED += \
"GIT_UART_OXIMETER_filter.out" \

BIN_OUTPUTS += \
GIT_UART_OXIMETER_filter.hex \

BIN_OUTPUTS__QUOTED += \
"GIT_UART_OXIMETER_filter.hex" \


# All Target
all: GIT_UART_OXIMETER_filter.out

# Tool invocations
GIT_UART_OXIMETER_filter.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --opt_for_speed=5 -g --gcc --define="ccs" --define=PART_LM4F120H5QR --diag_warning=225 --display_error_number --diag_wrap=off -z -m"GIT_UART_OXIMETER_filter.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/lib" -i"C:/ti/ccsv6/ccs_base/arm/include/CMSIS" -i"C:/Users/Dany/Documents/ti/cmsisproj/inc" -i"C:/Users/Dany/Documents/ti/cmsisproj/src" -i"C:/Users/Dany/Desktop/texas instruments/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --reread_libs --warn_sections --diag_wrap=off --display_error_number --xml_link_info="GIT_UART_OXIMETER_filter_linkInfo.xml" --rom_model -o "GIT_UART_OXIMETER_filter.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

GIT_UART_OXIMETER_filter.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armhex"  -o "GIT_UART_OXIMETER_filter.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "Butterword_filter.pp" "FIFO.pp" "FIR_filter.pp" "Filters.pp" "I2C.pp" "MAX30100.pp" "MA_filter.pp" "NRF24.pp" "Periferal_Init.pp" "Process_bio.pp" "SPI.pp" "UART.pp" "main.pp" "startup_ccs.pp" "src\system_LM4F.pp" 
	-$(RM) "Butterword_filter.obj" "FIFO.obj" "FIR_filter.obj" "Filters.obj" "I2C.obj" "MAX30100.obj" "MA_filter.obj" "NRF24.obj" "Periferal_Init.obj" "Process_bio.obj" "SPI.obj" "UART.obj" "main.obj" "startup_ccs.obj" "src\system_LM4F.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

