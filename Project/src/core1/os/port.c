/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    portASM.asm
 * @AUTHOR  Brox Chen
 * @DATE    Aug 8, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * port of FreeRTOS
 * CPU:      TI DSP F2837xD, Small Endian
 * Compiler: CCSV7
##################################################################*/
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/
/*                  Port Config                              */
#define   __TI_VFP_SUPPORT__                    1

/*-----------------------------------------------------------*/

/* Count of the critical section nesting depth. */
uint32_t ulCriticalNesting = 9999;

/*-----------------------------------------------------------*/

/* Count of the secure section nesting depth. */
uint32_t ulSecureNesting = 9999;
/*-----------------------------------------------------------*/

/* Registers required to configure the RTI. */
#define portRTI_GCTRL_REG  		( * ( ( volatile uint32_t * ) 0xFFFFFC00 ) )
#define portRTI_TBCTRL_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC04 ) )
#define portRTI_COMPCTRL_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC0C ) )
#define portRTI_CNT0_FRC0_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC10 ) )
#define portRTI_CNT0_UC0_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC14 ) )
#define portRTI_CNT0_CPUC0_REG  ( * ( ( volatile uint32_t * ) 0xFFFFFC18 ) )
#define portRTI_CNT0_COMP0_REG  ( * ( ( volatile uint32_t * ) 0xFFFFFC50 ) )
#define portRTI_CNT0_UDCP0_REG  ( * ( ( volatile uint32_t * ) 0xFFFFFC54 ) )
#define portRTI_SETINTENA_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC80 ) )
#define portRTI_CLEARINTENA_REG ( * ( ( volatile uint32_t * ) 0xFFFFFC84 ) )
#define portRTI_INTFLAG_REG  	( * ( ( volatile uint32_t * ) 0xFFFFFC88 ) )

#define portRTI_PIE_REG         ( * ( ( volatile uint32_t * ) 0xFFFFFC88 ) )
/* Constants required to set up the initial stack of each task. */
#define portINITIAL_SPSR	   	( ( StackType_t ) 0x1F )
#define portINITIAL_FPSCR	  	( ( StackType_t ) 0x00 )
#define portINSTRUCTION_SIZE   	( ( StackType_t ) 0x04 )
#define portTHUMB_MODE_BIT		( ( StackType_t ) 0x20 )

/* The number of words on the stack frame between the saved Top Of Stack and
R0 (in which the parameters are passed. */
#define portSPACE_BETWEEN_TOS_AND_PARAMETERS	( 15 )

/*-----------------------------------------------------------*/

/* vPortStartFirstSTask() is defined in portASM.asm */
extern void vPortStartFirstTask(void);

/*-----------------------------------------------------------*/

/* Saved as part of the task context.  Set to pdFALSE if the task does not
require an FPU context. */
StackType_t ulTaskHasFPUContext = 0;

