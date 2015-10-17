/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/10/31 14:53:03 $
 $Id: psc9x3x_virq_init.c,v 1.8 2012/10/31 14:53:03 b17975 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_virq_init.c,v $
 $Revision: 1.8 $
******************************************************************************/

/**************************************************************************//**
 
 @File          psc9x3x_virq_init.c

 @Description   psc9x3x-specific virtual interrupt functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_virq.h"
#include "starcore_utils_.h"
#include "psc9x3x_shared_.h"
#include "psc9x3x_hwi.h"
#include "os_message_queue_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"


/**************************************************************************/
os_status osVirtualInterruptFind(uint32_t * interrupt_handle)
{
    os_status status;
    uint32_t  index;
 
    OS_ASSERT_COND(interrupt_handle != NULL);
    osSpinLockSwiftGet(&shared_gic_guard);
    status = indexedResourceGet(&shared_virq_manager, &index);
    osSpinLockSwiftRelease(&shared_gic_guard);
 
    *interrupt_handle = index + OS_INT_VIRQ0;
    return (status == OS_SUCCESS) ? status : OS_ERR_VIRQ_UNAVAILABLE;
}


/**************************************************************************/
os_status osVirtualInterruptRelease(uint32_t interrupt_handle)
{
    interrupt_handle -= OS_INT_VIRQ0;
    osSpinLockSwiftGet(&shared_gic_guard);
    indexedResourceFree(&shared_virq_manager, interrupt_handle);
    osSpinLockSwiftRelease(&shared_gic_guard);
 
    return OS_SUCCESS;
 
}
/**************************************************************************/
os_status osVirtualInterruptReserve(uint32_t interrupt_handle)
{
    os_status status = OS_SUCCESS;

    interrupt_handle -= OS_INT_VIRQ0;
    osSpinLockSwiftGet(&shared_gic_guard);
    status = indexedResourceForceGet(&shared_virq_manager, interrupt_handle);
    osSpinLockSwiftRelease(&shared_gic_guard);
 
    return status;
 
}
/**************************************************************************/
os_status osVirtualInterruptActivate(uint32_t interrupt_handle)
{
    if(!((OS_INT_VIRQ0 <= interrupt_handle) && (interrupt_handle <= OS_INT_VIRQ15)))
    {
        return OS_FAIL;
    }
    interrupt_handle -= OS_INT_VIRQ0;
    interrupt_handle = ((interrupt_handle & 0x8) << 5) | (interrupt_handle & 0x7);
    WRITE_UINT32(g_dsp_ccsr_map->gic.vigr, interrupt_handle);
    return OS_SUCCESS;
}
/**************************************************************************/
os_status messageQueueNotify(os_msg_handle irq_num)
{
    if(irq_num < MAX_NUM_OF_VIRTS)
        WRITE_UINT32(g_dsp_ccsr_map->gic.vigr, ((irq_num & 0x8) << 5) | (irq_num & 0x7));
 
    return OS_SUCCESS;
}
/**************************************************************************/
os_status messageQueueHwiGet(uint16_t num_of_message_queues)
{
    uint32_t        manager_mask = 1;

    manager_mask <<= num_of_message_queues;
    manager_mask -= 1;
    shared_virq_manager &= ~manager_mask;
 
    return OS_SUCCESS;
}
/**************************************************************************/
os_hwi_handle messageQueueHwiCreate(os_msg_handle msg_num,
                                    os_hwi_priority priority,
                                    os_hwi_arg hwi_arg,
                                    os_hwi_function msg_handler)
{
    os_status status;
    os_hwi_handle hwi_handler;
 
    hwi_handler = (os_hwi_handle)(OS_INT_VIRQ0 + msg_num);
    OS_ASSERT_COND(msg_num < MAX_NUM_OF_VIRTS);
    status = osHwiCreate(hwi_handler, priority, EDGE_MODE, msg_handler, hwi_arg);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    return hwi_handler;
}
