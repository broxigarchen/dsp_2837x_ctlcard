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
 * CPU:      TI DSP F2837xD
 * Compiler: CCSV7
##################################################################*/

#ifndef __PORTMACRO_H__
#define __PORTMACRO_H__

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  uint16_t
#define portBASE_TYPE   int

typedef portSTACK_TYPE  StackType_t;
typedef int BaseType_t;
typedef unsigned int UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
    typedef uint16_t TickType_t;
    #define portMAX_DELAY (TickType_t) 0xFFFF
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY (TickType_t) 0xFFFFFFFFF

	/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
	not need to be guarded with a critical section. */
	#define portTICK_TYPE_IS_ATOMIC 1
#endif


/* Architecture specifics. */
#define portSTACK_GROWTH      (1)
#define portTICK_PERIOD_MS    ((TickType_t) 1000 / configTICK_RATE_HZ)
#define portBYTE_ALIGNMENT    16

/* Critical section handling. */
extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);
#define portENTER_CRITICAL()		        vPortEnterCritical()
#define portEXIT_CRITICAL()			        vPortExitCritical()
/* portSET_INTERRUPT_MASK_FROM_ISR
 * F2937xD do not support nest interrupts. Do nothing */
#define portSET_INTERRUPT_MASK_FROM_ISR()    0
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) 0
#define portDISABLE_INTERRUPTS()	        asm( " DINT " )
#define portENABLE_INTERRUPTS()		        asm( " EINT " )
#define portSECURE_UNLOCK()                 asm( " EALLOW ")
#define portSECURE_LOCK()                   asm( " EDIS ")


/* Scheduler utilities. */
extern void vPortYield( void );
/*  Trigger USER1 SWI to yield   */
#define portYIELD()             	        asm( " TRAP #20 ")
#define portYIELD_FROM_ISR( x )		        vPortYieldInSWI()

/* Architecture specific optimisations. */
#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1

	/* Check the configuration. */
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
	#endif

	/* Store/clear the ready priorities in a bit map. */
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

	/*-----------------------------------------------------------*/

	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31 - __clz( ( uxReadyPriorities ) ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION(vFunction, pvParameters)       void vFunction(void *pvParameters)
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)

#endif /* __PORTMACRO_H__ */

