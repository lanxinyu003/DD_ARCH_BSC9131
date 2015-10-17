/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/19 17:59:20 $
 $Id: hw_timers.c,v 1.10 2013/02/19 17:59:20 b41640 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/hw_timers.c,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_timers.c

 @Description   Hardware timers runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_timer_.h"
#include "os_rm_.h"
#include "starcore_utils_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "hw_timers.h"
#include "hw_timers_.h"
#include "hw_timers_shared_.h"


#ifdef ARCH_SUPPORTS_TIMER16B
extern os_timer_t           g_os_hw_timers_16b;
tmr_map_t                   *soc_timer_module_16b;
static uint16_t             timers_control_16b[NUM_OF_HW_TIMERS_16b];
#endif

#ifdef ARCH_SUPPORTS_TIMER32B
extern os_timer_t           g_os_hw_timers_32b;
tmr32b_map_t                *soc_timer_module_32b;
static uint16_t             timers_control_32b[NUM_OF_HW_TIMERS_32b];
#endif

/***********************************************************************/
/*
 * Sets the timer_num timer to the equivalent interrupt number
 * timers - timers array
 * is_32b - TRUE if timer is 32b
 * timer_num - timer number without type bit
 */
INLINE void timers_arr_set(os_timer_t *timers, os_timer_handle timer_num, bool is_32b)
{
#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    VAR_UNUSED(is_32b);
    timers->array[timer_num]->timer =  (uint32_t)(OS_INT_SOC_TMR0 + timer_num);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    VAR_UNUSED(is_32b);
    timers->array[timer_num]->timer =  (uint32_t)(OS_INT_TMRCMP0 + timer_num);
#else
    // 32b and 16b timers are supported
    if (is_32b)
        timers->array[timer_num]->timer =  (uint32_t)(OS_INT_SOC_TMR0 + timer_num);
    else
        timers->array[timer_num]->timer =  (uint32_t)(OS_INT_TMRCMP0 + timer_num);
#endif // ARCH_SUPPORTS_TIMER16B
}
/***********************************************************************/
INLINE bool timer_32_bit(os_timer_handle timer_num)
{
#if !defined(ARCH_SUPPORTS_TIMER16B)
    VAR_UNUSED(timer_num);
    return TRUE;
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    VAR_UNUSED(timer_num);
    return FALSE;
#else
    return (bool)((timer_num & HWT_SOC_32BIT) != 0);
#endif
}
/***********************************************************************/
INLINE os_timer_handle soc_timer_num(os_timer_handle timer_num)
{
#ifdef ARCH_SUPPORTS_TIMER32B
    timer_num &= ~HWT_SOC_32BIT;
#endif
    return timer_num;
}

/***********************************************************************/
INLINE unsigned int timer_module(os_timer_handle timer_num)
{
#ifdef ARCH_SUPPORTS_TIMER32B
    timer_num = soc_timer_num(timer_num);
#endif
    return (unsigned int)(timer_num >> 2);
}

/***********************************************************************/
INLINE unsigned int timer_in_module(os_timer_handle timer_num)
{
    return (unsigned int)(timer_num & 0x0003);
}

/***********************************************************************/
os_status osHwTimerCreate(os_timer_handle       timer_num,
                          os_timer_mode         mode,
                          os_timer_interval     interval,
                          os_hw_timer_source    source,
                          os_hwi_function       handler,
                          os_hwi_priority       priority)
{
    uint16_t     tmr_cntrl;
    uint32_t     arch_specific  = (mode & 0xFFFF0000);
    unsigned int module         = timer_module(timer_num);
    unsigned int timer          = timer_in_module(timer_num);
    uint16_t     *timers_ctrl   = NULL;
    os_timer_t   *timers        = NULL;
    os_timer_handle tmr_num     = soc_timer_num(timer_num);
    const bool is_32_bit        = timer_32_bit(timer_num);

#ifdef ARCH_SUPPORTS_TIMER16B
    timers_ctrl             = timers_control_16b;
    timers                  = &g_os_hw_timers_16b;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit)
    {
        timers_ctrl = timers_control_32b;
        timers = &g_os_hw_timers_32b;
    }
