/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/11/09 14:45:35 $
 $Id: hw_timers32.c,v 1.27 2014/11/09 14:45:35 b41932 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/hw_timers32.c,v $
 $Revision: 1.27 $
 ******************************************************************************/

/******************************************************************************

 @File          hw_timers.c

 @Description   Hardware 32 bit timers runtime functions.

 @Cautions      None.

 *//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_rm_.h"
#include "os_error.h"
#include "starcore_utils_.h"
#include "smartdsp_os_device.h"
#include "hw_timers32_init.h"
#include "hw_timers_.h"
#include "hw_timers32_.h"
#include "hw_timers32_shared_.h"
#include "hw_timers32.h"
#include "memmap.h"

extern hw_timer32_t           g_os_hw_timers32;
tmr32b_map_t                *soc_timer32_module;
static uint16_t             timers_control_32b[NUM_OF_HW_TIMERS_32b];

#define HW_TIMER32_CONFIG_COUNT_DOWN   0x0001

os_status hwTimer32PutInList(void **timer);

/***********************************************************************/
inline unsigned int timer32_module(os_timer_handle timer_num)
{
    return (unsigned int)(timer_num >> 2);
}

/***********************************************************************/
inline unsigned int timer32_in_module(os_timer_handle timer_num)
{
    return (unsigned int)(timer_num & 0x0003);
}

/***********************************************************************/
static inline unsigned int numeric_trans_timer32_in_module(timer32_number_in_module_t timer_num)
{
    return (unsigned int)get_num_of_first_one_bit(timer_num);
}
 

/*****************************************************************************/
os_status       hwTimer32Open(soc_timer32_num_t timer_num, timer32_open_params_t    *params)
{
    uint32_t    tmr_comscr = 0;
    uint16_t     tmr_cntrl, tmr_sctl=0x0000;
    unsigned int module         = timer32_module((os_timer_handle)timer_num);
    unsigned int timer          = timer32_in_module((os_timer_handle)timer_num);
    uint16_t     *timers_ctrl   = NULL;
    hw_timer32_t   *timers        = NULL;
    volatile uint32_t *resource_manager = NULL;
    os_status    status;
    timers_ctrl = timers_control_32b;
    timers = &g_os_hw_timers32;

    /****************** Start Error checking ********************/
#ifdef HW_TIMER_ERROR_CHECKING

    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }

    if ((params->operation_mode < SOC_TIMER32_ONE_SHOT) || (params->operation_mode > SOC_TIMER32_FREE_RUN))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_MODE_INVALID);
    }

    if (params->primary_source >= NUM_OF_HW_TIMERS_SOURCES+MAX_HW_TIMERS_SECONDARY_SOURCES)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_SOURCE_INVALID);
    }
    if ((params->priority > OS_HWI_PRIORITY30) && (params->priority != OS_HWI_PRIORITY_NMI))
    {
#ifdef HWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* HWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_PRIORITY_INVALID);
    }

    if (module >= NUM_OF_HW_TIMER_32b_MODULES)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }

#endif /* HW_TIMER_ERROR_CHECKING */

#ifdef B4860_FAMILY    // see A-006674 Device-level timer counter does not reinitialize when configured to be periodic and preloaded
    if ( params->operation_mode == SOC_TIMER32_PERIODIC && (params->preload_ctrl1 != SOC_TIMER32_PRELOAD_NEVER || params->preload_ctrl2 != SOC_TIMER32_PRELOAD_NEVER) )
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_MODE_INVALID);
    }
