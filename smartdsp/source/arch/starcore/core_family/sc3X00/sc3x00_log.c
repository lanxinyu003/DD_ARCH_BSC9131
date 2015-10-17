/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_log.c,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_log.c,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_log.c

 @Description   SC3X00 Logging functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtimecritical.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_log.h"


#pragma opt_level = "O3"

#define LOG_MAX_HWI     256
#define LOG_MAX_SWI     32
#define LOG_HWI_UNUSED  0
#define LOG_STACK_SIZE  80
#define MAX_NUMBER_OF_EVENTS 10

#define ECNT_CTRL 0xffefff00
#define ECNT_VAL  0xffefff04
#define ECNT_EXT  0xffefff08

void logGetTimeMs(uint32_t *time);


volatile uint32_t   os_log_version = 0x00000001;

volatile uint32_t  *_os_log_pointer     = (uint32_t *)OS_KA_LOG_BASE_DEFAULT;               /* Current LOG pointer */
volatile uint32_t  *_os_base_pointer    = (uint32_t *)OS_KA_LOG_BASE_DEFAULT;               /* Base LOG pointer */
volatile uint32_t  *_os_log_end_pointer = (uint32_t *)OS_KA_LOG_BASE_DEFAULT + 0x100000;    /* Pointer to end of LOG Area */

volatile uint32_t   _g_os_enable_swi_logging            = 0x1;
volatile uint32_t   _g_os_enable_hwi_logging            = 0x1;
volatile uint32_t   _g_os_enable_spinlock_logging       = 0x1;
volatile uint32_t   _g_os_enable_hwi_latency_logging    = 0x0;
volatile uint32_t   _g_os_enable_task_logging           = 0x0;

volatile uint32_t   _g_os_enable_class_profiling = 0x0;

uint32_t            _g_os_log_counter;
uint32_t            _os_log_hwi_used = 0;
extern uint32_t     g_os_ticks;

volatile uint32_t   _os_log_stack[LOG_STACK_SIZE];          /* Current Log Stack */
volatile uint32_t   _os_log_stack_save[LOG_STACK_SIZE];     /* Save For stack on wrap */
volatile uint32_t   _os_log_stack_pointer = 0;                  /* Current Log Stack Pointer */
volatile uint32_t   _os_log_stack_save_pointer;             /* Save For stack on wrap pointer */
volatile uint32_t   _os_log_swi_priorities[LOG_MAX_SWI];    /* SWI Interrupt priorities */


/* Boolean flag set in smartdsp_init.c. If TRUE, user provides log stack position. If FALSE, default values are used. */
extern volatile uint32_t   _g_os_log_by_user;
#if (OS_MULTICORE == ON)
os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size, uint32_t num_cores);
#else
os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size);
#endif


struct
{
    short hwi_num;
    char  priority;

} _os_log_hwi_priority[LOG_MAX_HWI];     /* HWI Interrupt priorities */

struct  user_log_func
{
    void  (*usr_log_func)(uint32_t op, uint32_t val, uint32_t time[2]);
};

struct user_log_func user_log_funcs[MAX_NUMBER_OF_EVENTS] =
{
    NULL, NULL
};





/* This function should return core clock time in 64 bit format. */

/*****************************************************************************/
void logGetTimeMs(uint32_t *time_val)
{
    uint32_t        msb_time;
    uint32_t        lsb_time;
    static uint32_t timer_initialized = 0;
 
    if (!timer_initialized)
    {
        *(volatile uint32_t*) ECNT_CTRL = 0x000001fc;
        *(volatile uint32_t*) ECNT_VAL = 0x7FFFFFFF;
        *(volatile uint32_t*) ECNT_EXT = 0x7FFFFFFF;
        timer_initialized = 1;
    }
 
    do
    {
        msb_time = *(volatile uint32_t*) ECNT_EXT;
        lsb_time = *(volatile uint32_t*) ECNT_VAL;
 
    } while ((*(volatile uint32_t*) ECNT_EXT) != msb_time);
 
    time_val[0] = msb_time;
    time_val[1] = lsb_time;
 
    return;
}


/* NOTES:

LIC HWIs will be reported twice:
Once as general interrupt and then will have numbers:
6,7,8,9,E,0x12,0x15,0x16 (OS_INT_IRQ06_LICOUTA0......)
Secondly, they are reported as LIC interrupts and then
LIC GroupA interrupts are reported as (counter | (1<<16))
and LIC GroupB interrupts are shown as (counter | (2<<16))
The connection between this counter and HWI create number is as follows:

The same for all cores (GROUP A)

 Counter        LIC GroupA interrupt

    0           OS_INT_TDM0RXER
    1           OS_INT_TDM0RSTE
.........................................

 CORE 0 (GROUP B)

 Counter        LIC GroupB interrupt

    0           OS_INT_DMA_0
....................................
    8           OS_INT_TIMER0A


See table 21.3 In SPEC.

*/
void osLogRegisterHandler(uint32_t type_of_command, void  (*handler)(uint32_t op, uint32_t val, uint32_t time[2]))
{
    user_log_funcs[GET_FUNCTION_CODE(type_of_command)].usr_log_func = handler;
}


