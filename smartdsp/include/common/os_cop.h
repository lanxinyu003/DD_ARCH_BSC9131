/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/18 11:06:57 $
 $Id: os_cop.h,v 1.27 2013/07/18 11:06:57 b41640 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cop.h,v $
 $Revision: 1.27 $
******************************************************************************/

/**************************************************************************//**

 @File          os_cop.h

 @Description   OS COP layer header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __OS_COP_H
#define __OS_COP_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         cop_id   COP – Co-Processor Module API

 @Description   Co-Processors routines, defines, enumerations and API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Defines a maximum number of channel allowed to be opened on a COP device.
*//***************************************************************************/
#define MAX_NUM_OF_CHANNELS 16

/**************************************************************************//**
 @Collection    COP Channel Modes
 
                When opening a channel, the application has to
                specify whether it is #COP_CH_REAP or
                #COP_CH_DISPATCH or both. Channels start as
                automatically being enabled

 @{
*//***************************************************************************/
#define COP_CH_REAP                 0x1
/**< The channel can reap jobs after the co-processor finishes
     executing them */
#define COP_CH_DISPATCH             0x2
/**< The channel can send jobs for the co-processor to execute */
#define COP_CH_SHARED_DELETED       0x4
/**< The channel can send jobs for the co-processor to execute */

/* @} */ /* end of cop_ch_modes */


/**************************************************************************//**
 @Anchor        cop_ctrl_cmd
 
 @Collection    COP Control Commands
 
                Used in osCopDeviceCtrl() and osCopChannelCtrl()
 
 @{
*//***************************************************************************/

#define COP_LLD_COMMAND             0x01000000
/**< Defines a command that will be handled by the Low Level Driver */
#define COP_NON_BLOCKING_COMMAND    0x02000000
/**< Defines a command that will be non-blocking */
#define COP_CHANNEL_DISABLE         (0x00000001 | COP_LLD_COMMAND)
/**< Disable a channel from processing more jobs */
#define COP_CHANNEL_ENABLE          (0x00000002 | COP_LLD_COMMAND)
/**< Enable a channel to process jobs */
#define COP_CHANNEL_STAT_GET        (0x0000000F | COP_LLD_COMMAND)
/**< Read the LLD statistics for the channel */
#define COP_CHANNEL_POLL            (0x0000000E | COP_LLD_COMMAND)
/**< Poll the LLD channel for jobs that are ready for reaping */

#define COP_DEVICE_RESET            (0x00000001 | COP_LLD_COMMAND)
/**< Reset the LLD device */
#define COP_DEVICE_STAT_GET         (0x00000002)
/**< Read the statistics for the COP device */
#define COP_DEVICE_DISPACHER_SET    (0x00000003)
/**< Set the function to be called by osCopChannelDispatch()
     in order to send jobs to the LLD. This function may only be set
     once, generally by the OS */
#define COP_DEVICE_ISR_SET          (0x00000004 | COP_LLD_COMMAND)
/**< Set the function to be called by the LLD when the co-processor
     indicates that a job is finished executing. This function may
     only be set once, generally by the OS */
#define COP_CHANNEL_CALLBACK_SET    (0x00000005)
/**< Set a channel specific callback fucntion, thus overriding the
     use of the device callback on the specified channel */
#define COP_CHANNEL_CALLBACK_REMOVE (0x00000006)
/**< Remove a channel specific callback fucntion, thus restoring the
     use of the device callback on the specified channel */

/* @} */ /* end of cop_ctrl_cmd */


/**************************************************************************//**
 @Description   COP device handle - passed as parameter to all device functions.
*//***************************************************************************/
typedef void* cop_dev_handle;

/**************************************************************************//**
 @Description   COP job ID - a parameter to the job descriptor.
*//***************************************************************************/
typedef void* cop_job_id;

/**************************************************************************//**
 @Description   COP job descriptor.
 
                The application creates a NULL terminated linked list of
                such descriptors and calls osCopChannelDispatch() to send
                them to be executed.
 
 @Cautions      The linked list of jobs must be created on the heap. Placing
                in on the stack may cause memory corruption
*//***************************************************************************/
typedef struct cop_job_handle_s
{
    cop_job_id  job_id;
    /**< Used by the application to identify finished jobs. */
    void        *device_specific;
    /**< LLD specific job parameters */
    struct cop_job_handle_s* next;
    /**< Pointer to the next job or NULL to indicate the last job */
} cop_job_handle;


