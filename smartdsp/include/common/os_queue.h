/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:26 $
 $Id: os_queue.h,v 1.25 2012/11/22 16:28:26 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_queue.h,v $
 $Revision: 1.25 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_queue.h

 @Description   OS Queue header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_QUEUE_H
#define __OS_QUEUE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         queue_id Queues API

 @Description   Queues setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Queue handle - represents a specific queue object.
 
                The queue handle is returned from osQueueFind(), and should
                be passed as a parameter to all the queue functions.
 
*//***************************************************************************/
typedef void* os_queue_handle;


/**************************************************************************//**
 @Description   Queue status values.
 
*//***************************************************************************/
typedef enum
{
    OS_QUEUE_EMPTY = 0, /**< Queue is empty */
    OS_QUEUE_MIDDLE,    /**< Queue is neither empty, nor full */
    OS_QUEUE_FULL       /**< Queue is full */
} os_queue_status;

/**************************************************************************//**
 @Description   Queue shared values.
 
*//***************************************************************************/
typedef enum
{
    OS_QUEUE_PRIVATE = 0,   /**< Queue is private, interfaced by a single core */
    OS_QUEUE_SHARED         /**< Queue is shared, may be interfaced by multiple cores */
} os_queue_shared;


/**************************************************************************//**
 @Function      osQueueCreate

 @Description   Prepares a queue for operation.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     queue_size -  Maximum number of elements allowed in the queue.

 @Retval        OS_SUCCESS                  - The queue was successfully created.
 @Retval        OS_ERR_NO_MEMORY            - Not enough memory.
 @Retval        OS_ERR_Q_ALREADY_CREATED    - The queue is already created.
 
 @Cautions      This function allocates some memory for the queue operation.
                You should use it only in your application's initialization
                code. Creating a specific queue twice will cause memory leaks.

*//***************************************************************************/
os_status   osQueueCreate(os_queue_handle que_handle, uint16_t queue_size);


/**************************************************************************//**
 @Function      osQueueCreateMultiple

 @Description   Prepares a multiple queue for operation.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     queue_size -  Maximum number of elements allowed in the queue.
 @Param[in]     num        -  Size of each element in 32 bits.


 @Retval        OS_SUCCESS                  - The queue was successfully created.
 @Retval        OS_ERR_NO_MEMORY            - Not enough memory.
 @Retval        OS_ERR_Q_ALREADY_CREATED    - The queue is already created.

 @Cautions      This function allocates some memory for the queue operation.
                You should use it only in your application's initialization
                code. Creating a specific queue twice will cause memory leaks.

*//***************************************************************************/
os_status   osQueueCreateMultiple(os_queue_handle que_handle, uint16_t queue_size, uint16_t num);


