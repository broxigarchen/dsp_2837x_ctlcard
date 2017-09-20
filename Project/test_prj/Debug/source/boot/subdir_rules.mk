################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
source/boot/init.obj: ../source/boot/init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Software/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="/home/cg/Work/Mcmaster/DSP/2837x Control Card/test_prj" --include_path="/home/cg/Software/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --include_path="/home/cg/Work/Mcmaster/DSP/2837x Control Card/test_prj/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="source/boot/init.d_raw" --obj_directory="source/boot" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