#endif

    /* general part of mode */
    mode &= 0xFFFF;
 
    /****************** Start Error checking ********************/
#ifdef HW_TIMER_ERROR_CHECKING
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit && (tmr_num >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (tmr_num >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

    if ((mode < OS_TIMER_ONE_SHOT) || ((mode > OS_TIMER_FREE_RUN)))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_MODE_INVALID);
    }
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (interval > 0xFFFF))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INTERVAL_UNSUPPORTED);
    }
#endif  // ARCH_SUPPORTS_TIMER16B
 
    if (source >= NUM_OF_HW_TIMERS_SOURCES)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_SOURCE_INVALID);
    }
    if ((priority > OS_HWI_PRIORITY30) && (priority != OS_HWI_PRIORITY_NMI))
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_PRIORITY_INVALID);
    }
#endif /* HW_TIMER_ERROR_CHECKING */
 
    /****************** End Error checking ********************/
 
    /* default */
    if( (arch_specific == 0) || (arch_specific == COUNT_RISING_EDGE))
        tmr_cntrl = 0x2003;
    else if( arch_specific == COUNT_RISING_AND_FALL_EDGE)
        tmr_cntrl = 0x4003;
    else if( arch_specific == COUNT_CASCADED)
        tmr_cntrl = 0xE003;
    else RETURN_ERROR(OS_ERR_HW_TM_MODE_INVALID);
 
    /* add source */
    tmr_cntrl |= (source << 9);
 
    if (mode == OS_TIMER_ONE_SHOT)
    {
        tmr_cntrl |= 0x0040;    /* Count Once bit = 1 */
    }
    else if (mode == OS_TIMER_PERIODIC)
    {
        tmr_cntrl |= 0x0020;    /* Count Length bit = 1 */
    }
 
    osHwiSwiftDisable();

    timers_ctrl[tmr_num] = tmr_cntrl;
 
#ifdef HW_TIMER_ERROR_CHECKING
    if (timers->array[tmr_num]->status >= OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_HW_TM_ALREADY_CREATED);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    /* Force value 0 on output */
#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x0004);
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_comscr, 0);
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cmp1, interval);
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cntr, 0);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x0004);
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_comscr, 0);
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cmp1, interval);
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cntr, 0);
#else
    // 32b and 16b timers are supported
    if (is_32_bit)
    {
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x0004);
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_comscr, 0);
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cmp1, interval);
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cntr, 0);
    }
    else
    {
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x0004);
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_comscr, 0);
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cmp1, interval);
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cntr, 0);
    }
#endif
 
    /* timer interrupts are required */
    if (handler != NULL)
    {
        /* this call cannot fail (all parameters are valid) */
        osHwiCreate((os_hwi_handle)timers->array[tmr_num]->timer,
                    priority,
                    (os_hwi_mode)LEVEL_MODE,
                    handler,
                    (os_hwi_arg)timer_num);
 
        /* Enable timer interrupt */
#ifndef ARCH_SUPPORTS_TIMER16B
        // Only 32b timers in system
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x4000);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
        // Only 16b timers in system
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x4000);
#else
        // 32b and 16b timers are supported
        if (is_32_bit)
            WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x4000);
        else
            WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x4000);
#endif // ARCH_SUPPORTS_TIMER16B
 
    } // if (handler != NULL)
 
    timers->array[tmr_num]->status = OS_RESOURCE_USED;
    timers->array[tmr_num]->timer_config |= mode;
    timers->array[tmr_num]->reload_value = interval;
    timers->array[tmr_num]->timer_function = (os_timer_function)handler;
 
    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status   osHwTimerSetInterval(os_timer_handle timer_num, os_timer_interval interval)
{
    os_status status;
    unsigned int module        = timer_module(timer_num);
    unsigned int timer         = timer_in_module(timer_num);
    os_timer_t   *timers       = NULL;
    const bool is_32_bit       = timer_32_bit(timer_num);

    timer_num = soc_timer_num(timer_num);

#ifdef ARCH_SUPPORTS_TIMER16B
    timers     = &g_os_hw_timers_16b;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit)
        timers = &g_os_hw_timers_32b;
