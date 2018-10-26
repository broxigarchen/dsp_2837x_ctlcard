################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
inc/bsp/utils/dev_timer.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_test/inc/bsp/utils/dev_timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_test/Prj_cpu1" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_test/inc" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_test/inc/core1" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_test/src/bsp/drv" --advice:performance=all --define=CPU1 -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="inc/bsp/utils/dev_timer.d_raw" --obj_directory="inc/bsp/utils" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


