/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/07 11:31:33 $
 $Id: os.c,v 1.35 2013/07/07 11:31:33 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os.c,v $
 $Revision: 1.35 $
******************************************************************************/

/******************************************************************************
 
 @File          os.c

 @Description   OS Initialization and start-up functions.

                This file also contains general utility functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_.h"
#include "os_list_.h"
#include "os_swi_.h"
#include "os_task_.h"
#include "os_task.h"
#include "os_tick_.h"
#include "os_version_.h"

#ifdef SIMULATOR
#include "stdio.h"
#endif

extern uint8_t              g_background_stack[];
extern volatile os_tcb_t    *g_highest_task;
extern volatile os_tcb_t    *g_running_task;
uint16_t                    *g_interrupt_stack = NULL;
uint16_t                    *g_temp_stack_holder;
os_status                   g_os_last_error = OS_SUCCESS;

uint32_t                    g_system_clock;
extern volatile unsigned long long    g_os_ticks;
volatile uint16_t           g_os_flags;
volatile uint16_t           g_os_task_lock = 0;

uint8_t                     g_dev_id;
#if (OS_MULTICORE == 1)
os_core_info_t              g_core_info;
uint8_t                     g_num_of_cores;
uint8_t                     g_max_num_of_cores;
#endif // OS_MULTICORE

#ifdef OS_LOAD_CALCULATION
static float_t              s_load;
static uint32_t             s_reference_counter = 0;
uint32_t                    g_run_counter;
#endif // OS_LOAD_CALCULATION

extern uint32_t             g_background_stack_size;

/*****************************************************************************/
#if (OS_MULTICORE == 1)
os_status   osInitializeKernel(uint32_t system_clock,
                               uint8_t  num_of_cores,
                               uint8_t  max_num_of_cores,
                               uint8_t  id_of_master_core)
#else
os_status   osInitializeKernel(uint32_t system_clock)
#endif // OS_MULTICORE
{
    /* Setup stack pointer. This is the interrupt stack. */
    g_temp_stack_holder = 0;
 
    g_os_flags = 0;
    g_system_clock = system_clock;

#if (OS_MULTICORE == 1)
    g_num_of_cores = num_of_cores;
    g_max_num_of_cores = max_num_of_cores;
    g_core_info.soc_master_core = id_of_master_core;
#endif // OS_MULTICORE

    return OS_SUCCESS;
}

char bg_name[TASK_NAME_LEN] = "background";


/*****************************************************************************/
os_status osActivate(os_background_task_function background_task)
{
    os_status status;
    extern volatile int g_int_disable_counter;
 
    char name[TASK_NAME_LEN] = "background";
    os_task_init_param_t os_task_init_param;
 
    /* Create background task. */
    os_task_init_param.task_function = (os_task_function) background_task;
    os_task_init_param.task_arg = 0;
    os_task_init_param.top_of_stack = (uint32_t)g_background_stack;
    os_task_init_param.stack_size = g_background_stack_size;
    os_task_init_param.private_data = 0;
    os_task_init_param.init_stack = TRUE;
    os_task_init_param.task_name = bg_name;
    os_task_init_param.task_priority = OS_TASK_PRIORITY_LOWEST;
 

    status = osTaskCreate(TASK_IDLE_ID, &os_task_init_param);
    OS_ASSERT_COND(status == OS_SUCCESS);

    status = osTaskActivate(TASK_IDLE_ID);
    OS_ASSERT_COND(status == OS_SUCCESS);

   /* Indicate that background task is running. */
    g_os_flags |= OS_FLG_BGD_ACTIVE;
 
 
    /* g_int_disable_counter is initialized with 1 to prevent the user from calling
       osHwiSwiftDisable() during the application bring up and accidentally enabling
       interrupts. This should set g_int_disable_counter to 0  */
    osHwiSwiftEnable();
    // Make sure that interrupts have been enabled
    OS_ASSERT_COND(g_int_disable_counter == 0);
 
    /* Start Multitasking. */
    osFirstTimeSwitch();

    /* Execution should not reach this point. */
    RETURN_ERROR(OS_FAIL);
}


/*****************************************************************************/
void osGetVersionInfo(os_version_t *version_info)
{
    version_info->major  = OS_VERSION_MAJOR;
    version_info->minor  = OS_VERSION_MINOR;
    version_info->custom = OS_VERSION_CUSTOM;
    version_info->patch  = OS_VERSION_PATCH;
}


/*****************************************************************************/
#ifndef HAVE_OS_ERROR_DEBUG
void osErrorDebug()
{
#ifdef SIMULATOR
    printf(" DEBUG STOP\n");
#else
    DEBUG();
#endif
}
#endif // HAVE_OS_ERROR_DEBUG

/*****************************************************************************/
uint32_t osGetSystemClock()
{
    return g_system_clock;
}


#ifdef OS_LOAD_CALCULATION

/*****************************************************************************/
void osCalcRefCounter()
{
    os_hwi_status int_status;
 
    g_os_ticks = 0;
 
    while ( g_os_ticks < OS_MEASURE_LOAD_TICKS )
    {
        int_status = osHwiDisable();
        s_reference_counter++;
        osHwiEnable(int_status);
    }
}


/*****************************************************************************/
uint32_t osGetCoreLoad()
{
    if (s_reference_counter == 0)
        osCalcRefCounter();
    osCalcLoad();
    return (uint32_t)s_load;
}


/*****************************************************************************/
void osCalcLoad()
{
    s_load = 100 * (float_t)(s_reference_counter - g_run_counter) / (float_t)s_reference_counter ;
    g_run_counter = 0;
}

#endif // OS_LOAD_CALCULATION

int osMulticoreSupport()
{
    return OS_MULTICORE;
}

#if (OS_MULTICORE == 1)

/*****************************************************************************/
uint8_t osGetNumberOfCores()
{
    return g_num_of_cores;
}


#endif // OS_MULTICORE

/*****************************************************************************/
void osWait(uint32_t num) __attribute__((noinline))
{

#pragma opt_level = "O3"
 
    uint32_t __i;
    for (__i=0; __i<(num) ; __i++) NOP();
}