#endif

#ifdef HW_TIMER_ERROR_CHECKING
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (interval > 0xFFFF))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INTERVAL_UNSUPPORTED);
    }
#endif  // ARCH_SUPPORTS_TIMER16B

#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit && (timer_num >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (timer_num >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

#endif /* HW_TIMER_ERROR_CHECKING */

    /* other error checks are done inside osHwTimerStop */
    status = osHwTimerStop(timer_num);
 
    if (status == OS_SUCCESS)
    {
        osHwiSwiftDisable();
 
#ifndef ARCH_SUPPORTS_TIMER16B
        // Only 32b timers in system
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cmp1, interval);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
        // Only 16b timers in system
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cmp1, interval);
#else
        // 32b and 16b timers are supported
        if (is_32_bit)
            WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cmp1, interval);
        else
            WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cmp1, interval);
#endif // ARCH_SUPPORTS_TIMER16B

        timers->array[timer_num]->reload_value = interval;
        osHwiSwiftEnable();
 
        status = osHwTimerStart(timer_num);
    }

    return status;
}

/*****************************************************************************/
os_status osHwTimerDelete(os_timer_handle timer_num)
{
    os_status status;
    os_timer_t   *timers                = NULL;
    volatile uint32_t *resource_manager = NULL;
 
#ifdef ARCH_SUPPORTS_TIMER16B
    timers     = &g_os_hw_timers_16b;
    resource_manager = &g_os_hw_timers_16b_free;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (timer_32_bit(timer_num))
    {
        timers = &g_os_hw_timers_32b;
        resource_manager = &g_os_hw_timers_32b_free;
    }
#endif
 
    /* error checks are done inside osHwTimerStop */
    status = osHwTimerStop(timer_num);
    timer_num = soc_timer_num(timer_num);
 
    if (status == OS_SUCCESS)
    {
        osHwiSwiftDisable();
 
        osSpinLockSwiftGet(&g_os_hw_timers_guard);
        indexedResourceFree(resource_manager, (uint32_t)timer_num);
        osSpinLockSwiftRelease(&g_os_hw_timers_guard);
 
        if (timers->array[timer_num]->timer_function)
        {
            osHwiDelete((os_hwi_handle)timers->array[timer_num]->timer);
            timers->array[timer_num]->timer_function = NULL;
        }
 
        timers->array[timer_num]->status = OS_RESOURCE_DELETED;
        /* clear only the timer mode */
        timers->array[timer_num]->timer_config &= 0xFFF0;
 
        osHwiSwiftEnable();
    }
 
    return status;
}


/*****************************************************************************/
os_status osHwTimerStart(os_timer_handle timer_num)
{
    unsigned int module        = timer_module(timer_num);
    unsigned int timer         = timer_in_module(timer_num);
    uint16_t     *timers_ctrl  = NULL;
    os_timer_t   *timers       = NULL;
    const bool is_32_bit       = timer_32_bit(timer_num);

#ifdef ARCH_SUPPORTS_TIMER16B
    timers_ctrl             = timers_control_16b;
    timers                  = &g_os_hw_timers_16b;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit)
    {
        timers_ctrl = timers_control_32b;
        timers = &g_os_hw_timers_32b;
    }
#endif

#ifdef HW_TIMER_ERROR_CHECKING
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

    if (timers->array[soc_timer_num(timer_num)]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
#endif /* HW_TIMER_ERROR_CHECKING */
 
    timer_num = soc_timer_num(timer_num);
 
    /* enable the timer */
#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cntr, 0);
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num]);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cntr, 0);
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num]);
#else
    // 32b and 16b timers are supported
    if (is_32_bit)
    {
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_cntr, 0);
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num]);
    }
    else
    {
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_cntr, 0);
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num]);
    }
