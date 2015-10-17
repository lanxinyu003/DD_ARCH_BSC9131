/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/06/03 07:22:50 $
 $Id: os_swi.c,v 1.34 2014/06/03 07:22:50 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_swi.c,v $
 $Revision: 1.34 $
******************************************************************************/

/******************************************************************************
 
 @File          os_swi.c

 @Description   OS Software Interrrupts runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_.h"
#include "os_swi_.h"
#include "os_readylist_.h"
#include "os_rm_.h"
#ifdef MMU_SUPPORT
#include "os_mmu.h"
#include "os_mmu_.h"
#endif

extern os_swi_struct            swi_data;
extern os_ready_list            swi_ready;

extern volatile uint16_t        g_max_swi;
extern volatile os_swi_priority g_swi_priority_level;
extern volatile os_swi_struct_  *g_active_swi_ptr;

extern volatile uint16_t        g_os_flags;
extern volatile uint16_t        g_os_interrupt_count;




/*****************************************************************************/
void osSwiEnable(os_swi_priority priority)
{
#ifdef SWI_ERROR_CHECKING
    if (priority > OS_SWI_PRIORITY_ALL)
    {
        /* Invalid priority */
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        return;
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();
 
    g_os_flags |= OS_FLG_SWI_ENABLE;
    g_swi_priority_level = priority;

    /* If needed, fire off a TRAP interrupt to handle pending SWIs */
    if ((g_os_interrupt_count == 0) &&
        (g_os_flags & OS_FLG_SWI_REQ))
    {
        osHwiSwiftEnable();
        osSwiTrap();
    }
    else
    {
        osHwiSwiftEnable();
    }
}


/*****************************************************************************/
os_swi_priority osSwiDisable()
{
    os_swi_priority priority;
 
    osHwiSwiftDisable();
 
    g_os_flags &= ~OS_FLG_SWI_ENABLE;
    priority = g_swi_priority_level;
 
    osHwiSwiftEnable();
 
    return priority;
}


/*****************************************************************************/
os_status osSwiActivate(os_swi_handle swi_num)
{
    os_swi_priority                 priority;
    os_status                       status = OS_SUCCESS;
    uint16_t                        swi_osflg_holder;
    volatile struct os_swi_struct_  *swi_ptr_holder;
    uint16_t                        req_swi_number;
    struct os_swi_struct_ * restrict  req_swi_ptr;
 
#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
    os_mmu_context_t prev_data_context;
    os_mmu_context_t prev_prog_context;
#endif
#endif
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */
 
    req_swi_ptr = &(swi_data.list[swi_num]);
    priority = req_swi_ptr->priority;
 
    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (req_swi_ptr->status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

#ifdef SWI_LOGGING
    /* log the activation (except OS_TIMER_SWI) */
    if (swi_num)
    {
        osLog(LOG_SWI_ACTIVATE, swi_num);
    }
#endif /* SWI_LOGGING */

    /* If SWI is not already on ready list, add it */
    if (req_swi_ptr->status != OS_RESOURCE_ACTIVE)
    {
        if (((g_os_flags & (OS_FLG_SWI_ENABLE | OS_FLG_SWI_ACTIVE)) ==
                           (OS_FLG_SWI_ENABLE | OS_FLG_SWI_ACTIVE)) &&
            (g_os_interrupt_count == 0) &&
            (g_swi_priority_level >= priority))
        {
            /* Activate can be called from another SWI or Background. We
               are on the right stack. No need to trap to get to the call. */

            /* If the activated SWI has a higher priority than the
               currently running one, than jump directly into the
               activated one. Otherwise, put the SWI onto the ready
               list. It will be handled in order of priority once the
               current SWI is complete. */
            do
            {
                if ((!(g_os_flags & OS_FLG_SWI_ACTIVE)) ||
                    (g_active_swi_ptr->priority > priority))
                {
                    swi_ptr_holder = g_active_swi_ptr;
 
                    if (swi_ready.ready_cnt == 0)
                    {
                        g_os_flags &= ~OS_FLG_SWI_REQ;
                    }
                    g_active_swi_ptr = req_swi_ptr;

                    swi_osflg_holder = (uint16_t)(g_os_flags & OS_FLG_SWI_ACTIVE);
                    g_os_flags |= OS_FLG_SWI_ACTIVE;
 
 
                    osHwiSwiftEnable();
#ifdef SWI_LOGGING
                osLog(LOG_SWI_ENTER, swi_num);
#endif /* SWI_LOGGING */
#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
                osMmuDataContextActivate((os_mmu_context_t*)req_swi_ptr->os_swi_mmu_data_context,
                &prev_data_context);
                osMmuProgContextActivate((os_mmu_context_t*)req_swi_ptr->os_swi_mmu_prog_context,
                &prev_prog_context);
#endif
#endif
                    (req_swi_ptr->handler)(req_swi_ptr->count);
#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
                osMmuDataContextActivate(&prev_data_context, NULL);
                osMmuProgContextActivate(&prev_prog_context, NULL);
#endif
#endif
 
#ifdef SWI_LOGGING
                osLog(LOG_SWI_EXIT, swi_num);
#endif /* SWI_LOGGING */
                    osHwiSwiftDisable();

                    g_os_flags &= ~OS_FLG_SWI_ACTIVE;
                    g_os_flags |= swi_osflg_holder;

                    g_active_swi_ptr = swi_ptr_holder;
                    if(g_os_flags&(OS_FLG_SWI_REQ))
                    {
                        req_swi_number =  osReadyListPop(&swi_ready,g_active_swi_ptr->priority);
                        if (req_swi_number ==  NO_TASK)
                        {
                            break;
                        }
                        req_swi_ptr = &(swi_data.list[req_swi_number]);
                        req_swi_ptr->status = OS_RESOURCE_USED;
                        priority = req_swi_ptr->priority;
                    }

                }
                else
                {
                    /* Add SWI to ready list */
                    status = osReadyListPush(&swi_ready, swi_num, priority);
 
                    if (status == OS_SUCCESS)
                    {
                        /* Change status of SWI */
                        swi_data.list[swi_num].status = OS_RESOURCE_ACTIVE;
 
                        /* Post request in os flags */
                        g_os_flags |= OS_FLG_SWI_REQ;
                    }
 
                    osHwiSwiftEnable();
                    return status;
                }
            }while(g_os_flags&(OS_FLG_SWI_REQ));
            osHwiSwiftEnable();
            return status;
        }
        else /* HWI active or there is no SWI active */
        {
            /* Add SWI to ready list */
            status = osReadyListPush(&swi_ready, swi_num, priority);

            if (status == OS_SUCCESS)
            {
                /* Change status of SWI */
                req_swi_ptr->status = OS_RESOURCE_ACTIVE;

                /* Post request in os flags */
                g_os_flags |= OS_FLG_SWI_REQ;
            }
        }

        /* Cause a SWI if Activating from outside an HWI */
        if ((g_os_interrupt_count == 0) &&
            ((g_os_flags & (OS_FLG_SWI_ENABLE | OS_FLG_SWI_REQ)) ==
                           (OS_FLG_SWI_ENABLE | OS_FLG_SWI_REQ)))
        {
            osHwiSwiftEnable();
            osSwiTrap();
            return status;
        }
    }

    osHwiSwiftEnable();
    return status;
}


/*****************************************************************************/
os_status osSwiCountInc(os_swi_handle swi_num)
{
    os_status status;
 
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */
 
    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    swi_data.list[swi_num].count++;
    status = osSwiActivate(swi_num);
 
    osHwiSwiftEnable();
 
    return status;
}


/*****************************************************************************/
os_status osSwiCountDec(os_swi_handle swi_num)
{
    os_status status = OS_SUCCESS;
 
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    swi_data.list[swi_num].count--;
    if (swi_data.list[swi_num].count == 0)
    {
        status = osSwiActivate(swi_num);
    }
 
    osHwiSwiftEnable();
 
    return status;
}


/*****************************************************************************/
os_status osSwiCountGet(os_swi_handle swi_num, os_swi_count *count)
{
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    *count = swi_data.list[swi_num].count;

    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osSwiCountSet(os_swi_handle swi_num, os_swi_count count)
{
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    swi_data.list[swi_num].count = count;

    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osSwiPriorityGet(os_swi_handle swi_num, os_swi_priority *priority)
{
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    *priority = swi_data.list[swi_num].priority;

    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osSwiPrioritySet(os_swi_handle swi_num, os_swi_priority priority)
{
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_INVALID);
    }
    if (priority > OS_SWI_PRIORITY15)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_PRIORITY_INVALID);
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status < OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_SWI_NOT_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    swi_data.list[swi_num].priority = priority;
 
#ifdef SWI_LOGGING
    osLog((uint32_t)LOG_SWI_SET_PRIORITY, (uint32_t)(priority << 16) | swi_num);
#endif /* SWI_LOGGING */

    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_swi_status osSwiStatus(os_swi_handle swi_num)
{
    os_swi_status swi_status;
 
#ifdef SWI_ERROR_CHECKING
    if (swi_num >= g_max_swi)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        return OS_SWI_UNUSED;
    }
#endif /* SWI_ERROR_CHECKING */

    osHwiSwiftDisable();

    swi_status = swi_data.list[swi_num].status;

    osHwiSwiftEnable();

    return swi_status;
}


/*****************************************************************************/
os_status osSwiSelf(os_swi_handle *swi_num)
{
    uint32_t offset;

#ifdef SWI_ERROR_CHECKING
    if (g_active_swi_ptr == 0)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_NOT_ACTIVE);
    }
#endif /* SWI_ERROR_CHECKING */

    offset = ((uint32_t) g_active_swi_ptr) - ((uint32_t) &(swi_data.list[0]));
    *swi_num = OS_SWI0;
 
    while (offset != 0)
    {
        (*swi_num)++;
        offset -= sizeof(os_swi_struct_);
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osSwiFind(os_swi_handle *swi_num)
{
#ifdef SWI_DYNAMIC_CONTROL
    uint16_t i;

    /* Slot 0 is reserved for the Tick SWI. */
    /* We add one additional SWI to the number of SWIs */
    /* the user has requested, and start the loop here from 1 */
    /* instead of 0. */
    OS_ASSERT_COMPILER_COND(g_max_swi >= 1);
 
    for (i = 1; i < g_max_swi; i++)
    {
        osHwiSwiftDisable();
 
        if (swi_data.list[i].status < OS_RESOURCE_ACQUIRED)
        {
            *swi_num = i;
            swi_data.list[i].status = OS_RESOURCE_ACQUIRED;
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
 
        osHwiSwiftEnable();
    }
#endif /* SWI_DYNAMIC_CONTROL */

    *swi_num = 0xFFFF;

    RETURN_ERROR(OS_ERR_SWI_UNAVAILABLE);
}







