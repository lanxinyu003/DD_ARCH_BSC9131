/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/04/06 11:32:29 $
 $Id: hw_timers32_init.c,v 1.17 2014/04/06 11:32:29 b41932 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/hw_timers32_init.c,v $
 $Revision: 1.17 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_timers_init.c

 @Description   Hardware timers initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_rm_.h"
#include "smartdsp_os_device.h"
#include "starcore_utils_.h"
#include "hw_timers32_init.h"
#include "hw_timers32.h"
#include "hw_timers_.h"
#include "hw_timers32_shared_.h"
#include "hw_timers32_.h"

#include "os_l1_defense.h"

hw_timer32_t          g_os_hw_timers32; // each core has its own view of the timers.
                                      // This is multicore safe because
                                      // timers are not assigned according to this variable.
extern tmr32b_map_t *soc_timer32_module;
static hw_timer32_t_  *hw_timers32_arr[NUM_OF_HW_TIMERS_32b] = {};
extern os_spinlock_handle g_os_hw_timers32_guard;
extern quad_event_spinlock_t timer32_quad_events[NUM_OF_HW_TIMER_32b_MODULES];
/*****************************************************************************/
os_status hwTimer32Initialize()
{
    uint32_t i;
    uint64_t tmp = (1 << NUM_OF_HW_TIMERS_32b_PER_MODULE);
    uint32_t l1d_mode = osL1dGetResetMode();
 
    if((l1d_mode == OS_L1D_MODE_1) || (l1d_mode == OS_L1D_MODE_2))
    {
    	/* free timers owned by reset core */
		for(i=0; i<NUM_OF_HW_TIMERS_32b; i++)
		{
			if(g_os_hw_timers32_l1d_data.timer32_core_id[i] == osGetCoreID())
			{
				os_status status = hwTimer32Delete((soc_timer32_num_t)i);
				OS_ASSERT_COND(status == OS_SUCCESS);
			}
		}
		if ((l1d_mode == OS_L1D_MODE_2)&&(osGetCoreID()==osGetMasterCore()))
		{
			for (i=0;i<NUM_OF_HW_TIMER_32b_MODULES;i++)
			{
				timer32_quad_events[i].num_of_timers=0;
#if NUM_OF_HW_TIMERS_32b_PER_MODULE < 9
				timer32_quad_events[i].all=0xffffffffffffffff;
#else
#error//find another solution for defining this union
#endif //NUM_OF_HW_TIMERS_32b_PER_MODULE < 9
			}
		}
	
    }
    else
    {
    	if(osL1dIsEnabled())
    	{
    		for(i=0; i<NUM_OF_HW_TIMERS_32b; i++)
				g_os_hw_timers32_l1d_data.timer32_core_id[i] = NO_CORE_ID;
    	}
 
        for (i=0; i<NUM_OF_HW_TIMER_32b_MODULES; i++)
            g_os_hw_timers32_free[i] = (uint32_t)(tmp-1);
 
    }
 
    if ((osGetCoreID() == osGetMasterCore()) && (l1d_mode != OS_L1D_MODE_1))
    {
        osSpinLockInitialize(&g_os_hw_timers32_guard);
    }
 
    g_os_hw_timers32.array = hw_timers32_arr;

#if defined(B4860_FAMILY)
    soc_timer32_module = &DEVICE_REGISTERS->timer32_memmap[0];
#else
    soc_timer32_module = &g_dsp_ccsr_map->timer_32b_module[0];
#endif
 
    return OS_SUCCESS;
}

#if !defined(B4860_FAMILY)

/*****************************************************************************/
os_status       hwTimer32ModuleInitialize(timer32_module_init_params_t    *init_params)
{
    uint8_t module         = init_params->module_num;
    uint8_t timer_num      = NUM_OF_HW_TIMERS_32b_PER_MODULE*module;
    uint8_t last_timer_num = (NUM_OF_HW_TIMERS_32b_PER_MODULE*(1+module))-1;
    os_status status;
    hw_timer32_t        *timers           = NULL;
    uint32_t i;
    uint32_t timer_interrupt;

    OS_ASSERT_COND(module < NUM_OF_HW_TIMER_32b_MODULES);

    timers           = &g_os_hw_timers32;

    /* ALL run this code */
    /* each has it's own view of timers */
    for (i=0; i<NUM_OF_HW_TIMERS_32b_PER_MODULE; i++)
    {
        timer_num = (uint8_t) ((NUM_OF_HW_TIMERS_32b_PER_MODULE*module)+i);
        status = hwTimer32PutInList((void **)&timers->array[timer_num]);
        if (status != OS_SUCCESS)
            RETURN_ERROR(status);
 
        hwTimer32InterruptGet(timer_num, &timer_interrupt);
        timers->array[timer_num]->timer =  timer_interrupt;
    }
 
    if (osGetCoreID() == osGetMasterCore())
    {
        /* select trigger */
        for(i=0; i<TIMER32_NUM_OF_TRIGGERS_PER_MODULE; i++)
        {
            status = hwTimer32TriggerInputSelect(module, init_params->trigger_params[i].trigger_input, init_params->trigger_params[i].trigger_type);
            if (status != OS_SUCCESS)
                RETURN_ERROR(OS_ERR_HW_TM_UNAVAILABLE);
        }
        status = hwTimer32InputClkSelect(module, init_params->clock_in);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }
    return OS_SUCCESS;

}
#endif