/**************************************************************************//**
 @Description   COP Channel callback function typedef.
 
                This function is called by the COP Module when the LLD
                indicates that a job(s) is ready for the application - i.e.
                the co-processor finished execution
*//***************************************************************************/
    typedef void (*os_cop_callback)(void * job_handle, void *param);

/**************************************************************************//**
 @Description   COP error callback function typedef.
*//***************************************************************************/
typedef void (*os_cop_error)(void *param);

/**************************************************************************//**
 @Description   COP device configuration parameters.

                This structure is passed to osCopDeviceOpen() and defines
                some of the device parameters that the COP requires.

                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
*//***************************************************************************/
typedef struct
{
 
    os_cop_callback dispatch_callback;
    /**< Application's dispatch callback function
         by the COP module */
    os_cop_error    error_callback;
    /**< Application's callback for errors. */
    void            *lld_params;
    /**< LLD parameters for device open. */
    uint8_t     max_num_of_channels;
    /**< Max number of channels allowed on device. */
 
} cop_dev_open_params_t;

/**************************************************************************//**
 @Description   COP channel configuration parameters.

                This structure is passed to osCopChannelOpen() and defines
                some of the channel parameters that the COP requires.

                The structure also contains a pointer to LLD-specific
                parameters. This pointer is passed to the LLD as it is.
*//***************************************************************************/
typedef struct
{

    uint16_t     channel_num;
    /**< Channel number. */
    uint16_t     num_jobs;
    /**< Number of jobs channel can handle */
    void         *callback_parameter;
    /**< Application's parameter for the data callbacks from this channel */
    void         *error_callback_parameter;
    /**< Application's parameter for the error callbacks from this channel */
    void         *lld_params;
    /**< LLD-specific channel parameters. */
    os_mem_type  heap;
    /**< Heap from which to allocate the channel structure */

} cop_ch_open_params_t;


typedef void* cop_channel_t;


/**************************************************************************//**
 @Description   COP interface statistics
*//***************************************************************************/
typedef struct
{
    uint32_t        dispatched_jobs;        /**< Number of dispatched jobs.  */
    uint32_t        reaped_jobs;            /**< Number of reaped jobs.   */
    uint32_t        failed_jobs;            /**< Number of failed jobs */

} cop_interface_stat_t;

/**************************************************************************//**
 @Group         cop_init_id     COP Initialization API

 @Description   COP upper layer API for the application initialization stage
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osCopDeviceOpen

 @Description   Opens a COP device for operation using the given parameters.

 @Param[in]    device_name     -  The name of the device.
 @Param[in]    dev_open_params -  Device parameters for use by both COP
                                  and LLD, or NULL to use the default device
                                  parameters.

 @Return        A COP device handle if the device was opened successfully
 @Return        NULL if the device failed to open
 
*//***************************************************************************/
cop_dev_handle  osCopDeviceOpen(char                  *device_name,
                                cop_dev_open_params_t *dev_open_params);

/**************************************************************************/

/***************************************************************************//**
 @Function      osCopDeviceClose

 @Description   Closes an opened COP device.

 @Param[in]    deviceHandle     -  A COP device handle to close

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_FAIL if the underlying LLD channel failed to open properly
 @Return        Error status, encoded in os_error.h, for other errors
*//***************************************************************************/
os_status  osCopDeviceClose(cop_dev_handle         deviceHandle);

