/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: os_message_queue_init.c,v 1.4 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_message_queue_init.c,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************

 @File          os_message_queue_init.c

 @Description   Intercore messages initialization functions.
 
                In this file messages functions are implemented. The basic idea
                is to connect a shared queue of data with a virtual interrupt
                to enable one core pass a message to another.
                The message queue supports both unicast and anycast types of messaging
 

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_rm_.h"
#include "os_message_queue_.h"
#include "os_message_queue_shared_.h"
#include "os_l1_defense.h"

#if (OS_MULTICORE == ON)

uint16_t g_max_os_message_queue;
os_message_queue_t* g_message_queue_list;



/*****************************************************************************/

os_status osMessageQueueInitialize(uint16_t num_of_message_queues)
{
    os_msg_handle   msg_cnt;
    uint32_t        manager_mask = 1;
	uint32_t        reset_mode = osL1dGetResetMode();
 
    OS_ASSERT_COND(num_of_message_queues > 0);
 
    g_max_os_message_queue = num_of_message_queues;
 
    if((reset_mode == OS_L1D_NO_WARM_RESET) || (reset_mode == OS_L1D_MODE_3))
    {
        if (osGetCoreID() == osGetMasterCore())
        {
            os_status status;
 
            g_message_queue_list_shared = (volatile os_queue_t **)
                osMalloc((uint16_t)(sizeof(os_queue_t *) * num_of_message_queues), OS_MEM_SHARED);
 
            if (g_message_queue_list_shared == NULL)
            {
                RETURN_ERROR(OS_ERR_NO_MEMORY);
            }
 
            status = messageQueueHwiGet(g_max_os_message_queue);
            OS_ASSERT_COND(status == OS_SUCCESS);
        }
 
        /* wait until allocation */
        osWaitForAllCores();
    }

    g_message_queue_list = (os_message_queue_t *)
        osMalloc((uint16_t)(sizeof(os_message_queue_t) * num_of_message_queues), OS_MEM_LOCAL);

    if (g_message_queue_list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    for (msg_cnt = 0; msg_cnt < num_of_message_queues; msg_cnt++)
    {
        g_message_queue_list[msg_cnt].status = OS_RESOURCE_UNUSED;
        g_message_queue_list[msg_cnt].queue = NULL;
    }

    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osMessageQueueHwiGet(os_msg_handle msg_num,os_hwi_handle* hwi_num)
{
    OS_ASSERT_COND(hwi_num != NULL);

#ifdef MESSAGE_QUEUE_ERROR_CHECKING
    if (msg_num >= g_max_os_message_queue)
    {
#ifdef MESSAGE_QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
#endif
 
    if(g_message_queue_list[msg_num].hwi_num == 0) return OS_FAIL;
 
    *hwi_num = g_message_queue_list[msg_num].hwi_num;
 
    return OS_SUCCESS;
}



/*****************************************************************************/
os_status   osMessageQueueCreate(os_msg_handle        msg_num,
                                 os_hwi_priority      priority,
                                 os_hwi_arg           hwi_arg,
                                 os_hwi_function      msg_handler,
                                 uint16_t             queue_length,
                                 uint16_t             item_size)
{
    os_status status = OS_SUCCESS;

#ifdef MESSAGE_QUEUE_ERROR_CHECKING
    if (msg_num >= g_max_os_message_queue)
    {
#ifdef MESSAGE_QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
    if (msg_handler == NULL && priority != 0)
    {
#ifdef MESSAGE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_FUNCTION_INVALID;
    }
#endif /* MESSAGE_QUEUE_ERROR_CHECKING */
 
    osHwiSwiftDisable();
 
    /* Output message core will try to create a queue */
    /* If a queue is still not created                */
    osSpinLockGet(&shared_queue_message_guard);
    if(g_message_queue_list_shared[msg_num] == NULL)
    {
        os_queue_handle que_handle;
        status = osQueueFind(&que_handle, TRUE);
        OS_ASSERT_COND(status == OS_SUCCESS)
 
        /* Plus 1 to support message ID */
        status = osQueueCreateMultiple(que_handle, queue_length, item_size);
        OS_ASSERT_COND(status == OS_SUCCESS)
 
        g_message_queue_list_shared[msg_num] = que_handle;
        g_message_queue_list[msg_num].queue  = que_handle;
    }
    else
    {
        g_message_queue_list[msg_num].queue = (os_queue_t*)g_message_queue_list_shared[msg_num];
    }
    g_message_queue_list[msg_num].message_size = item_size;
 
    osSpinLockRelease(&shared_queue_message_guard);
 
    g_message_queue_list[msg_num].hwi_num = 0;
 
    osHwiSwiftEnable();

    /* attach to interrupt handler */
    if(priority != 0)
        g_message_queue_list[msg_num].hwi_num = messageQueueHwiCreate(msg_num, priority, hwi_arg, msg_handler);

    return status;
}




#endif // (OS_MULTICORE == ON)

