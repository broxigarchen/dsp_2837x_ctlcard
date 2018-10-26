################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/lib/tshell/tshell.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/lib/tshell/tshell.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/Prj_cpu1" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc/core1" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/bsp/drv" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc/core1/os" --advice:performance=all --define=CPU1 -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/lib/tshell/tshell.d_raw" --obj_directory="src/lib/tshell" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

src/lib/tshell/tshport.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/lib/tshell/tshport.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/Prj_cpu1" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc/core1" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/bsp/drv" --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/inc/core1/os" --advice:performance=all --define=CPU1 -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/lib/tshell/tshport.d_raw" --obj_directory="src/lib/tshell" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


