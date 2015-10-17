/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_message_queue.c,v 1.3 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_message_queue.c,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************

 @File          os_message_queues.c

 @Description   Intercore messages runtime functions.
 
                In this file messages functions are implemented.The basic idea
                is to connect a shared 4 bytes of data with a virtual interrupt
                to enable one core pass a message to another. While the message
                is posted, no other core can post a message on it so the data
                is not changed until the destination core fetches the message.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_rm_.h"
#include "os_queue_.h"
#include "os_message_queue_.h"
#include "os_message_queue_shared_.h"

#if (OS_MULTICORE == ON)

extern os_message_queue_t* g_message_queue_list;
extern uint16_t g_max_os_message_queue;

/*****************************************************************************/
os_status osMessageQueuePost(os_msg_handle   msg_num,
                             uint32_t*       msg_data)
{
    os_status status;
 
#ifdef MESSAGE_QUEUE_ERROR_CHECKING
    if (msg_num >= g_max_os_message_queue)
    {
#ifdef MESSAGE_QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
#endif

    status = osQueueEnqueueMultiple(g_message_queue_list[msg_num].queue, msg_data);
    if(status != OS_SUCCESS) return status;
 
    /* generate VIRT interrupt */
    status = messageQueueNotify(msg_num);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    return status;
}



/*****************************************************************************/

os_status osMessageQueueGet(os_msg_handle msg_num, uint32_t* msg_data)
{
    os_status status;

#ifdef MESSAGE_QUEUE_ERROR_CHECKING
    if (msg_num >= g_max_os_message_queue)
    {
#ifdef MESSAGE_QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* MESSAGE_ERROR_ASSERT */
        return OS_ERR_MSG_INVALID;
    }
#endif
 
    status = osQueueDequeueMultiple(g_message_queue_list[msg_num].queue, msg_data);

    return status;
}

/*****************************************************************************/
os_status osMessageQueueDispatcher(os_msg_handle msg_num, os_msg_function handler, os_hwi_arg hwi_arg,uint32_t* msg_data)
{
    os_status status;
 
    OS_ASSERT_COND(handler != NULL);
 
    /* Try to get message */
    status = osMessageQueueGet(msg_num,msg_data);
    if(status != OS_SUCCESS) return status;
 
    /* call user handler */
    return handler(hwi_arg,msg_data);
}

/*****************************************************************************/

#endif // (OS_MULTICORE == ON)