#endif // ARCH_SUPPORTS_TIMER16B

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osHwTimerStop(os_timer_handle timer_num)
{
    unsigned int module        = timer_module(timer_num);
    unsigned int timer         = timer_in_module(timer_num);
    uint16_t     *timers_ctrl  = NULL;
    os_timer_t   *timers       = NULL;
    const bool is_32_bit       = timer_32_bit(timer_num);

#ifdef ARCH_SUPPORTS_TIMER16B
    timers_ctrl             = timers_control_16b;
    timers                  = &g_os_hw_timers_16b;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit)
    {
        timers_ctrl = timers_control_32b;
        timers = &g_os_hw_timers_32b;
    }
#endif
 
#ifdef HW_TIMER_ERROR_CHECKING
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

    if (timers->array[soc_timer_num(timer_num)]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
#endif /* HW_TIMER_ERROR_CHECKING */
 
    timer_num = soc_timer_num(timer_num);

    /* disable the timer */
#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num] & 0xE);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_ctrl,
                 timers_ctrl[timer_num] & 0xE);
#else
    // 32b and 16b timers are supported
    if (is_32_bit)
        WRITE_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num] & 0xE);
    else
        WRITE_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num] & 0xE);
#endif // ARCH_SUPPORTS_TIMER16B

    return OS_SUCCESS;
}


/*****************************************************************************/
void osHwTimerClearEvent(os_timer_handle timer_num)
{
    unsigned int module        = timer_module(timer_num);
    unsigned int timer         = timer_in_module(timer_num);
 
#ifdef HW_TIMER_ERROR_CHECKING

#ifdef ARCH_SUPPORTS_TIMER32B
    if (timer_32_bit(timer_num) && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        return;
    }
#endif
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!timer_32_bit(timer_num) && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        return;
    }
#endif
 
#endif /* HW_TIMER_ERROR_CHECKING */

#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    WRITE_SYNCIO_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x4000);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    WRITE_SYNCIO_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x4000);
#else
    // 32b and 16b timers are supported
    if (timer_32_bit(timer_num))
        WRITE_SYNCIO_UINT32(soc_timer_module_32b[module].tmr[timer].tmr_sctl, 0x4000);
    else
        WRITE_SYNCIO_UINT16(soc_timer_module_16b[module].tmr[timer].tmr_sctl, 0x4000);
#endif // ARCH_SUPPORTS_TIMER16B

}

