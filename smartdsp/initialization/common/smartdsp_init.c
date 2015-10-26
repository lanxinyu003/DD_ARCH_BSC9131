/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/11 13:20:30 $
 $Id: smartdsp_init.c,v 1.80 2014/03/11 13:20:30 b41932 Exp $
 $Source: /cvsdata/SmartDSP/initialization/common/smartdsp_init.c,v $
 $Revision: 1.80 $
******************************************************************************/

/******************************************************************************
 
 @File          smartdsp_init.c

 @Description   SmartDSP OS Initialization file.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os.h"

#include "os_init.h"

#include "os_config.h"
#include "smartdsp_init.h"

#ifndef OS_L1_DEFENSE
#define OS_L1_DEFENSE OFF
#endif

#if (OS_L1_DEFENSE == ON)
#include "os_l1_defense.h"
#endif

#ifndef OS_INTERCORE_MESSAGES_MEM_HEAP
#define OS_INTERCORE_MESSAGES_MEM_HEAP OS_MEM_SHARED
#endif // OS_INTERCORE_MESSAGES_MEM_HEAP


#ifndef OS_TOTAL_NUM_OF_EVENT_QUEUES
#define OS_TOTAL_NUM_OF_EVENT_QUEUES 0
#endif // OS_TOTAL_NUM_OF_EVENT_QUEUES

#ifndef OS_TOTAL_NUM_OF_EVENT_SEMAPHORES
#define OS_TOTAL_NUM_OF_EVENT_SEMAPHORES 0
#endif // OS_TOTAL_NUM_OF_EVENT_SEMAPHORES

#ifndef OS_TOTAL_NUM_OF_MESSAGE_QUEUES
#define OS_TOTAL_NUM_OF_MESSAGE_QUEUES 0
#endif // OS_TOTAL_NUM_OF_MESSAGE_QUEUES

#ifndef OS_TOTAL_NUM_OF_TASKS
#define OS_TOTAL_NUM_OF_TASKS 1
#endif // OS_TOTAL_NUM_OF_TASKS


//extern uint32_t interr_vector_file[];

#ifndef OS_LOCAL_HEAP_MNGMNT_SIZE
#define OS_LOCAL_HEAP_MNGMNT_SIZE       OS_LOCAL_HEAP_MNGMNT_DEFAULT_SIZE
#endif

#ifndef OS_SHARED_HEAP_MNGMNT_SIZE
#define OS_SHARED_HEAP_MNGMNT_SIZE      OS_SHARED_HEAP_MNGMNT_DEFAULT_SIZE
#endif

extern uint8_t g_local_mngmnt_space[];
#if OS_MULTICORE == 1
#if OS_SHARED_HEAP_MNGMNT_SIZE > 0
extern uint8_t g_shared_mngmnt_space[];
#endif
#endif //OS_MULTICORE


extern os_mem_heap_t g_mem_heap_local[];
extern int g_num_of_local_heaps;


#if (OS_MULTICORE == ON)
extern os_mem_heap_t g_mem_heap_shared[];
extern int g_num_of_shared_heaps;
#endif // OS_MULTICORE

#ifndef OS_SYSTEM_CLOCK
#define OS_SYSTEM_CLOCK     0
#endif

#ifndef OS_WATCHDOGS
#define OS_WATCHDOGS        0
#endif

extern uint32_t StackStart[];


#ifndef USER_KERNEL_AWARENESS_STACK
#define USER_KERNEL_AWARENESS_STACK OFF
#endif
#if (USER_KERNEL_AWARENESS_STACK == ON)
volatile uint32_t   _g_os_log_by_user = TRUE;
#else
volatile uint32_t   _g_os_log_by_user = FALSE;
#endif

#if (OS_MULTIMODE == 1)
#if (OS_MULTICORE == OFF)
#error "Set OS_MULTICORE = ON, OS_MULTIMODE is not relevant for single core \n"
#endif
/*
 * Function which activates osMemLocalHeapSet() API in order to set local heaps of different size.
 */
extern os_status appMemHeapsSet();
#endif