/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	pxTopOfStack++;                              /* SP auto plus 1 */
	/* Automatic context save, load the registers which are automatically
	   saved in interrupt context switch. These registers will be auto
	   restored by SWI return   */
	*pxTopOfStack++ = ( StackType_t ) 0x0;         /* STO */
	*pxTopOfStack++ = ( StackType_t ) 0x0;         /* T */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* AL */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* AH */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* PL */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* AR0 */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* AR1 */
    *pxTopOfStack++ = ( StackType_t ) 0xA;         /* ST1, AMODE = 0, DBGM & VMAP & INTM */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* DP */
    *pxTopOfStack++ = ( StackType_t ) 0xFFFF;      /* IER, enable all interrupt */
    *pxTopOfStack++ = ( StackType_t ) 0x0;         /* DBGSTAT */
	*pxTopOfStack++ = ( StackType_t ) ((unsigned long)pxCode & 0xff);            /* Return PC Low */
	*pxTopOfStack++ = ( StackType_t ) (((unsigned long)pxCode>>16) & 0xff);      /* Return PC High */

	pxTopOfStack += ((unsigned int)pxTopOfStack & 0x1);        /* ASP ,SP aligned to even */

	/* Setup the stack of the task.  This part of stack is set exactly as
	    expected by the portRESTORE_CONTEXT() macro. */
	#if configCHECK_FOR_STACK_OVERFLOW > 0
	{
	    *pxTopOfStack++ = ( StackType_t ) 0x0101; /* AR0H */
	    *pxTopOfStack++ = ( StackType_t ) 0x0202; /* AR1H */
		*pxTopOfStack++ = ( StackType_t ) 0x0303; /* AR2L */
		*pxTopOfStack++ = ( StackType_t ) 0x0404; /* AR2H */
		*pxTopOfStack++ = ( StackType_t ) 0x0505; /* AR3L */
        *pxTopOfStack++ = ( StackType_t ) 0x0606; /* AR3H */
        *pxTopOfStack++ = ( StackType_t ) 0x0707; /* AR4L */
        *pxTopOfStack++ = ( StackType_t ) 0x0808; /* AR4H */
        *pxTopOfStack++ = ( StackType_t ) 0x0909; /* AR5L */
        *pxTopOfStack++ = ( StackType_t ) 0x0a0a; /* AR5H */
        *pxTopOfStack++ = ( StackType_t ) 0x0b0b; /* AR6L */
        *pxTopOfStack++ = ( StackType_t ) 0x0c0c; /* AR6H */
        *pxTopOfStack++ = ( StackType_t ) 0x0d0d; /* AR7L */
        *pxTopOfStack++ = ( StackType_t ) 0x0f0f; /* AR7H */
        *pxTopOfStack++ = ( StackType_t ) 0x0101; /* TL   */
        *pxTopOfStack++ = ( StackType_t ) 0x0202; /* T   */
	}
	#else
	{
		pxTopOfStack += portSPACE_BETWEEN_TOS_AND_PARAMETERS;
	}
	#endif

	#ifdef __TI_VFP_SUPPORT__
	{
	    /* if need FPU context, load FPU register */
	    if(ulTaskHasFPUContext == pdTRUE)
	    {
	        *pxTopOfStack++ = ( StackType_t ) 0x0101; /* R0H */
            *pxTopOfStack++ = ( StackType_t ) 0x0202; /* R1H */
            *pxTopOfStack++ = ( StackType_t ) 0x0303; /* R2L */
            *pxTopOfStack++ = ( StackType_t ) 0x0404; /* R2H */
            *pxTopOfStack++ = ( StackType_t ) 0x0505; /* R3L */
            *pxTopOfStack++ = ( StackType_t ) 0x0606; /* R3H */
            *pxTopOfStack++ = ( StackType_t ) 0x0707; /* R4L */
            *pxTopOfStack++ = ( StackType_t ) 0x0808; /* R4H */
            *pxTopOfStack++ = ( StackType_t ) 0x0909; /* R5L */
            *pxTopOfStack++ = ( StackType_t ) 0x0a0a; /* R5H */
            *pxTopOfStack++ = ( StackType_t ) 0x0b0b; /* R6L */
            *pxTopOfStack++ = ( StackType_t ) 0x0c0c; /* R6H */
            *pxTopOfStack++ = ( StackType_t ) 0x0d0d; /* R7L */
            *pxTopOfStack++ = ( StackType_t ) 0x0200; /* STF_L, RND32 1*/
            *pxTopOfStack++ = ( StackType_t ) 0x0000; /* STF_H, SHDWS 0 */
            *pxTopOfStack++ = ( StackType_t ) 0x0000; /* RB_H */
            *pxTopOfStack++ = ( StackType_t ) 0x0000; /* RB_L */
	    }
		/* The last thing on the stack is the tasks ulUsingFPU value, which by
		default is set to indicate that the stack frame does not include FPU
		registers. */
		*pxTopOfStack++ = ulTaskHasFPUContext;
	}
	#endif

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