#endif //B4860_FAMILY
    /****************** End Error checking ********************/
    resource_manager = &(g_os_hw_timers32_free[module]);
    timers           = &g_os_hw_timers32;
 
    g_os_hw_timers32_l1d_data.timer32_core_id[timer_num] = osGetCoreID();

    /* check that timers are available */
    osHwiSwiftDisable();
    osSpinLockSwiftGet(&g_os_hw_timers32_guard);
    status = indexedResourceForceGet(resource_manager, ((uint32_t)(timer_num & (NUM_OF_HW_TIMERS_32b_PER_MODULE-1))));
    osSpinLockSwiftRelease(&g_os_hw_timers32_guard);

    if (status != OS_SUCCESS)
    {
        osHwiSwiftEnable();

#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_UNAVAILABLE);
    }

    osHwiSwiftEnable();

    switch ( params->counting_mode)
    {
        case SOC_TIMER32_DEFAULT_COUNT:
            /* default */
            tmr_cntrl = 0x2003;
            break;
        case SOC_TIMER32_COUNT_RISING_EDGE:
            tmr_cntrl = 0x2003;
            break;
        case SOC_TIMER32_COUNT_RISING_AND_FALL_EDGE:
            tmr_cntrl = 0x4003;
            break;
        case SOC_TIMER32_COUNT_RISING_PRIMARY:
            tmr_cntrl = 0x6003;
            break;
        case SOC_TIMER32_COUNT_QUADRATURE:
            tmr_cntrl = 0x8003;
            break;
        case SOC_TIMER32_COUNT_PRIMARY_DIR_SECONDARY:
            tmr_cntrl = 0xA003;
            break;
            /*
            case SOC_TIMER32_COUNT_SECONDARY_TRIGGER:
                tmr_cntrl = 0xD003;
                break;
              timers behavior does not match HW documentation      */
        case SOC_TIMER32_COUNT_CASCADED:
            tmr_cntrl = 0xE003;
            break;
        default:
            RETURN_ERROR(OS_ERR_HW_TM_MODE_INVALID);
            break;
    }


    /* add primary source input */
    tmr_cntrl |= (params->primary_source  << 9);

    /* add secondary source input */
    if (params->secondary_source != SOC_TIMER32_SECONDARY_NOT_IN_USE)
    {
        tmr_cntrl |= (((uint32_t)params->secondary_source) << 7);
    }
    if (params->operation_mode == SOC_TIMER32_ONE_SHOT)
    {
        tmr_cntrl |= 0x0040;    /* Count Once bit = 1 */
    }
    else if (params->operation_mode == SOC_TIMER32_PERIODIC)
    {
        tmr_cntrl |= 0x0020;    /* Count Length bit = 1 */
    }

    osHwiSwiftDisable();

    if(params->output_enable)
    {
        tmr_sctl |= 0x1;
    }

#ifdef HW_TIMER_ERROR_CHECKING
    if (timers->array[timer_num]->status >= OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_HW_TM_ALREADY_CREATED);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    /* Force value 0 on output */
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_sctl, tmr_sctl);
 
    tmr_comscr |= params->preload_ctrl1;
    tmr_comscr |= (params->preload_ctrl2 << 2);
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_comscr, tmr_comscr);
 
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cntr, 0);

    if (params->direction == SOC_TIMER32_COUNT_UP)
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp1, params->interval);
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmpld1, params->preload);
    }
    else
    { /* count down */
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp2, params->interval);
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmpld2, params->preload);
        tmr_cntrl |= 0x0010;
        timers->array[timer_num]->timer_config |= HW_TIMER32_CONFIG_COUNT_DOWN;
    }

    timers_ctrl[timer_num] = tmr_cntrl;

    /* timer interrupts are required */
    if (params->handler != NULL)
    {
        /* this call cannot fail (all parameters are valid) */
        hwTimer32InterruptCreate(timer_num,
                params->priority,
                (os_hwi_mode)LEVEL_MODE,
                params->handler);

        /* Enable timer interrupt */
        SET_UINT32(soc_timer32_module[module].tmr[timer].tmr_sctl, TMR32_SCTL_TCFIE);
    } // if (handler != NULL)

#if !defined(MSC8157)
#if defined(B4860)
    if (params->mpic_ie)
    {
        /* this call cannot fail (all parameters are valid) */
        hwTimer32MpicInterruptAssign(timer_num);
    }
#endif
    if (params->maple_ie)
    {
        /* this call cannot fail (all parameters are valid) */
        hwTimer32MapleInterruptAssign(timer_num,(soc_timer32_maple_t)params->maple_id);
    }
#if defined(B4860)
    if ((params->mpic_ie)||(params->maple_ie))
#else
    if (params->maple_ie)
#endif
    {
        /* Enable timer interrupt */
        SET_UINT32(soc_timer32_module[module].tmr[timer].tmr_sctl, TMR32_SCTL_TCFIE);
    }
