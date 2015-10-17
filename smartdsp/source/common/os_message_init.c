/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_message_init.c,v 1.5 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_message_init.c,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          os_messages_init.c

 @Description   os intercore messages initialization functions.
 
                In this file messages functions are implemented. The basic idea
                is to connect a shared 4 bytes of data with a virtual interrupt
                to enable one core pass a message to another. While the message
                is posted, no other core can post a message on it so the data
                is not changed until the destination core fetches the message.

 @Cautions      Verification regarding message availability will be done always
                on the message from master core (osGetMasterCore()); as all to-from
                pairs are created and deleted together.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_rm_.h"
#include "os_message_shared_.h"


#if (OS_MULTICORE == ON)

volatile uint16_t         g_max_os_messages;
volatile soc_message_t    *shared_messages __attribute__ ((section (".os_shared_data_bss")));

#define MSG_INT_NUM(src_core, msg_num) (OS_INT_FROM_CORE0_0 /*TBD*/ + (OS_SOC_MAX_NUM_OF_MESSAGES * src_core) + msg_num)

/*****************************************************************************/
os_status osMessageInitialize(uint16_t num_of_messages)
{
    os_msg_handle   msg_cnt;
    uint32_t        i, j;
 
 
#ifdef MESSAGE_ERROR_CHECKING
    if (num_of_messages > OS_SOC_MAX_NUM_OF_MESSAGES)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_NUM_LIMIT;
    }
#endif /* MESSAGE_ERROR_CHECKING */

    // There is a single message from core to core
    g_max_os_messages = num_of_messages;
 
    if (osGetCoreID() == osGetMasterCore())
    {
        shared_messages = (soc_message_t *)
            osMalloc((sizeof(soc_message_t) * num_of_messages), OS_MEM_SHARED);
 
        if (shared_messages == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
 
        for (msg_cnt = 0; msg_cnt < num_of_messages; msg_cnt++)
        {
            for ( i=0; i < OS_SOC_MAX_NUM_OF_CORES; i++ )
            {
                for ( j=0; j < OS_SOC_MAX_NUM_OF_CORES; j++ )
                {
                    shared_messages[msg_cnt].messages[i][j].status = OS_RESOURCE_UNUSED;
                    shared_messages[msg_cnt].messages[i][j].data = 0;
                    osSpinLockInitialize((os_spinlock_handle *)&shared_messages[msg_cnt].messages[i][j].guard);
                }
            }
        }
    }
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osMessageCreate(os_msg_handle     msg_num,
                          os_hwi_function   msg_handler)
{
    os_status status;
    uint16_t src_core;
 
 
#ifdef MESSAGE_ERROR_CHECKING
    if (msg_num >= g_max_os_messages)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
    if (msg_handler == NULL)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_FUNCTION_INVALID;
    }
#endif /* MESSAGE_ERROR_CHECKING */
 
    osHwiSwiftDisable();
 
#ifdef MESSAGE_ERROR_CHECKING
    if (shared_messages[msg_num].messages[osGetMasterCore()][osGetCoreID()].status >= OS_RESOURCE_USED)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        osHwiSwiftEnable();
        return OS_ERR_MSG_ALREADY_CREATED;
    }
#endif /* MESSAGE_ERROR_CHECKING */

    for (src_core=0 ; src_core<OS_SOC_MAX_NUM_OF_CORES ; src_core++)
    {
        status = osHwiCreate((os_hwi_handle)MSG_INT_NUM(src_core, msg_num),
                             (os_hwi_priority)OS_HWI_PRIORITY0,
                             (os_hwi_mode)EDGE_MODE,
                             msg_handler,
                             (os_hwi_arg)CREATE_MSG_NUM(msg_num,src_core));
 
        if (status == OS_SUCCESS)
        {
            shared_messages[msg_num].messages[src_core][osGetCoreID()].status = OS_RESOURCE_USED;
            shared_messages[msg_num].messages[src_core][osGetCoreID()].type = OS_MESSAGE_SYNC_TYPE;
        }
        else
        {
            OS_ASSERT;
            break;
        }
    }
 

    osHwiSwiftEnable();

    return status;
}

