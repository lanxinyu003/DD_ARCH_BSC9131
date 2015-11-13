/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/19 18:00:34 $
 $Id: psc9x3x_general_hwi_init.c,v 1.8 2013/02/19 18:00:34 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_general_hwi_init.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_general_hwi_init.c

 @Description   Ored general Hardware Interrupts initialization functions.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_init.h"

#include "psc9x3x_hwi.h"
#include "hwi_generic_.h"
#include "starcore_hwi_.h"
#include "psc9x3x_memmap.h"



#if defined(PSC9132) || defined(PSC9131)
#include "psc913x_general_hwi_.h"
#endif

extern os_hwi_arg g_general_hwi_arg;

//void osHwiGeneralIsr(os_hwi_arg arg)
 void osHwiGeneralIsr(void)
{
    uint32_t gir_val, gier_val;
    //uint32_t  start_index = arg;
    uint32_t  start_index = g_general_hwi_arg;
    uint32_t  i;
    general_interrupt_t *general_interrupt = (general_interrupt_t *)g_general_hwi_arg;
 
    for (i=0; general_interrupt[i].mask != 0; i++)
    {
        gier_val  = GET_UINT32(*(general_interrupt[i].gier_addr + osGetCoreID()));
        gir_val   = GET_UINT32(*general_interrupt[i].gir_addr);

        if((gir_val & gier_val) & general_interrupt[i].mask)
        {
            //general_interrupt[i].callback(general_interrupt[i].arg);
        	general_interrupt[i].callback();
            WRITE_SYNCIO_UINT32(*general_interrupt[i].gir_addr, general_interrupt[i].mask);
            return;
        }
    }
 
    OS_ASSERT; // interrupt does not exist!!!
}

os_status osHwiCreateGeneral(os_hwi_handle     hwi_num,
                             os_hwi_function   handler,
                             os_hwi_arg        argument)
{
    uint32_t gier_val;
    volatile uint32_t *gier_addr = NULL;
    uint32_t i;
    general_interrupt_t *general_interrupt;

    OS_ASSERT_COND(hwi_num >= OS_INT_GENERAL_OFFSET);

    switch (hwi_num)
    {
        case OS_INT_MAPLE_ECC_ERR:
        case OS_INT_DDR_ERR:
        case OS_INT_DMA_ERR:
            general_interrupt = general_ored_interrupt;
            break;
 
        case OS_INT_CLASS_OVR:
        case OS_INT_CLASS_WP:
        case OS_INT_CLASS_ERR:
        case OS_INT_CPRI_GEN_ERR:
            general_interrupt = debug_ored_interrupt;
            break;
        case OS_INT_MAPLE_SYS_ERR:
            general_interrupt = maple_ored_interrupt;
            break;
        default: OS_ASSERT;
    }
 
    for (i=0; general_interrupt[i].mask != 0; i++)
        if (general_interrupt[i].hwi_num == hwi_num)
        {
            general_interrupt[i].callback = handler;
            general_interrupt[i].arg      = argument;
            gier_addr                     = general_interrupt[i].gier_addr;
            break;
        }

    OS_ASSERT_COND(i < MAX_ORED_INTERRUPTS);
    OS_ASSERT_COND(gier_addr != NULL);
 
    osHwiSwiftDisable();
    gier_val = GET_UINT32(*(gier_addr + osGetCoreID()));
    WRITE_UINT32(*(gier_addr + osGetCoreID()), (gier_val| general_interrupt[i].mask));

    /* clean interrupt */
    WRITE_SYNCIO_UINT32(*general_interrupt[i].gir_addr,general_interrupt[i].mask);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}


