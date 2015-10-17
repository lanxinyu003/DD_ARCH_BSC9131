/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_event_queue.h,v 1.16 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_event_queue.h,v $
 $Revision: 1.16 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_event_queue.h

 @Description   OS event queue definitions and prototypes.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_EVENT_QUEUE_H
#define __OS_EVENT_QUEUE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         event_q_id Event Queues API

 @Description   Queue Events setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         event_q_runtime_id Event Queues RunTime functions
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osEventQueueFind

 @Description   Get a handle for an event.
 
                Get a handle that can be used to create a queue event.

 @Param[out]    event_handle - Handle to an event queue structure.

 @Retval        OS_SUCCESS                      : Found.
 @Retval        OS_ERR_Q_UNAVAILABLE            : No more queues in the system.
 @Retval        OS_ERR_EVENT_QUEUE_UNAVAILABLE  : No more event queues in the system.
*//***************************************************************************/
os_status osEventQueueFind(os_event_handle *event_handle);


/**************************************************************************//**
 @Function      osEventQueueCreate

 @Description   Creates a queue event.
                Initializes a queue and an empty waiting list. i.e - find  a queue
                using osQueueFind(), creates it using osQueueCreate(). Then
                places a handle to this queue and to the empty waiting list in an
                event structure.

 @Param[in]     event_handle    - Handle to an event queue structure.
 @Param[in]     size            - Queue size.

 @Retval        OS_SUCCESS                          : Created.
 @Retval        OS_ERR_EVENT_QUEUE_ALREADY_CREATED  : Not Created.
 @Retval        Error status, encoded in os_error.h, for other errors.
*//***************************************************************************/
os_status osEventQueueCreate(os_event_handle event_handle, uint16_t size);


/**************************************************************************//**
 @Function      osEventQueuePend

 @Description   Pend on an event queue.
 
                The calling task will block until a message will be posted to the
                queue or until its timeout shall expire.
 
 @Param[in]     event_handle    - Handle to an event structure.
 @Param[out]    message         - Recieved message.
 @Param[in]     timeout         - Timeout to pend in ticks.

 @Retval        OS_SUCCESS                      : Got the resource.
 @Retval        OS_ERR_EVENT_QUEUE_PEND_INTERR  : Cannot block in interrupt.
 @Retval        OS_ERR_EVENT_PEND_IN_LOCK       : Can not block a running locked task. will cause a crash.
 @Retval        OS_ERR_EVENT_QUEUE_TIMEOUT      : Timeout expired..

 @Cautions       background task can not call this function.
 @Cautions       SWI and HWI can not call this function.
*//***************************************************************************/
os_status osEventQueuePend(os_event_handle event_handle, uint32_t *message, uint32_t timeout);


/**************************************************************************//**
 @Function      osEventQueueAccept

 @Description   Try to get A message from a queue without pending.
 
                This function checks if there is a message available in the queue
                and if there is returns it to the caller. Otherwise return a status
                indicating that queue is empty. This is a non blocking function so
                it can be used in interrupt and software interrupt contexts.
 
 @Param[in]     event_handle    - Handle to an event structure.
 @Param[out]    message         - Recieved message.

 @Retval        OS_SUCCESS              : Got the resource.
 @Retval        OS_ERR_Q_NOT_CREATED    : Queue is not created.
 @Retval        OS_ERR_Q_EMPTY          : No message in the queue.
*//***************************************************************************/
os_status osEventQueueAccept(os_event_handle event_handle, uint32_t *message);


/**************************************************************************//**
 @Function      osEventQueuePost

 @Description   Posts a message to a queue.

                If tasks are waiting in the waiting list of the queue, wake the
                first one and give it the message, otherwise queue the message
                for tasks that will pend or accept it later.

 @Param[in]     event_handle    - Handle to an event structure.
 @Param[in]     message         - Message to post.
 @Param[out]    resumed_task    - Task that was resumed or NULL.

 @Retval        OS_SUCCESS              : Got the ersource.
 @Retval        OS_ERR_Q_NOT_CREATED    : Queue was not created.
 @Retval        OS_EVENT_QUEUE_OVERFLOW : Queue is full.
*//***************************************************************************/
os_status osEventQueuePost(os_event_handle event_handle, uint32_t message, os_task_handle *resumed_task);

/**************************************************************************//**
 @Function      osEventQueueReset

 @Description   Release event pending tasks with posted message and
                empty queue of messages if there are no pending tasks.
 
 @Param[in]     event_handle    - Handle to an event structure.
 @Param[in]     message         - Message to post.

 @Retval        OS_SUCCESS                  : Got the event queue was reset.
 @Retval        OS_ERR_EVENT_QUEUE_INVALID  : event_handle is not event queue.
*//***************************************************************************/
os_status osEventQueueReset(os_event_handle event_handle,  uint32_t message);

/** @} */ // event_q_runtime_id
/** @} */ // event_q_id

_OS_END_EXTERN_C

#endif __OS_EVENT_QUEUE_H