/*****************************************************************************/
os_status osHwTimerValueGet(os_timer_handle timer_num, uint32_t *value)
{
    unsigned int module        = timer_module(timer_num);
    unsigned int timer         = timer_in_module(timer_num);
    uint32_t     val           = 0;
 
#ifdef HW_TIMER_ERROR_CHECKING
#ifdef ARCH_SUPPORTS_TIMER32B
    if (timer_32_bit(timer_num) && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!timer_32_bit(timer_num) && (soc_timer_num(timer_num) >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

#endif /* HW_TIMER_ERROR_CHECKING */
 
#ifndef ARCH_SUPPORTS_TIMER16B
    // Only 32b timers in system
    READ_UINT32(val, soc_timer_module_32b[module].tmr[timer].tmr_cntr);
#elif !defined(ARCH_SUPPORTS_TIMER32B)
    // Only 16b timers in system
    READ_UINT16(val, soc_timer_module_16b[module].tmr[timer].tmr_cntr);
#else
    // 32b and 16b timers are supported
    if (timer_32_bit(timer_num))
        READ_UINT32(val, soc_timer_module_32b[module].tmr[timer].tmr_cntr);
    else
        READ_UINT16(val, soc_timer_module_16b[module].tmr[timer].tmr_cntr);
#endif // ARCH_SUPPORTS_TIMER16B

    *value = val;
    return OS_SUCCESS;
}

/*****************************************************************************/
static os_status put_in_list(os_timer_t_ **timer)
{
    OS_ASSERT_COND(timer != NULL);
 
    if (*timer == NULL)
    {
        *timer = (os_timer_t_ *)osMalloc(sizeof(os_timer_t_), OS_MEM_LOCAL);
        if (*timer == NULL)
        {
#ifdef HW_TIMER_ERROR_ASSERT
            OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
            return OS_ERR_NO_MEMORY;
        }
        (*timer)->status = OS_RESOURCE_UNUSED;
    }

    if ((*timer)->status < OS_RESOURCE_ACQUIRED)
    {
        (*timer)->status = OS_RESOURCE_ACQUIRED;
        return OS_SUCCESS;
    }
#ifdef HW_TIMER_ERROR_ASSERT
            OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
    return OS_ERR_HW_TM_UNAVAILABLE;
}


/*****************************************************************************/
os_status osHwTimerTypeFind(os_timer_handle *timer_num, int type)
{
#ifdef HW_TIMER_DYNAMIC_CONTROL
    os_status   status;
    uint32_t    free_timer;
    volatile uint32_t *resource_manager = NULL;
    os_timer_t        *timers           = NULL;

    OS_ASSERT_COND((type == HWT_SOC_16BIT) || (type == HWT_SOC_32BIT));
 
#ifdef ARCH_SUPPORTS_TIMER16B
    resource_manager = &g_os_hw_timers_16b_free;
    timers           = &g_os_hw_timers_16b;
#endif
#ifdef ARCH_SUPPORTS_TIMER32B
    if (type == HWT_SOC_32BIT)
    {
        resource_manager = &g_os_hw_timers_32b_free;
        timers           = &g_os_hw_timers_32b;
    }
#endif

    OS_ASSERT_COND(timers != NULL);
    OS_ASSERT_COND(resource_manager != NULL);

    osHwiSwiftDisable();
    osSpinLockSwiftGet(&g_os_hw_timers_guard);
    status = reversedIndexedResourceGet(resource_manager, &free_timer);
    osSpinLockSwiftRelease(&g_os_hw_timers_guard);
 
    if (status != OS_SUCCESS)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_UNAVAILABLE);
    }
 
    status = put_in_list(&timers->array[free_timer]);
    if (status != OS_SUCCESS)
    {
        osSpinLockSwiftGet(&g_os_hw_timers_guard);
        indexedResourceFree(resource_manager, free_timer);
        osSpinLockSwiftRelease(&g_os_hw_timers_guard);
        osHwiSwiftEnable();
        RETURN_ERROR(status);
    }

    *timer_num = (os_timer_handle)(free_timer | type);
    timers_arr_set(timers, (os_timer_handle)free_timer, (type == HWT_SOC_32BIT));

    osHwiSwiftEnable();
    return OS_SUCCESS;
 
#endif /* HW_TIMER_DYNAMIC_CONTROL */

    *timer_num = 0xFFFF;

    RETURN_ERROR(OS_ERR_HW_TM_UNAVAILABLE);
}

/*****************************************************************************/
os_status osHwTimerFind(os_timer_handle *timer_num)
{
    return osHwTimerTypeFind(timer_num, __SOC_DEFAULT_TIMER);
}


/*****************************************************************************/
os_status osHwTimerFindForce(os_timer_handle timer_num)
{
    os_status status                    = OS_ERR_HW_TM_UNAVAILABLE;
    os_timer_t   *timers                = NULL;
    volatile uint32_t *resource_manager = NULL;
    const bool is_32_bit                = timer_32_bit(timer_num);
    timer_num                           = soc_timer_num(timer_num);
 
#ifdef ARCH_SUPPORTS_TIMER16B
    timers     = &g_os_hw_timers_16b;
    resource_manager = &g_os_hw_timers_16b_free;
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit)
    {
        timers = &g_os_hw_timers_32b;
        resource_manager = &g_os_hw_timers_32b_free;
    }
#endif
 