/*****************************************************************************/
os_status osInitialize()
{
    os_status status;
    uint32_t  exception_stack_pointer = (uint32_t)&StackStart;
 
    // Verify that the OS and application are both compiled with OS_MULTICORE set the same
    OS_ASSERT_COND(osMulticoreSupport() == OS_MULTICORE);

    /* Verify exception stack alignment. */
    OS_ASSERT_COND(IS_ALIGNED(exception_stack_pointer, ALIGNED_8_BYTES));

/* KERNEL */

    /* DO NOT MOVE! */
#if (USER_KERNEL_AWARENESS_STACK == ON)
#if (OS_MULTICORE == ON)
    status = osLogInitialize((uint8_t *)KERNEL_AWARENESS_ADDR, KERNEL_AWARENESS_STACK_SIZE, OS_NUM_OF_CORES);
#else
    status = osLogInitialize((uint8_t *)KERNEL_AWARENESS_ADDR, KERNEL_AWARENESS_STACK_SIZE);
#endif
    if (status != OS_SUCCESS)
        return status;
#endif // (USER_KERNEL_AWARENESS_STACK == ON)
    status = sysInit();
    if (status != OS_SUCCESS)
        return status;

#if (OS_MULTICORE == ON)
    status = osInitializeKernel((uint32_t)OS_SYSTEM_CLOCK,
                            OS_NUM_OF_CORES,
                            OS_MAX_NUM_OF_CORES,
                            OS_MASTER_CORE);
#else
    status = osInitializeKernel((uint32_t)OS_SYSTEM_CLOCK);
#endif // OS_MULTICORE
    if (status != OS_SUCCESS)
        return status;


/* MEMORY MANAGEMENT */
status = osMemInitialize(g_mem_heap_local, g_num_of_local_heaps
#if (OS_MULTICORE == ON)
        ,g_mem_heap_shared
        ,g_num_of_shared_heaps
#endif
    	);
if (status != OS_SUCCESS)
    return status;

#if (OS_MULTIMODE == 1)
status = appMemHeapsSet();
if (status != OS_SUCCESS)
    return status;
#endif

status = osMemLocalInitialize(OS_TOTAL_NUM_OF_MEM_PARTS + OS_TOTAL_NUM_OF_FRAME_POOLS,
                             g_local_mngmnt_space,
                             OS_LOCAL_HEAP_MNGMNT_SIZE);
    if (status != OS_SUCCESS)
        return status;
 

/* ARCHITECTURE BASICS */
 
    status = osArchInitialize();
    if (status != OS_SUCCESS)
        return status;


/* MEMORY MANAGEMENT */
#if (OS_MULTICORE == ON)
    status = osMemSharedInitialize(
#if OS_SHARED_HEAP_MNGMNT_SIZE > 0
                            g_shared_mngmnt_space,
#else
                            (uint8_t*)NULL,
#endif
                            OS_SHARED_HEAP_MNGMNT_SIZE);
    if (status != OS_SUCCESS)
        return status;
#endif

 
/* HARDWARE INTERRUPTS */

#ifndef NO_HWI_INTERRUPTS
 
    //status = osHwiInitialize(&interr_vector_file);
    //if (status != OS_SUCCESS)
        //return status;
#endif

/* HARDWARE TIMERS */

#if (OS_HW_TIMERS == ON)
    status = osHwTimerInitialize();
    if (status != OS_SUCCESS)
        return status;
#endif // OS_HW_TIMERS


/* TICK */

#if (OS_TICK == ON)


#ifndef OS_TICK_PARAMETER
#error "OS_TICK needs OS_TICK_PARAMETER"
#endif
#ifndef OS_SYSTEM_CLOCK
#error "OS_TICK needs defined OS_SYSTEM_CLOCK"
#endif
    status = osTickInitialize(OS_TICK_PARAMETER);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TICK


#ifndef VERBOSE

/* SOFTWARE TIMERS */
#if (OS_MULTIMODE == 1)
    if (OS_TOTAL_NUM_OF_SW_TIMERS > 0)
    {
        if (OS_TICK == OFF) return OS_FAIL; // OS_SW_TIMERS needs OS_TICK
        status = osTimerInitialize(OS_TOTAL_NUM_OF_SW_TIMERS);
        if (status != OS_SUCCESS)
            return status;
    }
 
#else // (OS_MULTIMODE == 0)
#if (OS_TOTAL_NUM_OF_SW_TIMERS > 0)
#if (OS_TICK == OFF)
#error "OS_SW_TIMERS needs OS_TICK"
#endif //(OS_TICK == OFF)
    status = osTimerInitialize(OS_TOTAL_NUM_OF_SW_TIMERS);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_SW_TIMERS
#endif // OS_MULTIMODE

/* SOFTWARE INTERRUPTS */
#if (OS_MULTIMODE == 1)
    if ((OS_TOTAL_NUM_OF_SWI > 0) || (OS_TICK == ON))
    {
        /* In case the user has requested SWIs, or Tick is on, */
        /* we reserve enough resources for the number of SWIs */
        /* requested, plus 1 to suppurt the Tick. */
        status = osSwiInitialize(OS_TOTAL_NUM_OF_SWI + 1);
        if (status != OS_SUCCESS)
            return status;
    }
#else // (OS_MULTIMODE == 0)
#if ((OS_TOTAL_NUM_OF_SWI > 0) || (OS_TICK == ON))
/* In case the user has requested SWIs, or Tick is on, */
/* we reserve enough resources for the number of SWIs */
/* requested, plus 1 to suppurt the Tick. */
    status = osSwiInitialize(OS_TOTAL_NUM_OF_SWI + 1);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_SWI
#endif // OS_MULTIMODE


#endif // VERBOSE


/* MULTICORE SYNCHRONIZATION */

#if (OS_MULTICORE == ON)
#if (OS_MULTICORE_SYNCHRONIZATION == ON)
	status = osMultiCoreSyncInitialize();
	if (status != OS_SUCCESS)
		return status;
#endif // OS_MULTICORE_SYNCHRONIZATION
#endif // OS_MULTICORE


/* INTERCORE MESSAGES */

#if (OS_MULTICORE == ON)
#if (OS_TOTAL_NUM_OF_INTERCORE_MESSAGES > 0)
    status = osMessageInitialize(OS_TOTAL_NUM_OF_INTERCORE_MESSAGES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_INTERCORE_MESSAGES
#endif // OS_MULTICORE


/* QUEUES */
#if (OS_MULTIMODE == 1)
    if ((OS_TOTAL_NUM_OF_QUEUES > 0) || (OS_TOTAL_NUM_OF_SHARED_QUEUES > 0))
    {
        status = osQueueInitialize(OS_TOTAL_NUM_OF_QUEUES,
                OS_TOTAL_NUM_OF_SHARED_QUEUES+OS_TOTAL_NUM_OF_MESSAGE_QUEUES);
        if (status != OS_SUCCESS)
            return status;
    } // OS_TOTAL_NUM_OF_QUEUES || OS_TOTAL_NUM_OF_SHARED_QUEUES
#else // (OS_MULTIMODE == 0)

#if (OS_MULTICORE == ON)
#if ((OS_TOTAL_NUM_OF_QUEUES > 0) || (OS_TOTAL_NUM_OF_SHARED_QUEUES > 0))
    status = osQueueInitialize(OS_TOTAL_NUM_OF_QUEUES,
                               OS_TOTAL_NUM_OF_SHARED_QUEUES+OS_TOTAL_NUM_OF_MESSAGE_QUEUES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_QUEUES || OS_TOTAL_NUM_OF_SHARED_QUEUES
#else
#if (OS_TOTAL_NUM_OF_QUEUES > 0)
    status = osQueueInitialize(OS_TOTAL_NUM_OF_QUEUES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_QUEUES
#endif // OS_MULTICORE
#endif // OS_MULTIMODE
 


#if (OS_MULTICORE == ON)
#if (OS_TOTAL_NUM_OF_MESSAGE_QUEUES > 0)
    status = osMessageQueueInitialize(OS_TOTAL_NUM_OF_MESSAGE_QUEUES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_MESSAGE_QUEUES
#endif // OS_MULTICORE

/* CIO */

#if (OS_TOTAL_NUM_OF_CIO_DEVICES > 0)
    status = osCioInitialize(OS_TOTAL_NUM_OF_CIO_DEVICES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_CIO_DEVICES


/* BIO */

#if (OS_TOTAL_NUM_OF_BIO_DEVICES > 0)
    status = osFrameInitialize();
    if (status != OS_SUCCESS)
        return status;
 
    status = osBioInitialize(OS_TOTAL_NUM_OF_BIO_DEVICES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_BIO_DEVICES

/* SIO */

#if (OS_TOTAL_NUM_OF_SIO_DEVICES > 0)
    status = osSioInitialize(OS_TOTAL_NUM_OF_SIO_DEVICES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_SIO_DEVICES


#if (OS_TOTAL_NUM_OF_COP_DEVICES > 0)
    status = osCopInitialize(OS_TOTAL_NUM_OF_COP_DEVICES);
    if (status != OS_SUCCESS)
        return status;
#endif // OS_TOTAL_NUM_OF_COP_DEVICES

#if (OS_WATCHDOGS != 0)
#if (OS_WATCHDOG_SUPPORT != OS_WDT_DISABLED)
/* Watchdog Timers */
    status = osHwWatchdogsInit();
    if (status != OS_SUCCESS)
        return status;
#endif // OS_WATCHDOG_SUPPORT
#endif // OS_WATCHDOGS

/* ARCHITECTURE DEVICES */

    status = osArchDevicesInitialize();
    if (status != OS_SUCCESS)
        return status;

#if (OS_MULTICORE == ON)
#if (OS_MULTICORE_SYNCHRONIZATION == ON)
#if (OS_L1_DEFENSE == ON)
    if(osL1dGetResetMode() != OS_L1D_MODE_1)
    {
    	extern os_barrier_t shared_cores_barrier;
#endif //OS_L1_DEFENSE == ON
    	osWaitForAllCores();
#if (OS_L1_DEFENSE == ON)
    	/*return init flag to reset state as a preparation for next warm reset*/
    	shared_cores_barrier.init_flag = FALSE;
    }
#endif //OS_L1_DEFENSE == ON
#endif // OS_MULTICORE_SYNCHRONIZATION
#endif // OS_MULTICORE


/* EVENTS */
#if (OS_MULTIMODE == 1)
    if (OS_TOTAL_NUM_OF_EVENT_QUEUES + OS_TOTAL_NUM_OF_EVENT_SEMAPHORES)
    {
        status = osEventsInitialize(OS_TOTAL_NUM_OF_EVENT_QUEUES,
                OS_TOTAL_NUM_OF_EVENT_SEMAPHORES);
        if (status != OS_SUCCESS)
            return status;
    }
#else // (OS_MULTIMODE == 0)
#if (OS_TOTAL_NUM_OF_EVENT_QUEUES + OS_TOTAL_NUM_OF_EVENT_SEMAPHORES)
    status = osEventsInitialize(OS_TOTAL_NUM_OF_EVENT_QUEUES,
                                OS_TOTAL_NUM_OF_EVENT_SEMAPHORES);
    if (status != OS_SUCCESS)
        return status;
#endif
#endif // OS_MULTIMODE

/* TASKS */
#ifndef OS_TOTAL_NUM_OF_TASKS
#define OS_TOTAL_NUM_OF_TASKS 1
#endif // OS_TOTAL_NUM_OF_TASKS

    //status = osTasksInitialize(OS_TOTAL_NUM_OF_TASKS);
    //if (status != OS_SUCCESS)
        //return status;

 
#if (OS_L1_DEFENSE == ON)
    if(osL1dGetResetMode() != 0)
    {
        /* report end of osInitialize to L2 when in warm reset */
        osL1dReportStatusEndInit();
    }
#endif //OS_L1_DEFENSE
 
    return status;
}

/*****************************************************************************/

os_status osStart(os_background_task_function background_task)
{

#if (OS_TICK == ON)
#ifndef VERBOSE
    if (osTickStart(OS_TICK_PRIORITY) != OS_SUCCESS)
    {
        RETURN_ERROR(OS_FAIL);
    }
#endif // VERBOSE

#if (OS_WATCHDOGS != 0)
#if (OS_WATCHDOG_SUPPORT == OS_WDT_AUTO_HANDLING)
#ifndef OS_WATCHDOG_HANDLER
#define OS_WATCHDOG_HANDLER  NULL
#endif
#ifndef OS_WATCHDOG_BEHAVIOR
#define OS_WATCHDOG_BEHAVIOR GENERATE_RESET
#endif
    /* assumes watchdg is disabled by default after reset */
    if ((osHwWatchdogSupport((os_watchdog_handle)osGetCoreID(),
                             OS_WATCHDOG_TIMEOUT,
                             OS_WATCHDOG_BEHAVIOR,
                             OS_WATCHDOG_PRIORITY,
                             OS_WATCHDOG_HANDLER, 0, OS_TICK_PARAMETER)) != OS_SUCCESS)
    {
        RETURN_ERROR(OS_FAIL);
    }
#elif (OS_WATCHDOG_SUPPORT == OS_WDT_DISABLED)
{
    osHwWatchdogDisable(osGetCoreID());
}
#endif // OS_WATCHDOG_SUPPORT
#endif // OS_WATCHDOGS


/* Enable Core watchdog in OS_WDT_AUTO_HANDLING mode */
#if (OS_CORE_WATCHDOGS != 0)
#if (OS_CORE_WATCHDOG_SUPPORT == OS_CORE_WDT_AUTO_HANDLING)
#ifndef OS_CORE_WATCHDOG_HANDLER
#define OS_WATCHDOG_HANDLER  NULL
#endif

    /* assumes watchdog is disabled by default after reset */
    if ((osCoreWatchdogSupport( OS_CORE_WATCHDOG_TIMEOUT,
                                OS_CORE_WATCHDOG_WRS_MODE,
                                OS_CORE_WATCHDOG_PRIORITY,
                                OS_CORE_WATCHDOG_HANDLER,
                                0,
                                OS_TICK_PARAMETER)) != OS_SUCCESS)
    {
        RETURN_ERROR(OS_FAIL);
    }
#endif // OS_WATCHDOG_SUPPORT
#endif // OS_WATCHDOGS

 
#endif // OS_TICK

#if (OS_L1_DEFENSE == ON)
    if(osL1dGetResetMode() != 0)
    {
        osL1dReportStatusSuccess();
    }
#endif //OS_L1_DEFENSE
 
    return osActivate(background_task);
}

