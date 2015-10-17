/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_swi_init.c,v 1.25 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_swi_init.c,v $
 $Revision: 1.25 $
******************************************************************************/

/******************************************************************************
 
 @File          os_swi_init.c

 @Description   OS Software Interrrupts initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_.h"
#include "os_swi_.h"
#include "os_readylist_.h"
#include "os_rm_.h"


os_swi_struct               swi_data;
os_ready_list               swi_ready;

volatile uint16_t           g_max_swi;
volatile os_swi_priority    g_swi_priority_level = OS_SWI_PRIORITY_ALL;
volatile os_swi_struct_     *g_active_swi_ptr = NULL;

extern volatile uint16_t    g_os_flags;


/*****************************************************************************/
os_status osSwiInitialize(uint16_t max_swi)
{
    os_status   status;
    uint16_t    i;

    g_max_swi = max_swi;

    swi_data.list = (os_swi_struct_ *)
        osMalloc((sizeof(os_swi_struct_) * max_swi), OS_MEM_LOCAL);

    if (swi_data.list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    /* Clear elements in the data structure */
    for (i=0; i < g_max_swi; i++)
    {
        swi_data.list[i].handler    = NULL;
        swi_data.list[i].status     = OS_SWI_UNUSED;
        swi_data.list[i].priority   = OS_SWI_PRIORITY_NONE;
        swi_data.list[i].user_id    = 0;
        swi_data.list[i].count      = 0;
    }

    /* Initialize the global flags. SWI handling should be enabled by
       default and no pending SWI to be serviced. */
    g_os_flags |= OS_FLG_SWI_ENABLE;
    g_os_flags &= ~OS_FLG_SWI_REQ;

    /* Initialize active SWI variable to NULL */
    g_active_swi_ptr = NULL;

    /* Initialize SWI ready list */
    status = osReadyListInitialize(&swi_ready, g_max_swi);

    return status;
}


/*****************************************************************************/
os_status osSwiCreate(os_swi_function   handler,
                      os_swi_handle     swi_num,
                      os_swi_priority   priority,
                      os_user_id        user_id)
{
    os_hwi_status int_status;
#ifdef MMU_SUPPORT
#if defined(SC3X00) || defined(SC3X50)
    os_mmu_context_handle context;
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
    if (priority > OS_SWI_PRIORITY15)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_PRIORITY_INVALID);
    }
#ifndef MMU_SUPPORT    //It is possible with MMU enabled
    if (handler == NULL)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_SWI_FUNCTION_INVALID);
    }
#endif
#endif /* SWI_ERROR_CHECKING */

    int_status = osHwiDisable();
 
#ifdef SWI_ERROR_CHECKING
    if (swi_data.list[swi_num].status >= OS_RESOURCE_USED)
    {
#ifdef SWI_ERROR_ASSERT
        OS_ASSERT;
#endif /* SWI_ERROR_ASSERT */
        osHwiEnable(int_status);
        RETURN_ERROR(OS_ERR_SWI_ALREADY_CREATED);
    }
#endif /* SWI_ERROR_CHECKING */

    swi_data.list[swi_num].status   = OS_RESOURCE_USED;
    swi_data.list[swi_num].handler  = handler;
    swi_data.list[swi_num].priority = priority;
    swi_data.list[swi_num].user_id  = user_id;
    swi_data.list[swi_num].count    = 0;

#if defined(SC3X00) || defined(SC3X50)
#ifdef MMU_SUPPORT
    osMmuDataContextFindExisting(&context, OS_MMU_SYSTEM_CONTEXT);
    osSwiMmuDataContextSet(swi_num, context);

    osMmuProgContextFindExisting(&context, OS_MMU_SYSTEM_CONTEXT);
    osSwiMmuProgContextSet(swi_num, context);
#endif
#endif

#ifdef SWI_LOGGING
    osLog((uint32_t)LOG_SWI_CREATE, (uint32_t)(priority << 16) | swi_num);
#endif /* SWI_LOGGING */

    osHwiEnable(int_status);

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osSwiDelete(os_swi_handle swi_num)
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

    if (swi_data.list[swi_num].status == OS_RESOURCE_ACTIVE)
    {
        status = osReadyListRemove(&swi_ready, swi_num);
    }

    swi_data.list[swi_num].handler  = NULL;
    swi_data.list[swi_num].status   = OS_RESOURCE_DELETED;
    swi_data.list[swi_num].priority = OS_SWI_PRIORITY_NONE;
    swi_data.list[swi_num].user_id  = 0;
    swi_data.list[swi_num].count    = 0;

    osHwiSwiftEnable();

    return status;
}


#if defined(SC3X00) || defined(SC3X50)
#ifdef MMU_SUPPORT


void osSwiMmuProgContextSet(os_swi_handle  swi_num, os_mmu_context_t* context)
{
    swi_data.list[swi_num].os_swi_mmu_prog_context = context;
}

void osSwiMmuDataContextSet(os_swi_handle  swi_num, os_mmu_context_t* context)
{
    swi_data.list[swi_num].os_swi_mmu_data_context = context;
}

os_mmu_context_t const * osSwiMmuDataContextGet(os_swi_handle swi_num)
{
    return swi_data.list[swi_num].os_swi_mmu_data_context;
}
os_mmu_context_t const * osSwiMmuProgContextGet(os_swi_handle swi_num)
{
    return swi_data.list[swi_num].os_swi_mmu_prog_context;
}

#endif
#endif