/*****************************************************************************/
void osLog(uint32_t op, uint32_t val)
{
    uint32_t        time[2];
#pragma align time 8
    uint32_t        type_of_command = op & LOG_COMMAND_TYPE_MASK;
 
    if (type_of_command == OS_LOG_TASK)
    {
        if (_g_os_enable_task_logging)
        goto os_log_continue;
    }
 
    if (type_of_command == OS_LOG_HWI)
    {
        if (_g_os_enable_hwi_logging)
        goto os_log_continue;
    }
    if (type_of_command == OS_LOG_SWI)
    {
        if (_g_os_enable_swi_logging)
        goto os_log_continue;
    }

    if (type_of_command == OS_LOG_SPINLOCK)
    {
        if (_g_os_enable_spinlock_logging)
        goto os_log_continue;
    }

    if (type_of_command == OS_LOG_HWI_LATENCY)
    {
        if (_g_os_enable_hwi_latency_logging)
        goto os_log_continue;
    }
 
    if (type_of_command  == OS_LOG_CLASS_UTILIZATION)
    {
        /* the various enabling of each class is tested after os_log_continue */
        goto os_log_continue;
    }

    if (type_of_command == OS_LOG_USER_DEFINED_EVENT)
    {
        goto os_log_continue;
    }

    return;
 
os_log_continue:

    if(user_log_funcs[GET_FUNCTION_CODE(type_of_command)].usr_log_func)
    {
        logGetTimeMs(time);
        user_log_funcs[GET_FUNCTION_CODE(type_of_command)].usr_log_func(op, val, time);
        return;
    }
 
    osHwiSwiftDisableNoLog();

    logGetTimeMs(time);
 
    if (op & LOG_OPCODE_TO_STACK)
    {
 
        _os_log_stack[_os_log_stack_pointer++] = op;
        _os_log_stack[_os_log_stack_pointer++] = val;
    }
    else if (op & LOG_OPCODE_FROM_STACK)
    {

        _os_log_stack_pointer -= 2;
    }
 
    /* updating software interrupt priorities */
    else if ((op == LOG_SWI_SET_PRIORITY) || (op == LOG_SWI_CREATE))
    {
        int swi_num = (int)(val & 0xFFFF);
 
        if (swi_num < LOG_MAX_SWI)
            _os_log_swi_priorities[swi_num] = (val >> 16);
    }
 
    /* updating hardware interrupt priorities */
    else if ((op == LOG_HWI_SET_PRIORITY) || (op == LOG_HWI_CREATE))
    {
        int i;
        int priority = (int)(val >> 16);
        int hwi_num = (int)(val & 0xFFFF);
 
        for (i = 0; i <= _os_log_hwi_used; i++)
        {
            if (_os_log_hwi_priority[i].hwi_num == hwi_num)
            {
                _os_log_hwi_priority[i].priority = (char)priority;
                break;
            }
            else
            {
                if (_os_log_hwi_priority[i].hwi_num == LOG_HWI_UNUSED)
                {
                    _os_log_hwi_priority[i].hwi_num = (short)hwi_num;
                    _os_log_hwi_priority[i].priority = (char)priority;
                    _os_log_hwi_used++;
                    break;
                }
            }
        }
 
        //if (_os_log_hwi_used == LOG_MAX_HWI)
        {
        }
    }
 
    /* updating class information */
    else if (type_of_command  == OS_LOG_CLASS_UTILIZATION)
    {
        /* Nothing to do */
    }

    *_os_log_pointer++ = op;
    *_os_log_pointer++ = (time[0]);
    *_os_log_pointer++ = (time[1]);
    *_os_log_pointer++ = val;

    // Wrap if needed and copy stack to save.
    if ((_os_log_pointer) >= (_os_log_end_pointer))
    {
        int i;
 
        for (i = 0; i < _os_log_stack_pointer; i++)
        {
            _os_log_stack_save[i] = _os_log_stack[i];
        }
        _os_log_stack_save_pointer = _os_log_stack_pointer;
        _os_log_pointer = _os_base_pointer;
        _g_os_log_counter++;
    }

    if ((op != LOG_HWI_ENABLE) && (op != LOG_HWI_DISABLE))
    {
        osHwiSwiftEnableNoLog();
    }
}