/**************************************************************************//**
 @Function      osCopChannelOpen

 @Description   Opens a channel of a COP device using the given parameters.

 @Param[in]     cop_handle     -  COP device handle, osCopDeviceOpen()
                                  returns this handle.
 @Param[out]    cop_ch         -  Pointer to a COP channel structure.
 @Param[in]     ch_open_params -  Channel parameters.

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_FAIL if the underlying LLD channel failed to open properly
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status   osCopChannelOpen(cop_dev_handle       cop_handle,
                             cop_channel_t        *cop_ch,
                             cop_ch_open_params_t *ch_open_params);



/**************************************************************************//**
 @Function      osCopSharedChannelOpen

 @Description   Opens a shared channel of a COP device using the given parameters
 
                A COP shared channel is one that can be dispatched to from one
                core and reaped by another (or the same one).

 @Param[in]     cop_handle     -  COP device handle, osCopDeviceOpen()
                                  returns this handle.
 @Param[out]    cop_ch         -  Pointer to a COP channel structure.
 @Param[in]     ch_open_params -  Channel parameters.
 @Param[in]     mode           -  Channel mode (#COP_CH_REAP or #COP_CH_DISPATCH)
 @Param[out]    queue          -  Pointer to a created queue handle; if mode
                                  isn't (#COP_CH_REAP | #COP_CH_DISPATCH) - must
                                  be a shared queue

 @Retval        OS_SUCCESS if channel was opened successfully
 @Retval        OS_FAIL if the underlying LLD channel failed to open properly
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status   osCopSharedChannelOpen(cop_dev_handle       cop_handle,
                                   cop_channel_t        *cop_ch,
                                   cop_ch_open_params_t *ch_open_params,
                                   int                  mode,
                                   os_queue_handle      queue);

/**************************************************************************//**
 @Function      osCopChannelClose

 @Description   Closes an open channel of a COP device.

 @Param[in]     cop_ch -  Pointer to a valid COP channel, osCopChannelOpen()
                          returns this pointer.

 @Retval        OS_SUCCESS if channel was closed successfully
 @Return        Error status, encoded in os_error.h, for other errors
*//***************************************************************************/
os_status   osCopChannelClose(cop_channel_t cop_ch);

/**************************************************************************//**
 @Function      osCopChannelClose

 @Description   Closes an open channel of a COP device.

 @Param[in]     cop_ch -  Pointer to a valid COP channel, osCopChannelOpen()
                          returns this pointer.

 @Retval        OS_SUCCESS if channel was closed successfully
 @Return        Error status, encoded in os_error.h, for other errors
*//***************************************************************************/
os_status   osCopSharedChannelClose(cop_channel_t cop_ch);

/** @} */ // end of cop_init_id

/**************************************************************************//**
 @Group         cop_runtime_id     COP Runtime API

 @Description   COP upper layer API for the  application runtime stage
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osCopDeviceCtrl

 @Description   Performs control commands on a device.

 @Param[in]     cop_handle  -  COP device handle returned by osCopDeviceOpen()
 @Param[in]     command     -  Command code - see @ref cop_ctrl_cmd "COP Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_UNSUPPORTED if command is unsupported
 @Retval        OS_ERR_ALREADY_CREATED if calling with #COP_DEVICE_DISPACHER_SET
                and there already is a dispatcher for the device
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                device failed to execute the command
*//***************************************************************************/
os_status   osCopDeviceCtrl(cop_dev_handle cop_handle, uint32_t command, void *param);


/**************************************************************************//**
 @Function      osCopChannelCtrl

 @Description   Performs control commands on a channel.

 @Param[in]     cop_ch      -  COP device handle returned by osCopDeviceOpen()
 @Param[in]     command     -  Command code - see @ref cop_ctrl_cmd "COP Control Commands"
 @Param[in]     param       -  Command parameter, command specific

 @Retval        OS_SUCCESS if command was executed properly
 @Retval        OS_ERR_COMMAND_UNSUPPORTED if command is unsupported
 @Return        Error status, encoded in os_error.h, if the underlying LLD
                device failed to execute the command
*//***************************************************************************/
os_status   osCopChannelCtrl(cop_channel_t *cop_ch, uint32_t command, void *param);


/**************************************************************************//**
 @Function      osCopChannelDispatch

 @Description   Dispatches a job(s) to a channel.
 
                This channel takes a NULL terminated link list of jobs
                (cop_job_handle) and sends it the channel for execution

  @Param[in]    cop_ch   -  Pointer to a valid COP channel, osCopChannelOpen()
                            returns this pointer.
 @Param[in]     jobs     -  Pointer to the head of a NULL terminated link list of jobs
 @Param[in]     num_jobs -  Number of jobs in the linked list

 @Retval        OS_SUCCESS if the jobs were dispatched successfully
 @Return        Error status, encoded in os_error.h, if there was an error in
                dispatching the jobs
*//***************************************************************************/
os_status osCopChannelDispatch(cop_channel_t *cop_ch, cop_job_handle *jobs, int* num_jobs);

/** @} */ // cop_runtime_id

/** @} */ // end of cop_id

_OS_END_EXTERN_C

#endif // __OS_COP_H
