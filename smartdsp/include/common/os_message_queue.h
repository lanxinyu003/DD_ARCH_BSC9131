/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_message_queue.h,v 1.14 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_message_queue.h,v $
 $Revision: 1.14 $
******************************************************************************/

/**************************************************************************//**

 @File          os_message_queue.h

 @Description   OS Intercore Messages header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MESSAGE_QUEUE_H
#define __OS_MESSAGE_QUEUE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#if (OS_MULTICORE == 1)


/**************************************************************************//**
 @Group         inter_msg_id Intercore Messages

 @Description   Intercore Messages API.
 
                This module is available only on multicore systems.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         inter_msg_queue_id Intercore Messages

 @Description   Intercore Message Queues API.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osMessageQueueInitialize

 @Description   Initializes the intercore message queue. Must be called before using message queue.
 
 @Param[in]     num_of_message_queues       -  Number of message queues to initizali.
 
 @Retval        OS_SUCCESS                  - The message queues were successfully initilized.
 @Retval        OS_ERR_NO_MEMORY            - memory allocation for message queue structure failed.
*//***************************************************************************/
os_status osMessageQueueInitialize(uint16_t num_of_message_queues);

/**************************************************************************//**
 @Function      osMessageQueueCreate

 @Description   Installs an intercore message handler for the calling core.
 
                Enables the calling core to be notified when the specified
                message arrives. The calling core can then handle the message.
 
 @Param[in]     msg_num      -  The intercore message number.
 @Param[in]     priority     -  Priority of interrupt generated at post
 @Param[in]     hwi_arg      -  The argument to use when msg_handler is called
 @Param[in]     msg_handler  -  The function to call when the message arrives.
 @Param[in]     queue_length -  Number of entries in the queue
 @Param[in]     item_size    -  Size of each entry in the queue

 @Retval        OS_SUCCESS                  - The message handler was successfully installed.
 @Retval        OS_ERR_MSG_INVALID          - Invalid message number.
 @Retval        OS_ERR_MSG_FUNCTION_INVALID - NULL message handler.
 @Retval        OS_ERR_MSG_ALREADY_CREATED  - The message number is already created.
*//***************************************************************************/
os_status   osMessageQueueCreate(os_msg_handle        msg_num,
                                 os_hwi_priority      priority,
                                 os_hwi_arg           hwi_arg,
                                 os_hwi_function      msg_handler,
                                 uint16_t             queue_length,
                                 uint16_t             item_size);


/**************************************************************************//**
 @Function      osMessageQueueHwiGet

 @Description   Retrive interrupt information of message queue
 
 @Param[in]     msg_num -  The intercore message number.
 @Param[out]    hwi_num -  Interrupt number assotiated with message queue
 
 @Retval        OS_SUCCESS  - The message handler is linked to interrupt.
 @Retval        OS_FAIL     - Invalid message num.
*//***************************************************************************/
os_status osMessageQueueHwiGet(os_msg_handle msg_num, os_hwi_handle* hwi_num);
 
/**************************************************************************//**
 @Function      osMessageQueuePost

 @Description   Posts the given message number with the given data to the
                destination core.
 
 @Param[in]     msg_num     -  The intercore message number.
 @Param[in]     msg_data    -  The data to include in the message.

 @Retval        OS_SUCCESS              - The message was successfully posted.
 @Retval        OS_ERR_MSG_INVALID      - Invalid message number.
 @Retval        OS_ERR_MSG_DEST_INVALID - Invalid destination.
 @Retval        OS_ERR_MSG_BUSY         - Destination is locked for posting.
*//***************************************************************************/
os_status osMessageQueuePost(os_msg_handle   msg_num, uint32_t *msg_data);


/**************************************************************************//**
 @Function      osMessageQueueGet

 @Description   Retrieves the data from a message that was posted to the
                calling core.
 
                This function must be called by the message handler.
 
 @Param[in]     msg_num     -  The intercore message number.
 @Param[out]    msg_data    -  The intercore message.
 
 @Return        OS_SUCCESS if succeeded.
*//***************************************************************************/
os_status osMessageQueueGet(os_msg_handle msg_num, uint32_t* msg_data);


/**************************************************************************//**
 @Function      osMessageQueueDispatcher

 @Description   Retrieves the data from a message that was posted to the
                calling core.
 
                This function must be called by the message handler.
 
 @Param[in]     msg_num     -  The intercore message number.
 @Param[in]     handler     -  Pointer to message handler.
 @Param[in]     hwi_arg     -  Message handler argument
 @Param[out]    msg_data    -  The intercore message.
 
 @Return       status of message processing
*//***************************************************************************/
os_status osMessageQueueDispatcher(os_msg_handle msg_num, os_msg_function handler, os_hwi_arg hwi_arg,uint32_t* msg_data);

/*****************************************************************************/

/** @} */ // inter_msg_queue_id
/** @} */ // inter_msg_id

#endif // OS_MULTICORE

_OS_END_EXTERN_C

#endif // __OS_MESSAGE_H