void osLogImmediate(uint32_t op, uint32_t val,  uint32_t  time[2])
{
    osHwiSwiftDisableNoLog();

    if (op & LOG_OPCODE_TO_STACK)
    {
 
        _os_log_stack[_os_log_stack_pointer++] = op;
        _os_log_stack[_os_log_stack_pointer++] = val;
    }
    else if (op & LOG_OPCODE_FROM_STACK)
    {

        _os_log_stack_pointer -= 2;
    }
 
    /* updating software interrupt priorities */
    else if ((op == LOG_SWI_SET_PRIORITY) || (op == LOG_SWI_CREATE))
    {
        int swi_num = (int)(val & 0xFFFF);
 
        if (swi_num < LOG_MAX_SWI)
            _os_log_swi_priorities[swi_num] = (val >> 16);
    }
 
    /* updating hardware interrupt priorities */
    else if ((op == LOG_HWI_SET_PRIORITY) || (op == LOG_HWI_CREATE))
    {
        int i;
        int priority = (int)(val >> 16);
        int hwi_num = (int)(val & 0xFFFF);
 
        for (i = 0; i <= _os_log_hwi_used; i++)
        {
            if (_os_log_hwi_priority[i].hwi_num == hwi_num)
            {
                _os_log_hwi_priority[i].priority = (char)priority;
                break;
            }
            else
            {
                if (_os_log_hwi_priority[i].hwi_num == LOG_HWI_UNUSED)
                {
                    _os_log_hwi_priority[i].hwi_num = (short)hwi_num;
                    _os_log_hwi_priority[i].priority = (char)priority;
                    _os_log_hwi_used++;
                    break;
                }
            }
        }
 
        //if (_os_log_hwi_used == LOG_MAX_HWI)
        {
        }
    }

    *_os_log_pointer++ = op;
    *_os_log_pointer++ = (time[0]);
    *_os_log_pointer++ = (time[1]);
    *_os_log_pointer++ = val;

    // Wrap if needed and copy stack to save.
    if ((_os_log_pointer) >= (_os_log_end_pointer))
    {
        int i;
 
        for (i = 0; i < _os_log_stack_pointer; i++)
        {
            _os_log_stack_save[i] = _os_log_stack[i];
        }
        _os_log_stack_save_pointer = _os_log_stack_pointer;
        _os_log_pointer = _os_base_pointer;
        _g_os_log_counter++;
    }

    if ((op != LOG_HWI_ENABLE) && (op != LOG_HWI_DISABLE))
    {
        osHwiSwiftEnableNoLog();
    }

}


/*****************************************************************************/
#if (OS_MULTICORE == 1)
/* find mmu segment with vaddr within */
#ifdef MMU_SUPPORT
#include <att_mmu.h>


/* find mmu segment with vaddr within */
static int mmuFindDataSegment(uint32_t vaddr, struct att_mmu* att_mmu_table,int mmu_table_size)
{
    uint32_t vaddr_offset;
    int i;

    for (i=0; i<mmu_table_size ; i++)
    {
        /* this is program entry */
        if(att_mmu_table[i].other & OVL_OTHER_EXEC) continue;

        /* check if address within limits */
        vaddr_offset = vaddr - ((uint32_t)att_mmu_table[i].base_address);
        if(vaddr_offset < att_mmu_table[i].physical_size) return i;
    }

    /* not found */
    return -1;
}
#endif


os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size, uint32_t num_cores)
{
#pragma opt_level = "O0"
    uint32_t core_base_addr, core_stack_size;
    uint8_t  core_id;
#ifdef MMU_SUPPORT
    struct att_mmu* address_translation_table_mmu = _address_translation_table_mmu;
    unsigned long int address_translation_table_mmu_count = _address_translation_table_mmu_count;
#endif
 
    /* Sanity check - Kernel Awareness DLL in sync with project */
    if (_g_os_log_by_user != TRUE)
    {
        OS_ASSERT;
        return OS_FAIL;
    }
    /* Change this line ONLY pased on platform implementaiton */
    core_id = (uint8_t)((*(uint32_t *)0xFFF06028 & 0x00FF0000) >> 16);
 
    /* If MMU is supported, check if the address is already mapped by (e.g.) an _RESEREVED_
     * entry into the lcf.
     * If not mapped (or no support) assume that the space is shared and continguous and
     * must be broken up for all cores.
     * If mapped, assign to kernel awareness pointers as-is */
#ifdef MMU_SUPPORT
    if (mmuFindDataSegment((uint32_t)base_addr, address_translation_table_mmu, (int)address_translation_table_mmu_count) == -1)
#endif
    {
        core_stack_size = stack_size / num_cores;
        core_base_addr = (uint32_t)base_addr + core_id * core_stack_size;
    }
#ifdef MMU_SUPPORT
    else
    {
        core_stack_size = stack_size;
        core_base_addr = (uint32_t)base_addr;
    }
#endif
 
    _os_log_pointer = (uint32_t*)core_base_addr;
    _os_base_pointer = (uint32_t*)core_base_addr;
    _os_log_end_pointer = (uint32_t*)(core_base_addr + core_stack_size);

    return OS_SUCCESS;
}

#else /* OS_MULTICORE */

os_status osLogInitialize(uint8_t *base_addr, uint32_t stack_size)
{
    /* Sanity check - Kernel Awareness DLL in sync with project */
    if (_g_os_log_by_user != TRUE)
    {
        OS_ASSERT;
        return OS_FAIL;
    }

    _os_log_pointer = (uint32_t*)base_addr;
    _os_base_pointer = (uint32_t*)base_addr;
    _os_log_end_pointer = (uint32_t*)((uint32_t)base_addr + stack_size);
 
    return OS_SUCCESS;
}
#endif /* OS_MULTICORE */