#endif //!defined(MSC8157)    /* timer interrupts are required */
 
    timers->array[timer_num]->status = OS_RESOURCE_USED;
    timers->array[timer_num]->reload_value = (uint32_t)(params->interval);
    timers->array[timer_num]->timer_function = params->handler;

    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status   hwTimer32SetInterval(soc_timer32_num_t timer_num, os_timer_interval interval)
{
    os_status status = OS_SUCCESS;
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    hw_timer32_t   *timers       = NULL;

    timers = &g_os_hw_timers32;

#ifdef HW_TIMER_ERROR_CHECKING
    if ((timer_num >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
    if (timers->array[timer_num]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
    else if (timers->array[timer_num]->status == OS_RESOURCE_ACTIVE)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_OS_ERR_BUSY);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    if (timers->array[timer_num]->timer_config & HW_TIMER32_CONFIG_COUNT_DOWN)
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp2, interval);
    }
    else
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp1, interval);
    }

    timers->array[timer_num]->reload_value = (uint32_t)interval;
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return status;
}

/*****************************************************************************/
os_status hwTimer32Delete(soc_timer32_num_t timer_num)
{
    os_status status = OS_SUCCESS;
    hw_timer32_t   *timers                = NULL;
    volatile uint32_t *resource_manager = NULL;
    unsigned int module        = timer32_module(timer_num);


    timers = &g_os_hw_timers32;

#ifdef HW_TIMER_ERROR_CHECKING

    if (module >= NUM_OF_HW_TIMER_32b_MODULES)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
    if ((timer_num >= NUM_OF_HW_TIMERS_32b))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
    if (timers->array[timer_num]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
#endif /* HW_TIMER_ERROR_CHECKING */


    /* error checks are done inside osHwTimerStop */

    g_os_hw_timers32_l1d_data.timer32_core_id[timer_num] = NO_CORE_ID;
 
    resource_manager = &g_os_hw_timers32_free[module];
    if (timers->array[timer_num]->status == OS_RESOURCE_ACTIVE)
        status = hwTimer32Stop(timer_num);

    if (status == OS_SUCCESS)
    {
        osHwiSwiftDisable();

        osSpinLockSwiftGet(&g_os_hw_timers32_guard);
        indexedResourceFree(resource_manager, ((uint32_t)(timer_num & (NUM_OF_HW_TIMERS_32b_PER_MODULE-1))));
        osSpinLockSwiftRelease(&g_os_hw_timers32_guard);

        if (timers->array[timer_num]->timer_function)
        {
            hwTimer32InterruptDelete(timer_num);
            timers->array[timer_num]->timer_function = NULL;
        }

        timers->array[timer_num]->status = OS_RESOURCE_DELETED;
        timers->array[timer_num]->timer_config = 0x0;

        osHwiSwiftEnable();
    }

    return status;
}


/*****************************************************************************/
os_status hwTimer32Start(soc_timer32_num_t timer_num)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    uint16_t     *timers_ctrl  = NULL;
    hw_timer32_t   *timers       = NULL;

    timers_ctrl = timers_control_32b;
    timers = &g_os_hw_timers32;

#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }

    if (timers->array[timer_num]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
    if (timers->array[timer_num]->status == OS_RESOURCE_ACTIVE)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_ALREADY_ACTIVE);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    timers->array[timer_num]->status = OS_RESOURCE_ACTIVE;

    /* enable the timer */
    // Only 32b timers in system
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cntr, 0);
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num]);
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY


    return OS_SUCCESS;
}


/*****************************************************************************/
os_status hwTimer32Stop(soc_timer32_num_t timer_num)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    uint16_t     *timers_ctrl  = NULL;
    hw_timer32_t   *timers       = NULL;

    timers_ctrl = timers_control_32b;
    timers = &g_os_hw_timers32;

#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }

    if (timers->array[timer_num]->status < OS_RESOURCE_ACTIVE)
    {
        if (timers->array[timer_num]->status < OS_RESOURCE_USED)
        {
#ifdef HW_TIMER_ERROR_ASSERT
            OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
            RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
        }
        else
        {
#ifdef HW_TIMER_ERROR_ASSERT
            OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
            RETURN_ERROR(OS_ERR_HW_TM_NOT_ACTIVE);
        }
    }
#endif // HW_TIMER_ERROR_CHECKING

    /* disable the timer */
    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_ctrl, timers_ctrl[timer_num] & 0xE);
    timers->array[timer_num]->status = OS_RESOURCE_USED;
 
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}


/*****************************************************************************/
void hwTimer32ClearEvent(soc_timer32_num_t timer_num)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
#ifdef B4860_FAMILY
    uint16_t    reg;