/*****************************************************************************/
os_status   osMessageCreateAsync(os_msg_handle   msg_num,
                                 os_hwi_function msg_handler,
                                 uint32_t        msg_data,
                                 os_hwi_priority v_p)
{
    os_status status;
    uint16_t src_core;
 
 
#ifdef MESSAGE_ERROR_CHECKING
    if (msg_num >= g_max_os_messages)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
    if (msg_handler == NULL)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_FUNCTION_INVALID;
    }
 
#endif /* MESSAGE_ERROR_CHECKING */

    osHwiSwiftDisable();
 
#ifdef MESSAGE_ERROR_CHECKING
    if (shared_messages[msg_num].messages[osGetMasterCore()][osGetCoreID()].status >= OS_RESOURCE_USED)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        osHwiSwiftEnable();
        return OS_ERR_MSG_ALREADY_CREATED;
    }
#endif /* MESSAGE_ERROR_CHECKING */


    for (src_core=0 ; src_core<OS_SOC_MAX_NUM_OF_CORES ; src_core++)
    {
        status = osHwiCreate((os_hwi_handle)MSG_INT_NUM(src_core, msg_num),
                             v_p,
                             (os_hwi_mode)EDGE_MODE,
                             msg_handler,
                             (os_hwi_arg)msg_data);
 
        if (status == OS_SUCCESS)
        {
            osSpinLockInitialize((os_spinlock_handle *)&shared_messages[msg_num].messages[src_core][osGetCoreID()].guard);
            shared_messages[msg_num].messages[src_core][osGetCoreID()].status = OS_RESOURCE_USED;
            shared_messages[msg_num].messages[src_core][osGetCoreID()].type = OS_MESSAGE_ASYNC_TYPE;
        }
        else
        {
            OS_ASSERT;
            break;
        }
    }

 
    osHwiSwiftEnable();

    return status;
}



/*****************************************************************************/
os_status osMessageFind(os_msg_handle *msg_num)
{
    os_status       status = OS_ERR_MSG_UNAVAILABLE;
    os_msg_handle   msg_cnt, src_core;

    osHwiSwiftDisable();

    for (msg_cnt = 0; msg_cnt < g_max_os_messages; msg_cnt++)
    {
        if (shared_messages[msg_cnt].messages[osGetMasterCore()][osGetCoreID()].status < OS_RESOURCE_ACQUIRED)
        {
            for (src_core=0 ; src_core<OS_SOC_MAX_NUM_OF_CORES ; src_core++)
            {
                shared_messages[msg_cnt].messages[src_core][osGetCoreID()].status = OS_RESOURCE_ACQUIRED;
            }
 
            *msg_num = msg_cnt;
            status = OS_SUCCESS;
            break;
        }
    }

    osHwiSwiftEnable();

    return status;
}

/*****************************************************************************/
os_status osMessageDelete(os_msg_handle msg_num)
{
    os_status status;
    uint16_t src_core;
 
 
#ifdef MESSAGE_ERROR_CHECKING
    if (msg_num >= g_max_os_messages)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
#endif /* MESSAGE_ERROR_CHECKING */
 

    osHwiSwiftDisable();
 
#ifdef MESSAGE_ERROR_CHECKING
    if (shared_messages[msg_num].messages[osGetMasterCore()][osGetCoreID()].status < OS_RESOURCE_USED)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        osHwiSwiftEnable();
        return OS_ERR_MSG_NOT_CREATED;
    }
#endif /* MESSAGE_ERROR_CHECKING */
 
    for (src_core=0 ; src_core<OS_SOC_MAX_NUM_OF_CORES ; src_core++)
    {
        shared_messages[msg_num].messages[src_core][osGetCoreID()].status = OS_RESOURCE_DELETED;
        status = osHwiDelete((os_hwi_handle)MSG_INT_NUM(src_core, msg_num));
        if (status != OS_SUCCESS)
        {
            OS_ASSERT;
            break;
        }
    }

    osHwiSwiftEnable();

    return status;
}


#endif // (OS_MULTICORE == ON)

