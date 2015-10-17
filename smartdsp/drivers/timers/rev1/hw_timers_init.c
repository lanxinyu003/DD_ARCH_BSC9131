/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: hw_timers_init.c,v 1.6 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/hw_timers_init.c,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_timers_init.c

 @Description   Hardware timers initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_timer_.h"
#include "os_rm_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "hw_timers.h"
#include "hw_timers_.h"

#ifdef ARCH_SUPPORTS_TIMER16B
/** 16 bit timer variables */
os_timer_t          g_os_hw_timers_16b;
extern tmr_map_t    *soc_timer_module_16b;
static os_timer_t_  *hw_timers_arr_16b[NUM_OF_HW_TIMERS_16b] = {};
#endif

#ifdef ARCH_SUPPORTS_TIMER32B
os_timer_t          g_os_hw_timers_32b;
extern tmr32b_map_t *soc_timer_module_32b;
static os_timer_t_  *hw_timers_arr_32b[NUM_OF_HW_TIMERS_32b] = {};
#endif

/**************************************************************************//*
 @Function      osHwTimerInitialize

 @Description   Initializes the hardware timers module.
 
                There are 4 groups, each one of 4 16-bit timers.
                SDOS uses those timers in cascaded mode, so there is
                2 32-bit timers per core (there are also two 32-bit in platform)
 
                Each timer could be programmed separately - one shot, edge or
                level, etc. The driver does not support all the different
                configurations.
 
 @Return        OS status

 @Cautions      Must be called only once. (usually called by osInitialize())
 
                Multithread: Safe
 

*//***************************************************************************/
os_status osHwTimerInitialize()
{
 
#ifdef ARCH_SUPPORTS_TIMER16B
    g_os_hw_timers_16b.array = hw_timers_arr_16b;
    soc_timer_module_16b = &g_dsp_ccsr_map->timer_module[0];
#endif
 
#ifdef ARCH_SUPPORTS_TIMER32B
    g_os_hw_timers_32b.array = hw_timers_arr_32b;
    soc_timer_module_32b = &g_dsp_ccsr_map->timer_32b_module[0];
#endif
    return OS_SUCCESS;
}


