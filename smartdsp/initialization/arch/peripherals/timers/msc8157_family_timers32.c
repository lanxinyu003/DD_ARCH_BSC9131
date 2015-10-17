/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:27:57 $
 $Id: msc8157_family_timers32.c,v 1.8 2012/11/22 16:27:57 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/timers/msc8157_family_timers32.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          msc8157_family_timers.c

 @Description   This file contains the implementation of device specific timers.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"

#include "hw_timers32_init.h"
#include "os_timer_.h"
#include "os_hw_timer.h"
#include "hw_timers32.h"
#include "hw_timers32_.h"
#include "hw_timers_.h"
#include "hw_timers32_shared_.h"
#include "starcore_utils_.h"

extern hw_timer32_t           g_os_hw_timers32;

/*****************************************************************************/
os_status   hwTimer32TriggerInputSelect(uint32_t module, timer32_input_select_t mux_select, hw_timer_trigger_input_t trigger)
{
    volatile uint32_t *gcr  = (module == 1) ? &g_msc815x_ccsr->general_config.gcr8 : &g_msc815x_ccsr->general_config.gcr7;

    SET_UINT32(*gcr, (trigger << (mux_select*8)));
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status   hwTimer32InputClkSelect(uint32_t is_32_bit, timer_clock_input_t source)
{
    uint32_t   gcr8_reg;
 
    if (source == SOC_CLOCK_IN_CPRI)
    {
        /* 32 bit timer */
        gcr8_reg = is_32_bit ? 0x80000000 : 0x40000000;

        SET_UINT32(g_msc815x_ccsr->general_config.gcr8, gcr8_reg);
    }
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32InterruptGet(uint32_t timer_num, uint32_t *timer_interrupt)
{
    *timer_interrupt = (uint32_t)(OS_INT_SOC_TMR0 + timer_num);
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status hwTimer32InterruptCreate(soc_timer32_num_t timer_num,
                                   os_hwi_priority priority,
                                   os_hwi_mode     mode,
                                   os_hwi_function handler)
{

    VAR_UNUSED(mode);
    return osHwiCreate((os_hwi_handle)((uint32_t)OS_INT_SOC_TMR0 + (uint32_t)timer_num),
                priority,
                (os_hwi_mode)LEVEL_MODE,
                handler,
                (os_hwi_arg)timer_num);
}

/*****************************************************************************/
os_status hwTimer32InterruptDelete(soc_timer32_num_t timer_num)
{
    hw_timer32_t   *timers = &g_os_hw_timers32;

    return osHwiDelete((uint16_t)timers->array[timer_num]->timer);
}

