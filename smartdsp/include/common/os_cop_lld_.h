/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/04/22 09:05:40 $
 $Id: os_cop_lld_.h,v 1.20 2013/04/22 09:05:40 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cop_lld_.h,v $
 $Revision: 1.20 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cop_lld_.h

 @Description   OS BIO-LLD Interface.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_COP_LLD_H_
#define __OS_COP_LLD_H_

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include "os_cop.h"

/**************************************************************************//**
 @Group         cop_group   Co-processor

 @Description   Co-processor routines
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         cop_lld     COP LLD API

 @Description   COP lower layer (LLD) API
 
                This API is for use by the Low Level Driver developer. Users
                should use the COP higher layer API within their applications.
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   COP channel internal structure.

                This structure is exposed to the application layer to let the
                application allocate memory for the channels in a convenient
                way. The application should not change any field in this
                structure.

 @Cautions      This structure is declared publicly to enable allocating enough
                memory for it by the application. Users are prohibited from
                directly accessing the structure.
*//***************************************************************************/
typedef struct
{
    cop_dev_handle          *cop_dev;
    /**< COP device handle. */
    uint8_t                 status;
    /**< COP channel status. */
    uint8_t                 flags;
    /**< Flags used by the kernel to manage the channel */
    os_cop_callback         dispatch_callback;
    /**< Channel callback function - defaults to Device's callback function */
    os_queue_handle         job_queue;
    /**< The channel's jobs queue. Each pointer here is a pointer to a list of jobs*/
    void                    *lld_channel;
    /**< Handle to LLD channel */
    cop_job_handle          *cb_job;
    /**< Internal: next job handle to return.
         If it is NULL then the next list of jobs is taken from job_queue. */

} cop_channel_;


/* cop_channel_t.flags checks */
#define __COP_CAN_REAP(CH)          (CH->flags & COP_CH_REAP)
#define __COP_CAN_DISPATCH(CH)      (CH->flags & COP_CH_DISPATCH)
#define __COP_STANDARD_CH(CH)       ((CH->flags & (COP_CH_REAP | COP_CH_DISPATCH)) == (COP_CH_REAP | COP_CH_DISPATCH))

/**************************************************************************//**
 @Description   COP LLD channel parameters, passed by the COP serializer to
                the LLD when a channel is opened.
 
*//***************************************************************************/
typedef struct
{
    uint16_t    channel_num;
    /**< Channel number. */
    uint16_t    number_of_jobs;
    /**< Number of jobs in channel BD ring */
    void          *cop_ch;
    /**< Handle to COP channel */
    void          *lld_params;
    /**< LLD-specific channel parameters. */
 
} cop_lld_ch_open_t;


/**************************************************************************//**
 @Description   COP LLD registration parameters.
 
*//***************************************************************************/
typedef struct
{
    os_status   (*device_open)(void *device_handle, void *lld_params);
    /**< Device-open function. */
    void *      (*channel_open)(void *device_handle, cop_lld_ch_open_t *lld_ch_params);
    /**< Channel-open function. */
    os_status   (*channel_close)(void *ch_handle);
    /**< Channel-close function. */
    os_status   (*channel_dispatch)(void *ch_handle, void *jobs, int *num_jobs);
    /**< Channel-transmit function. */
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< Channel-control function. */
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< Device-control function. */
    void        *lld_handle;
    /**< LLD Handle. */
    os_status   (*device_close)(void *device_handle);
    /**< Device-close function. */
    uint16_t    max_job_queue_size;
    /**< Max size of Job queue. */
} cop_register_params_t;


/**************************************************************************//**
 @Function      copRegister

 @Description   This function registers a new device.
 
                The function takes the LLD's function pointers and basic LLD
                definitions as parameters. This function should be called by
                each LLD instance.
 
 @Param[in]    device_name -  The name of the of device.
 @Param[in]    register_params -  COP registration parameters.

 @Return        The COP device handle; This handle should be used by the LLD
                in any COP call for this device.

*//***************************************************************************/
cop_dev_handle  copRegister(char *device_name, cop_register_params_t *register_params);

/**************************************************************************//**
 @Function      copResetDevice

 @Description   This function resets the device.
 
                This function updates the device to it's initial state before
                the copDeviceOpen().
 
 @Param         cop_handle - The COP device to be reset.

 @Return        None.

*//***************************************************************************/
void copResetDevice(cop_dev_handle *cop_handle);


/**************************************************************************//**
 @Function      copChannelDispatchCb

 @Description   LLD calls this function to inform the COP  that
                        a job has finished execution.
 
 @Param[in]    cop_ch -  Pointer to a valid COP channel.
 @Param[in]    error_status -  LLD-specific error and status value.
 @Param[in]    num_jobs -  Number of jobs to dequeue.

*//***************************************************************************/
void    copChannelDispatchCb(cop_channel_ *cop_ch, uint32_t error_status, uint32_t num_jobs);

/**************************************************************************//**
 @Function      copChannelDispatchCbNoUserCb

 @Description   LLD calls this function to inform the COP  that
                        a job has finished execution and BD handling is done by LLD.
 
 @Param[in]    cop_ch -  Pointer to a valid COP channel.
 @Param[in]    error_status -  LLD-specific error and status value.
 @Param[in]    num_jobs -  Number of jobs to dequeue.

*//***************************************************************************/
void    copChannelDispatchCbNoUserCb(cop_channel_ *cop_ch, uint32_t error_status, uint32_t num_jobs);

/**************************************************************************//**
 @Function      copChannelFirstJobPeek

 @Description   Queries the first value in the channel queue without removing it.
 
 @Param[in]    cop_ch -  The channel handle.
 @Param[out]    value -  Receives the first value in the queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
#define copChannelFirstJobPeek(cop_ch, value) \
    osQueuePeek(((cop_channel_*)(cop_ch))->job_queue, (value))


/**************************************************************************//**
 @Function      copChannelType

 @Description   Queries if channel is reap/dispatch or both.
 
 @Param[in]    cop_ch -  The channel handle.

 @Return        A bit mask including COP_CH_REAP, COP_CH_DISPATCH

*//***************************************************************************/
#define copChannelType(cop_ch)  (((cop_channel_ *)(cop_ch))->flags & (COP_CH_REAP | COP_CH_DISPATCH))

/**************************************************************************//**
 @Function      copChannelPeek

 @Description   Queries the first value in the jobs-queue without removing it.
 
 @Param[in]     cop_ch -  COP channel handle.
 @Param[out]    job    -  pointer to first job in COP queue.

 @Retval        OS_SUCCESS              - The first value was successfully
                                          removed.
 @Retval        OS_ERR_Q_EMPTY          - The queue is empty; operation failed.
 @Retval        OS_ERR_Q_NOT_CREATED    - The queue was not created, or it was
                                          already deleted.

*//***************************************************************************/
os_status copChannelPeek(cop_channel_ *cop_ch, cop_job_handle** job);


/** @} */ /* end of COP LLD API group */

/** @} */ /* end of COP group */

_OS_END_EXTERN_C

#endif // __OS_COP_LLD_H_