/**************************************************************************//**
 @Function      osQueueDelete

 @Description   Deletes the given queue from the system.
 
 @Param[in]     que_handle -  The queue handle.

 @Retval        OS_SUCCESS              - The queue was successfully deleted.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.
 
*//***************************************************************************/
os_status   osQueueDelete(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueDeleteUnsafe

 @Description   Deletes the given queue from the system (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.

 @Retval        OS_SUCCESS - The queue was successfully deleted.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.
 
 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status osQueueDeleteUnsafe(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueReset

 @Description   empty the given queue.
 
 @Param[in]     que_handle -  The queue handle.

 @Retval        OS_SUCCESS - The queue was successfully reset.

*//***************************************************************************/
os_status   osQueueReset(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueResetUnsafe

 @Description   empty the given queue (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.

 @Retval        OS_SUCCESS - The queue was successfully reset.
 
 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueResetUnsafe(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueEnqueue

 @Description   Inserts a value to the queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     value      -  The value to insert to the queue.

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueueEnqueue(os_queue_handle que_handle, uint32_t value);


/**************************************************************************//**
 @Function      osQueueUnsafeEnqueue

 @Description   Inserts a value to the queue (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     value      -  The value to insert to the queue.

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.
 
 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueUnsafeEnqueue(os_queue_handle que_handle, uint32_t value);

/**************************************************************************//**
 @Function      osQueueHeadEnqueue

 @Description   Inserts a value to the front of queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     value      -  The value to insert to the queue.

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueueHeadEnqueue(os_queue_handle que_handle, uint32_t value);


/**************************************************************************//**
 @Function      osQueueUnsafeHeadEnqueue

 @Description   Inserts a value to the front of queue (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.
 @Param[in]     value      -  The value to insert to the queue.

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueUnsafeHeadEnqueue(os_queue_handle que_handle, uint32_t value);

/**************************************************************************//**
 @Function      osQueueDequeue

 @Description   Removes the first value from the queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueueDequeue(os_queue_handle que_handle, uint32_t *value);


/**************************************************************************//**
 @Function      osQueueUnsafeDequeue
 
 @Description   Removes the first value from the queue (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueUnsafeDequeue(os_queue_handle que_handle, uint32_t *value);


/**************************************************************************//**
 @Function      osQueuePeek

 @Description   Queries the first value in the queue without removing it.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueuePeek(os_queue_handle que_handle, uint32_t *value);


/**************************************************************************//**
 @Function      osQueueTailDequeue

 @Description   Removes the last value from the queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the last value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueueTailDequeue(os_queue_handle que_handle, uint32_t *value);


/**************************************************************************//**
 @Function      osQueueUnsafeTailDequeue
 
 @Description   Removes the last value from the queue (unsafe version).
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the last value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueUnsafeTailDequeue(os_queue_handle que_handle, uint32_t *value);


/**************************************************************************//**
 @Function      osQueueStatus

 @Description   Queries the status of the queue (empty, full, etc.).
 
 @Param[in]     que_handle -  The queue handle.

 @Return        The queue status (#OS_QUEUE_EMPTY, #OS_QUEUE_MIDDLE, #OS_QUEUE_FULL);
                if the queue was not created, OS_QUEUE_EMPTY will be returned.

*//***************************************************************************/
os_queue_status osQueueStatus(os_queue_handle que_handle);


/**************************************************************************//**
 @Function      osQueueUnsafeStatus

 @Description   Queries the status of the queue (unsafe version).
                Use this function when you can be sure that the queue object
                cannot be accessed from any other source.
 
 @Param[in]     que_handle -  The queue handle.

 @Return        The queue status (#OS_QUEUE_EMPTY, #OS_QUEUE_MIDDLE, #OS_QUEUE_FULL);
                if the queue was not created, OS_QUEUE_EMPTY will be returned.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_queue_status osQueueUnsafeStatus(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueFind

 @Description   Finds the first available queue number.
 
 @Param[out]    que_handle -  Receives the available queue number, or
                              NULL if no queue is available.
 @Param[in]     shared     -  Set to TRUE to find an available shared
                              queue, or FALSE for a regular queue.

 @Retval        OS_SUCCESS              - Found an available queue.
 @Retval        OS_ERR_Q_UNAVAILABLE    - No queue is available.

*//***************************************************************************/
os_status   osQueueFind(os_queue_handle *que_handle, bool shared);

/**************************************************************************//**
 @Function      osQueueEnqueueMultiple

 @Description   Inserts a value to the queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Pointer to an array of values to insert to queue

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status osQueueEnqueueMultiple(os_queue_handle que_handle, uint32_t* value);

/**************************************************************************//**
 @Function      osQueueUnsafeEnqueueMultiple

 @Description   Inserts a multiple value to the queue (unsafe version).

 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Pointer to an array of values to insert to queue

 @Retval        OS_SUCCESS              - The value was successfully inserted.
 @Retval        OS_ERR_Q_FULL           - The queue is full; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status osQueueUnsafeEnqueueMultiple(os_queue_handle que_handle, uint32_t* value);

/**************************************************************************//**
 @Function      osQueueDequeueMultiple

 @Description   Removes the first value from the queue.
 
 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status   osQueueDequeueMultiple(os_queue_handle que_handle, uint32_t *value);

/**************************************************************************//**
 @Function      osQueueUnsafeDequeueMultiple

 @Description   Removes the first value from the queue (unsafe version).

 @Param[in]     que_handle -  The queue handle.
 @Param[out]    value      -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

 @Cautions      Use this function when you can be sure that the queue object
                cannot be accessed from any other source.

*//***************************************************************************/
os_status   osQueueUnsafeDequeueMultiple(os_queue_handle que_handle, uint32_t *value);

/**************************************************************************//**
 @Function      osQueueShared

 @Description   Queries the status of the queue (shared, private)
 
 @Param[in]     que_handle -  The queue handle.

 @Return        The queue status (OS_QUEUE_PRIVATE, OS_QUEUE_SHARED).

*//***************************************************************************/
os_queue_shared   osQueueShared(os_queue_handle que_handle);

/**************************************************************************//**
 @Function      osQueueSize

 @Description   Returns the maximum number of elements allowed in the queue
 
 @Param[in]     que_handle -   The queue handle.
 @Param[out]    size       -  The size of the queue.

 @Retval        OS_SUCCESS          - The size was returned successfully.
 @Retval        OS_ERR_Q_EMPTY      - The queue was not created.

*//***************************************************************************/
os_status   osQueueSize(os_queue_handle que_handle, uint32_t* size);

/**************************************************************************//**
 @Function      osQueueMultipleSize

 @Description   Returns the maximum number of elements allowed in the multiple queue
 
 @Param[in]     que_handle -   The queue handle.
 @Param[out]    size       -  The size of the queue.

 @Retval        OS_SUCCESS          - The size was returned successfully.
 @Retval        OS_ERR_Q_EMPTY      - The queue was not created.

*//***************************************************************************/
os_status   osQueueMultipleSize(os_queue_handle que_handle, uint32_t* size);

/**************************************************************************//**
 @Function      osQueueMultipleNum

 @Description   Returns the size of each element in the multiple queue in 32 bits
 
 @Param[in]     que_handle -   The queue handle.
 @Param[out]    num        -  The size of the queue.

 @Retval        OS_SUCCESS          - The size was returned successfully.
 @Retval        OS_ERR_Q_EMPTY      - The queue was not created.

*//***************************************************************************/
os_status   osQueueMultipleNum(os_queue_handle que_handle, uint32_t* num);


/** @} */ // end of queue_id

_OS_END_EXTERN_C

#endif // __OS_QUEUE_H

