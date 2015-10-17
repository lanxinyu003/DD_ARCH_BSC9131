/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_event_.h,v 1.8 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_event_.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_event_.h

 @Description   OS internal events definitions and prototypes.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_EVENT_H_
#define __OS_EVENT_H_

/**************************************************************************//**
 @Description   Event types.
 
*//***************************************************************************/
#define EVENT_TYPE_UNUSED     0x80000000
#define EVENT_TYPE_QUEUE      0x40000000
#define EVENT_TYPE_SEMAPHORE  0x20000000

extern struct os_ecb_t *g_all_events_array;

#define CHECK_EVENT_ID(EVENT) {extern uint32_t g_total_events_number;\
    OS_ASSERT_COND((EVENT) < g_total_events_number);}

/**************************************************************************//**
 @Description   Event Control Structure.
 
                General information needed during an event life span.
 
*//***************************************************************************/
typedef struct
{
    os_event_type        event_type;      /**< Event type. */
    uint32_t             event_count;     /**< Event count. */
    void                 *event_specific; /**< Event specific structure. */
    uint32_t             event_id;        /**< Identifier for the eevnt. */
    list_object_t          event_list;      /**< For unused events. */
} os_ecb_t;

#define GET_ECB_LIST(ptr) LIST_COMPONENT(ptr, os_ecb_t, event_list)

#define ECB_GET(EVT) (((os_ecb_t *)g_all_events_array) + (EVT))

/**************************************************************************//**
 @Function      osEventFind

 @Description   Finds the first available event.
 
 @Param[out]    event_handle -  Receives the free event handle.

 @Retval        OS_SUCCESS                  - Found an event.
 @Retval        OS_ERR_EVENT_UNAVAILABLE    - No event is available.

*//***************************************************************************/
os_status osEventFind(os_event_handle *event_handle);


/**************************************************************************//**
 @Function      osEventDelete

 @Description   Return event to unused pool.
 
 @Param         event_handle - Event to delete.

 @Retval        Whether.scheduling is required

*//***************************************************************************/
bool osEventDelete(os_ecb_t *event_handle);

/**************************************************************************//**
 @Function      osEventReset

 @Description   Release all pending tasks of event.
 
 @Param         event_handle - Event to delete.
 @Param         message - Message that is posted to all pending tasks.
 
 @Retval        Whether.scheduling is required

*//***************************************************************************/
bool osEventReset(os_ecb_t *reset_event, uint32_t message);

/**************************************************************************//**
 @Function      osEventPendUnsafe

 @Description   This function is called when event is not available and task
                needs to wait for it. Puts ECB handle in TCB of current running
                task, make the highest ready the Highest Task.
 
 @Param         event_handle - Event to pend current task upon.
 @Param         timeout - How much time to pend.

*//***************************************************************************/
os_status osEventPendUnsafe(os_ecb_t *event_handle, uint32_t timeout);


/**************************************************************************//**
 @Function      osEventReadyUnsafe

 @Description   This function will be called from event post functions to
                restart a pending task, if there are such.
                Get the TCB of the highest waiting task, remove it from
                delayed tasks, because the delay must be timeout for the
                event, and if not suspended, make task ready.
 
 
 @Param         event_handle - Event that was posted.
 @Param         message - Message that was posted.

 @Retval        Task that was resumed.

*//***************************************************************************/
os_task_handle osEventReadyUnsafe(os_ecb_t *event, uint32_t message);

#endif __OS_EVENT_H_