#endif
 
#ifdef HW_TIMER_ERROR_CHECKING

    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        return;
    }
#endif /* HW_TIMER_ERROR_CHECKING */
#ifdef B4860_FAMILY
    CLEAR_UINT16(soc_timer32_module[module].tmr[timer].tmr_sctl, TMR32_SCTL_TCF);
    DBAR_SCFG();
    READ_UINT16(reg, soc_timer32_module[module].tmr[timer].tmr_sctl);
#else
    CLEAR_UINT32(soc_timer32_module[module].tmr[timer].tmr_sctl, TMR32_SCTL_TCF);
#endif //B4860_FAMILY
}

/*****************************************************************************/
os_status hwTimer32ValueGet(soc_timer32_num_t timer_num, os_timer_interval *value)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    uint32_t     val           = 0;

#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    READ_UINT32(val, soc_timer32_module[module].tmr[timer].tmr_cntr);

    *value = val;
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status hwTimer32HoldGet(soc_timer32_num_t timer_num, uint32_t *value)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    uint32_t     val           = 0;

#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    READ_UINT32(val, soc_timer32_module[module].tmr[timer].tmr_hold);

    *value = val;
    return OS_SUCCESS;
}
/*****************************************************************************/

os_status hwTimer32CounterSet(soc_timer32_num_t timer_num, uint32_t value)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);

#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cntr, value);
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32PreloadSet(soc_timer32_num_t timer_num, uint32_t preload_value)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    hw_timer32_t   *timers        = &g_os_hw_timers32;


#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    if (timers->array[timer_num]->timer_config & HW_TIMER32_CONFIG_COUNT_DOWN)
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmpld2, preload_value);
    }
    else
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmpld1, preload_value);
    }
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32CompareSet(soc_timer32_num_t timer_num, uint32_t compare)
{
    unsigned int module        = timer32_module(timer_num);
    unsigned int timer         = timer32_in_module(timer_num);
    hw_timer32_t   *timers        = &g_os_hw_timers32;


#ifdef HW_TIMER_ERROR_CHECKING
    if (timer_num >= NUM_OF_HW_TIMERS_32b)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_INVALID);
    }
    if (timers->array[timer_num]->status < OS_RESOURCE_USED)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_NOT_CREATED);
    }
    else if (timers->array[timer_num]->status == OS_RESOURCE_ACTIVE)
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* HW_TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_HW_TM_OS_ERR_BUSY);
    }
