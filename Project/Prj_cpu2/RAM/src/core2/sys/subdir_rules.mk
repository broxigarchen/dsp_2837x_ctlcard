################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/core2/sys/sys_fault.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/core2/sys/sys_fault.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/Prj_cpu2" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/core2/sys/sys_fault.d_raw" --obj_directory="src/core2/sys" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

src/core2/sys/sys_schd.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/core2/sys/sys_schd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/Prj_cpu2" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/core2/sys/sys_schd.d_raw" --obj_directory="src/core2/sys" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

src/core2/sys/sys_tick.obj: /home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/src/core2/sys/sys_tick.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --include_path="/home/cg/Work/McMaster/Prj_TMS320F28379D/Prj_Controller/Prj_cpu2" --include_path="/home/cg/Apps/CCSV7/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/core2/sys/sys_tick.d_raw" --obj_directory="src/core2/sys" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