static void prvSetupTimerInterrupt(void)
{
    /*  configure real timer, using CPU Timer 2    */


    /*  configure real timer PIE, using CPU Timer 2   */
    portSECURE_UNLOCK();
#if configUSE_PREEMPTION == 0
    portRTI_PIE_REG = vPortNonPreemptiveTick;
#else
    extern void vPortPreemptiveTick( void );
    portRTI_PIE_REG = vPortPreemptiveTick;
#endif
    portSECURE_LOCK();
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler(void)
{
	/* Start the timer that generates the tick ISR. */
	prvSetupTimerInterrupt();

	/* Reset the critical section nesting count read to execute the first task. */
	ulCriticalNesting = 0;

	/* Start the first task.  */
	vPortStartFirstTask();

	/* Should not get here! */
	return pdFAIL;
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
void vPortEndScheduler(void)
{
	/* Not implemented in ports where there is nothing to return to.
	Artificially force an assert. */
	configASSERT( ulCriticalNesting == 1000UL );
}

/*-----------------------------------------------------------*/
#if configUSE_PREEMPTION == 0

	/* The cooperative scheduler requires a normal IRQ service routine to
	 * simply increment the system tick. */
	__interrupt void vPortNonPreemptiveTick( void )
	{
		/* clear clock interrupt flag */
		portRTI_INTFLAG_REG = 0x00000001;

		/* Increment the tick count - this may make a delaying task ready
		to run - but a context switch is not performed. */
		xTaskIncrementTick();
	}
#else
	/*
     **************************************************************************
     * The preemptive scheduler ISR is written in assembler and can be found
     * in the portASM.asm file. This will only get used if portUSE_PREEMPTION
     * is set to 1 in portmacro.h
     **************************************************************************
     */
	void vPortPreemptiveTick(void);
#endif
/*-----------------------------------------------------------*/


/*
 * Disable interrupts, and keep a count of the nesting depth.
 */
void vPortEnterCritical( void )
{
	/* Disable interrupts as per portDISABLE_INTERRUPTS(); */
	portDISABLE_INTERRUPTS();

	/* Now interrupts are disabled ulCriticalNesting can be accessed
	directly.  Increment ulCriticalNesting to keep a count of how many times
	portENTER_CRITICAL() has been called. */
	ulCriticalNesting++;
}
/*-----------------------------------------------------------*/

/*
 * Decrement the critical nesting count, and if it has reached zero, re-enable
 * interrupts.
 */
void vPortExitCritical( void )
{
	if( ulCriticalNesting > 0 )
	{
		/* Decrement the nesting count as we are leaving a critical section. */
		ulCriticalNesting--;

		/* If the nesting level has reached zero then interrupts should be
		re-enabled. */
		if( ulCriticalNesting == 0 )
		{
			/* Enable interrupts as per portENABLE_INTERRUPTS(). */
			portENABLE_INTERRUPTS();
		}
	}
}
/*
 * F29379D do not support nest interrupts. Do nothing
 */
void vPortEnterCriticalFromISR(BaseType_t isr_mask)
{

}
/*-----------------------------------------------------------*/

#if __TI_VFP_SUPPORT__

	void vPortTaskUsesFPU( uint16_t on_off )
	{
	extern void vPortInitialiseFPSCR( void );

		/* A task is registering the fact that it needs an FPU context.  Set the
		FPU flag (saved as part of the task context. */
	if(on_off)
		ulTaskHasFPUContext = (StackType_t)pdTRUE;
	else
	    ulTaskHasFPUContext = (StackType_t)pdFALSE;
	}

#endif /* __TI_VFP_SUPPORT__ */

/*-----------------------------------------------------------*/

void  vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
    portDISABLE_INTERRUPTS();
    while(1);
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{

}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{

}
#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{

}
#endif