#endif /* HW_TIMER_ERROR_CHECKING */

    if (timers->array[timer_num]->timer_config & HW_TIMER32_CONFIG_COUNT_DOWN)
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp2, compare);
    }
    else
    {
        WRITE_UINT32(soc_timer32_module[module].tmr[timer].tmr_cmp1, compare);
    }
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32SetAndForget(soc_timer32_module_t module, timer32_input_select_t timer_input)
{
    /* clear is required by HW for preventing the lock to start from previous signals */
    SET_UINT32(soc_timer32_module[module].tmr_clrl, (0x1 << timer_input));
    SET_UINT32(soc_timer32_module[module].tmr_saf, (0x1 << timer_input));
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32LockClear(soc_timer32_module_t module, timer32_input_select_t timer_input)
{

    CLEAR_UINT32(soc_timer32_module[module].tmr_saf, (0x1 << timer_input));
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}


/*****************************************************************************/
static os_status put_in_list(hw_timer32_t_ **timer)
{
    OS_ASSERT_COND(timer != NULL);

    if (*timer == NULL)
    {
        *timer = (hw_timer32_t_ *)osMalloc(sizeof(hw_timer32_t_), OS_MEM_LOCAL);
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
os_status hwTimer32PutInList(void **timer)
{
    return put_in_list((hw_timer32_t_ **)timer);
}




/*****************************************************************************/
uint32_t hwTimer32GlobalGet(soc_timer32_module_t module)
{
    volatile uint32_t reg;
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > NUM_OF_HW_TIMER_32b_MODULES-1));
#endif
    // Reading one of the counters in order to load the global timer
    READ_UINT32(reg, soc_timer32_module[module].tmr[0].tmr_cntr);
    return GET_UINT32(soc_timer32_module[module].tmr_glb);
}

/*****************************************************************************/
os_status hwTimer32GlobalConfig(soc_timer32_module_t module,
        timer32_number_in_module_t timers,
        uint32_t     hold0,
        uint32_t     hold1,
        uint32_t     hold2)
{
    uint32_t reg;
#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > NUM_OF_HW_TIMER_32b_MODULES-1));
    OS_ASSERT_COND((timers & ~(SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_1 | SOC_TIMER32_IN_MODULE_TIMER_2 | SOC_TIMER32_IN_MODULE_TIMER_3)) == 0);
    OS_ASSERT_COND(hold0 < 32);
    OS_ASSERT_COND(hold1 < 32);
    OS_ASSERT_COND(hold2 < 32);
#endif


#ifdef HW_TIMER_ERROR_CHECKING
    if (((timers & SOC_TIMER32_IN_MODULE_TIMER_0) && (!(hold0 < 32)                    )) ||
            ((timers & SOC_TIMER32_IN_MODULE_TIMER_1) && (!(hold1 < 32) || !(hold1 > hold0))) ||
            ((timers & SOC_TIMER32_IN_MODULE_TIMER_2) && (!(hold2 < 32) || !(hold2 > hold1))) ||
            ((timers & SOC_TIMER32_IN_MODULE_TIMER_3) && (!(hold2 < 31))))
    {
#ifdef HW_TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif
        RETURN_ERROR(OS_ERR_TM_MODE_INVALID);
    }
#endif
    // flip the bits, since setting "1" means "hold" whereas we want to not-hold
    timers ^= (SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_1 | SOC_TIMER32_IN_MODULE_TIMER_2 | SOC_TIMER32_IN_MODULE_TIMER_3);
    reg =  hold0 | (hold1 << 8) | (hold2 << 16) | (timers << 28);
    WRITE_UINT32(soc_timer32_module[module].tmr_glbctl, reg);
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32SetInitiatorMode(soc_timer32_module_t module,
                                    timer32_number_in_module_t initiator_timer,
                                    uint32_t     reinit_timers,
                                    uint32_t     output_signal_timers)
{
    uint32_t i = 0;
    uint32_t reinit_timers_ = reinit_timers;
    uint32_t output_signal_timers_ = output_signal_timers;
    uint16_t *timers_ctrl = timers_control_32b;

#ifdef HW_TIMER_ERROR_ASSERT
    OS_ASSERT_COND(!(module > NUM_OF_HW_TIMER_32b_MODULES-1));
    OS_ASSERT_COND((initiator_timer & ~(SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_1
                                             | SOC_TIMER32_IN_MODULE_TIMER_2 | SOC_TIMER32_IN_MODULE_TIMER_3)) == 0);
    OS_ASSERT_COND((reinit_timers & ~(SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_1
                                      | SOC_TIMER32_IN_MODULE_TIMER_2 | SOC_TIMER32_IN_MODULE_TIMER_3)) == 0);
    OS_ASSERT_COND((output_signal_timers & ~(SOC_TIMER32_IN_MODULE_TIMER_0 | SOC_TIMER32_IN_MODULE_TIMER_1
                                             | SOC_TIMER32_IN_MODULE_TIMER_2 | SOC_TIMER32_IN_MODULE_TIMER_3)) == 0);
#endif

    /* set initiator timer */
    SET_UINT32(soc_timer32_module[module].tmr[numeric_trans_timer32_in_module(initiator_timer)].tmr_sctl, TMR32_SCTL_MSTR);
 
    /* set timers to reinitialized when a compare event occurs on the initiator timer */
    while(reinit_timers_!=0)
    {
        if (reinit_timers_ & 0x1)
        {
            timers_ctrl[FIRST_TIMER_IN_MODULE(module)+i] |= TMR32_CTRL_COINIT;
        }
        reinit_timers_ = reinit_timers_ >>1;
        i++;
    }
 
    /* set timers to forces its output flag signal when a compare event occurs on the initiator timer */
    i=0;
    while(output_signal_timers_ !=0)
    {
        if (output_signal_timers_ & 0x1)
        {
            SET_UINT32(soc_timer32_module[module].tmr[i].tmr_sctl, TMR32_SCTL_EEOF);
        }
        output_signal_timers_ = output_signal_timers_ >>1;
        i++;
    }
 
#ifdef B4860_FAMILY
    DBAR_SCFG();
#endif //B4860_FAMILY

    return OS_SUCCESS;
}

