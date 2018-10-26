;*##################################################################
;* MCMASTER UNIVERSITY P.E.R.L. LAB.
;* This file is included in Project of High Power Electronic Controller
;*
;* @FILE    portASM.asm
;* @AUTHOR  Brox Chen
;* @DATE    Aug 8, 2018
;* @LICENSE Please see file LICENSE
;* @DESC.
;* port ASM of FreeRTOS
;* CPU: 	 TI DSP F2837xD
;* Compiler: CCSV7
;##################################################################*

		.ref _vTaskSwitchContext
		.ref _xTaskIncrementTick
		.ref _ulTaskHasFPUContext
		.ref _pxCurrentTCB

		.def _vPortStartFirstTask
		.def _vPortSWIUser1
		.def _vPortPreemptiveTick


__TI_VFP_SUPPORT__  .set    1
pxCurrentTCBConst	.word	_pxCurrentTCB
ulFPUContextConst 	.word   _ulTaskHasFPUContext
pctemp              .word   0
;/*-----------------------------------------------------------*/
		.sect "freertos_text_portASM"

; Write SP pointer to pxCurrentTCB
portWRITE_SP .macro
		MOV      AL,SP
		MOVL     XAR0, #pxCurrentTCBConst
		MOV      *XAR0, AL
		.endm

; Read SP pointer pxCurrentTCB -> XAR0
portREAD_SP .macro
		MOVL     XAR0, #pxCurrentTCBConst
        MOV      AL, *XAR0
        MOV      SP, AL
        .endm

;/*-----------------------------------------------------------*/
; Save Task Context in SWI
; SWI has auto save some registers and used ASP.
; Manually save others here
portSAVE_CONTEXT_SWI .macro
		;save remaining registers
		PUSH     AR1H:AR0H
		PUSH     XAR2
		PUSH     XAR3
		PUSH     XAR4
		PUSH     XAR5
		PUSH     XAR6
		PUSH     XAR7
		PUSH     XT
		PUSH     RPC

		.if (__TI_VFP_SUPPORT__)
		;if the task has FPU context, save it
		MOVL     XAR0, #ulFPUContextConst
		MOVL     ACC, *XAR0
		CMPB     AL, #0
		BF       0x12, EQ
		MOV32	 *SP++,R0H
		MOV32	 *SP++,R1H
		MOV32	 *SP++,R2H
		MOV32	 *SP++,R3H
		MOV32	 *SP++,R4H
		MOV32	 *SP++,R5H
		MOV32	 *SP++,R6H
		MOV32	 *SP++,R7H
		MOV32	 *SP++,STF
		PUSH     RB
		.endif

		;Save new stack top pointer
		MOV      AL,SP
		MOVL     XAR0, #pxCurrentTCBConst
		MOV      *XAR0, AL
		.endm
;/*-----------------------------------------------------------*/
;
; Restore Task Context in SWI
;
portRESTORE_CONTEXT_SWI .macro
		;read Stack point position
		MOVL     XAR0, #pxCurrentTCBConst
        MOV      AL, *XAR0
        MOV      SP, AL

		.if (__TI_VFP_SUPPORT__)
		;restore ulFPUContextConst
		MOVL     XAR1, #ulFPUContextConst
		MOVL     ACC, *--SP
		MOVL     *XAR1,ACC
		;if has FPU context, restore it
		CMPB     AL, #0
		B        0x16, EQ
		POP	     RB
		MOV32	 STF,*--SP
		MOV32	 R7H,*--SP
		MOV32	 R6H,*--SP
		MOV32	 R5H,*--SP
		MOV32	 R4H,*--SP
		MOV32	 R3H,*--SP
		MOV32	 R2H,*--SP
		MOV32	 R1H,*--SP
		MOV32	 R0H,*--SP
		.endif

		;restore cpu context
		POP      RPC
		POP      XT
		POP      XAR7
		POP      XAR6
		POP      XAR5
		POP      XAR4
		POP      XAR3
		POP      XAR2
		POP      AR1H:AR0H
		.endm

;/*-----------------------------------------------------------*/
; Save Task Context
; manually save the register which are auto saved by SWI, then reuse
; the SWI save/restore function
portSAVE_CONTEXT .macro
/*
		ASP
		PUSH	T:ST0
		PUSH    ACC
		PUSH    P
		PUSH    AR1:AR0
		PUSH    DP:ST1
		MOV     AL, IER
		PUSH    AL
		PUSH	DBGIER
		;assuming jumped from RPC, save RPC as PC
		MOV     ACC, RPC
		ADD     pctemp, #2
		PUSH    pctemp
		portSAVE_CONTEXT_SWI    ;use the SWI save part
		LCR _vTaskSwitchContext
		portRESTORE_CONTEXT_SWI ;use the SWI restore part
		NASP
*/
		.endm
;/*-----------------------------------------------------------*/
; Restore Task Context
portRESTORE_CONTEXT .macro
/*
		portRESTORE_CONTEXT_SWI
		NASP
*/
		.endm
;/*-----------------------------------------------------------*/
; Start the first task by restoring its context.

_vPortStartFirstTask:
		portRESTORE_CONTEXT_SWI
		; unallign sp
		NASP
		; SWI will auto restore some registers.
		; Continue user programs
		IRET
	    ESTOP0   ;should not run to here

;/*-----------------------------------------------------------*/
; Port Task Yield in SWI User1
_vPortSWIUser1:
		; allign sp
		ASP
		; Save the context of the current task.
		portSAVE_CONTEXT_SWI
		; Select the next task to execute.
		LCR _vTaskSwitchContext
		portRESTORE_CONTEXT_SWI
		NASP
		IRET
	    ESTOP0   ;should not run to here
;/*-----------------------------------------------------------*/
; Port Task Yield In SWI

_vPortYieldInSWI:
		;start in middle of SWI
		portSAVE_CONTEXT_SWI
		; Select the next task to execute.
		LCR _vTaskSwitchContext
		portRESTORE_CONTEXT_SWI
		NASP
		; SWI will auto restore some registers.
		; Continue user programs
		IRET
	    ESTOP0   ;should not run to here
;-------------------------------------------------------------------------------
; Preemptive

_vPortPreemptiveTick:
		; allign sp
		ASP
		; Save the context of the current task.
		portSAVE_CONTEXT_SWI
		; Increment the tick count, making any adjustments to the blocked lists
		LCR _xTaskIncrementTick
		; Select the next task to execute.
		CMP AL,#0
		B   0x4, EQ
		LCR _vTaskSwitchContext
		; Restore the context of the task selected to execute.
		portRESTORE_CONTEXT_SWI
		; unallign sp
		NASP
		; SWI will auto restore some registers.
		; Continue user programs
		IRET
        ESTOP0   ;should not run to here

;-------------------------------------------------------------------------------