#ifdef HW_TIMER_ERROR_CHECKING
#ifdef ARCH_SUPPORTS_TIMER32B
    if (is_32_bit && (timer_num >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif
 
#ifdef ARCH_SUPPORTS_TIMER16B
    if (!is_32_bit && (timer_num >= NUM_OF_HW_TIMERS_16b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
    #endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif

#endif /* HW_TIMER_ERROR_CHECKING */

#ifdef HW_TIMER_DYNAMIC_CONTROL
    osHwiSwiftDisable();
    osSpinLockSwiftGet(&g_os_hw_timers_guard);
    status = indexedResourceForceGet(resource_manager, (uint32_t)timer_num);
    osSpinLockSwiftRelease(&g_os_hw_timers_guard);
 
    if (status != OS_SUCCESS)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_HW_TM_UNAVAILABLE);
    }

    status = put_in_list(&timers->array[timer_num]);
    if (status != OS_SUCCESS)
    {
        osSpinLockSwiftGet(&g_os_hw_timers_guard);
        indexedResourceFree(resource_manager, (uint32_t)timer_num);
        osSpinLockSwiftRelease(&g_os_hw_timers_guard);
    }
    else
        timers_arr_set(timers, timer_num, is_32_bit);

 
    osHwiSwiftEnable();
#endif /* HW_TIMER_DYNAMIC_CONTROL */

    if(status == OS_SUCCESS)
        return status;
    RETURN_ERROR(status);
}


#ifdef ARCH_SUPPORTS_TIMER32B
/*****************************************************************************/
uint32_t osHwTimerGlobalGet(unsigned int module)
{
    volatile uint32_t reg;
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > HWT_MODULE_1));
#endif
    // Reading one of the counters in order to load the global timer
    READ_UINT32(reg, soc_timer_module_32b[module].tmr[0].tmr_cntr);
    return GET_UINT32(soc_timer_module_32b[module].tmr_glb);
}

/*****************************************************************************/
os_status osHwTimerLock(unsigned int module, unsigned int timers)
{
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > HWT_MODULE_1));
    OS_ASSERT_COND((timers & ~(HWT_TIMER_0 | HWT_TIMER_1 | HWT_TIMER_2 | HWT_TIMER_3)) == 0);
#endif
    WRITE_UINT32(soc_timer_module_32b[module].tmr_saf, (uint32_t)timers);
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osHwTimerUnlock(unsigned int module, unsigned int timers)
{
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > HWT_MODULE_1));
    OS_ASSERT_COND((timers & ~(HWT_TIMER_0 | HWT_TIMER_1 | HWT_TIMER_2 | HWT_TIMER_3)) == 0);
#endif
    WRITE_UINT32(soc_timer_module_32b[module].tmr_clrl, (uint32_t)timers);
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osHwTimerGlobalConfig(unsigned int module,
                                unsigned int timers,
                                uint32_t     hold0,
                                uint32_t     hold1,
                                uint32_t     hold2)
{
    uint32_t reg;
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > HWT_MODULE_1));
    OS_ASSERT_COND((timers & ~(HWT_TIMER_0 | HWT_TIMER_1 | HWT_TIMER_2 | HWT_TIMER_3)) == 0);
    OS_ASSERT_COND(hold0 < 32);
    OS_ASSERT_COND(hold1 < 32);
    OS_ASSERT_COND(hold2 < 32);
#endif
 
 
#ifdef HW_TIMER_ERROR_CHECKING
    if (((timers & HWT_TIMER_0) && (!(hold0 < 32)                    )) ||
        ((timers & HWT_TIMER_1) && (!(hold1 < 32) || !(hold1 > hold0))) ||
        ((timers & HWT_TIMER_2) && (!(hold2 < 32) || !(hold2 > hold1))) ||
        ((timers & HWT_TIMER_3) && (!(hold2 < 31))))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif
        RETURN_ERROR(OS_ERR_TM_MODE_INVALID);
    }
#endif
    // flip the bits, since setting "1" means "hold" whereas we want to not-hold
    timers ^= (HWT_TIMER_0 | HWT_TIMER_1 | HWT_TIMER_2 | HWT_TIMER_3);
    reg =  hold0 | (hold1 << 8) | (hold2 << 16) | (timers << 28);
    WRITE_UINT32(soc_timer_module_32b[module].tmr_glbctl, reg);

    return OS_SUCCESS;
}

#endif // defined ARCH_SUPPORTS_TIMER32B
