################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
APP/iic/%.obj: ../APP/iic/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate" --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate/APP/iic" --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate/APP/OLED" --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate/APP/led" --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate/APP/key" --include_path="D:/CCS/dsp28335/workspace/OLED_ProjectTemplate/DSP2833x_Libraries/include" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="APP/iic/$(basename $(<F)).d_raw" --obj_directory="APP/iic" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


