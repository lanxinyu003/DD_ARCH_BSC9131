/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_event_semaphore.h,v 1.17 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_event_semaphore.h,v $
 $Revision: 1.17 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_event_semaphore.h

 @Description   OS event semaphore definitions and prototypes.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_EVENT_SEMAPHORE_H
#define __OS_EVENT_SEMAPHORE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         event_sem_id Event Semaphores API

 @Description   Semaphore Events setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         event_sem_runtime_id Event Semaphores RunTime functions
 
 @{
*//***************************************************************************/



/**************************************************************************//**
 @Function      osEventSemaphoreFind

 @Description   Get a handle for an event.
 
                Get a handle that can be used to create an semaphore event.

 @Param[out]    event_handle - Handle to an event semaphore structure.

 @Retval        OS_SUCCESS                      : Found.
 @Retval        OS_ERR_EVENT_SEMA_UNAVAILABLE   : Not Found.
*//***************************************************************************/
os_status osEventSemaphoreFind(os_event_handle *event_handle);


/**************************************************************************//**
 @Function      osEventSemaphoreCreate

 @Description   Creates a semaphore event.
 
                Initializes a semaphore and an empty waiting list. i.e - initialize
                the semaphore counter, and place it and the empty waiting list
                in an event structure.

 @Param[in]     event_handle    - Handle to an event semaphore structure.
 @Param[in]     count           - initial value of semaphore.

 @Retval        OS_SUCCESS                          : Created.
 @Retval        OS_ERR_EVENT_SEMA_ALREADY_CREATED   : Not Created.
*//***************************************************************************/
os_status osEventSemaphoreCreate(os_event_handle event_handle, uint32_t count);


/**************************************************************************//**
 @Function      osEventSemaphoreDelete

 @Description   Deletes the semaphore event.
 
                Frees the event structure that was used for a counting semaphore,
                and resume all the tasks that were pending on it.

 @Param[in]     event_handle - Handle to a semaphore event.

 @Retval        OS_SUCCESS                  : Deleted.
 @Retval        OS_ERR_EVENT_SEMA_INVALID   : Not a Semaphore event.
*//***************************************************************************/
os_status osEventSemaphoreDelete(os_event_handle event_handle);

/**************************************************************************//**
 @Function      osEventSemaphoreReset

 @Description   reset a semaphore event - release all waiting tasks and set
                semaphore's count.
 
 @Param[in]     event_handle    - Handle to an event semaphore structure.
 @Param[in]     count           - initial value of semaphore.

 @Retval        OS_SUCCESS                  : Reset.
 @Retval        OS_ERR_EVENT_SEMA_INVALID   : Not a Semaphore event.
*//***************************************************************************/
os_status osEventSemaphoreReset(os_event_handle event_handle, uint32_t count);

/**************************************************************************//**
 @Function      osEventSemaphoreCountGet

 @Description   Gets semaphore value.
 
 @Param[in]     event_handle    - Handle to an event semaphore structure.
 @Param[out]    count           - value of semaphore.

 @Retval        OS_SUCCESS                  : OK.
 @Retval        OS_ERR_EVENT_SEMA_INVALID   : bad semaphore.
*//***************************************************************************/
os_status osEventSemaphoreCountGet(os_event_handle event_handle, uint32_t* count);

/**************************************************************************//**
 @Function      osEventSemaphorePend

 @Description   Pend on a counting semaphore.

                The calling task will block until a the semaphore will be
                posted or until its timeout shall expire.
 
 @Param[in]     event_handle    - Handle to an event structure.
 @Param[in]     timeout         - Timeout to pend in ticks.

 @Retval        OS_SUCCESS                      : Got the ersource.
 @Retval        OS_ERR_EVENT_SEMA_PEND_INTERR   : called in interrupt.
 @Retval        OS_ERR_EVENT_SEMA_TIMEOUT       : Timeout expired.

 @Cautions      background task can not call this function.
 @Cautions      SWI and HWI can not call this function.
*//***************************************************************************/
os_status osEventSemaphorePend(os_event_handle event_handle, uint32_t timeout);


/**************************************************************************//**
 @Function      osEventSemaphoreAccept

 @Description   Try to counting semaphore without pending.
 
                This function checks if semaphore counter is not zero. If its not
                decrement it and return to the caller. Otherwise return a
                status indicating that counter is zero.
                This is a non blocking function so it can be used in interrupt and
                software interrupt contexts.
 
 @Param[in]     event_handle - Handle to an event structure.

 @Retval        OS_SUCCESS                      : Got the semaphore.
 @Retval        OS_ERR_EVENT_SEMA_UNAVAILABLE   : counter is 0.
*//***************************************************************************/
os_status osEventSemaphoreAccept(os_event_handle event_handle);


/**************************************************************************//**
 @Function      osEventSemaphorePost

 @Description   Posts a counting semaphore.
 
                If tasks are waiting in the waiting list of the semaphore, wake the
                first one, otherwise increment the counter for tasks that will pend
                or accept it later.
 
 @Param[in]     event_handle - Handle to an event structure.
 @Param[out]    resumed_task - Task that was resumed or NULL.

 @Retval        OS_SUCCESS                  : Got the ersource.
 @Retval        OS_EVENT_SEMAPHORE_OVERFLOW : if semaphore is full
*//***************************************************************************/
os_status osEventSemaphorePost(os_event_handle event_handle, os_task_handle *resumed_task);

/** @} */ // event_sem_runtime_id
/** @} */ // event_sem_id

_OS_END_EXTERN_C

#endif __OS_EVENT_SEMAPHORE_H
