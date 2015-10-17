/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/10/03 10:48:09 $
 $Id: os_cop.c,v 1.34 2013/10/03 10:48:09 b17975 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_cop.c,v $
 $Revision: 1.34 $
******************************************************************************/

/******************************************************************************

 @File          os_cop_init.c

 @Description   OS COP layer initialization functions.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_cop_lld_.h"
#include "os_cop_.h"
#include "os_rm_.h"

extern uint16_t             g_total_cop_devices;

/*****************************************************************************/
os_status   osCopDeviceCtrl(cop_dev_handle cop_handle, uint32_t command, void *param)
{
    cop_interface_t   *cop_dev = (cop_interface_t *)cop_handle;
    os_status         status = OS_SUCCESS;

    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COND(cop_dev->status == OS_RESOURCE_USED);

    if (command & COP_LLD_COMMAND)
    {
        OS_ASSERT_COND(cop_dev->device_ctrl != NULL);
        return cop_dev->device_ctrl(cop_dev->device_handle, command, param);
    }

    osHwiSwiftDisable();

    switch(command)
    {
#if COP_STATISTICS == ON
        case COP_DEVICE_STAT_GET:
        {
            cop_interface_stat_t * stats = (cop_interface_stat_t*)param;
            OS_ASSERT_COND(param != NULL);

            *stats = cop_dev->cop_stat;
            status = OS_SUCCESS;
            break;
        }
#endif
        case COP_DEVICE_DISPACHER_SET:
        {
            if (cop_dev->channel_dispatch != NULL)
            {
                OS_ASSERT;
                status = OS_ERR_ALREADY_CREATED;
            }
            else
                cop_dev->channel_dispatch = param;

            break;
        }

        default:
            status = OS_ERR_COMMAND_UNSUPPORTED;
    }

    osHwiSwiftEnable();

    return status;
}


/*****************************************************************************/
os_status   osCopChannelCtrl(cop_channel_t *cop_ch, uint32_t command, void *param)
{
    cop_interface_t   *cop_dev;
    cop_channel_      *ch = NULL;

    OS_ASSERT_COND(cop_ch != NULL);
    OS_ASSERT_COND(*cop_ch != NULL);
 
    ch = *cop_ch;
    cop_dev = (cop_interface_t *)ch->cop_dev;

    OS_ASSERT_COND(ch->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COND(cop_dev->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(cop_dev->channel_ctrl != NULL);

    if (command & COP_LLD_COMMAND)
        return cop_dev->channel_ctrl(ch->lld_channel, command, param);

    switch(command)
    {
        case COP_CHANNEL_CALLBACK_SET:
            OS_ASSERT_COND(param != NULL);
            ch->dispatch_callback = (os_cop_callback)param;
            break;
 
        case COP_CHANNEL_CALLBACK_REMOVE:
            ch->dispatch_callback = cop_dev->dispatch_callback;
            break;
 
        default:
            RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED);
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osCopChannelDispatch(cop_channel_t *cop_ch, cop_job_handle *jobs, int* num_jobs)
{
    cop_interface_t *cop_dev;
    cop_job_handle  *job;
    os_status       status;
    int             required_num_jobs;
    cop_channel_    *ch = NULL;
    bool     		shared_queue;

    OS_ASSERT_COND(cop_ch != NULL);

    ch = *cop_ch;

    OS_ASSERT_COND(ch->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(__COP_CAN_DISPATCH(ch));
 
    shared_queue = (copChannelType(ch) != (COP_CH_REAP | COP_CH_DISPATCH));
 
    cop_dev = (cop_interface_t *)ch->cop_dev;
 
    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COND(cop_dev->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(cop_dev->channel_dispatch != NULL);
    OS_ASSERT_COND(cop_dev->dispatch_callback != NULL);
    OS_ASSERT_COND(jobs != NULL);
    OS_ASSERT_COMPILER_COND((*num_jobs <= cop_dev->max_job_queue_size) && (*num_jobs > 0));
    OS_ASSERT_COMPILER_COND(*num_jobs > 0);

    osHwiSwiftDisable();

    // Enqueue before dispatching in order to avoid situation where copChannelDispatchCb()
    //  is called by LLD on a job which hasn't yet been queued. Can happen if job execution
    //  time is very short.
#ifndef DISABLE_ALL_ASSERTS
    required_num_jobs = *num_jobs;

    job = jobs->next;
 
    while(--required_num_jobs)
    {
        OS_ASSERT_COND(job != NULL);
        job = job->next;
    }

    // Verify NULL terminated job list
    OS_ASSERT_COND(job == NULL);
#endif // DISABLE_ALL_ASSERTS

    required_num_jobs = *num_jobs;
 
    if (shared_queue)
        status = osQueueEnqueue(ch->job_queue, (uint32_t)jobs);
    else
        status = osQueueUnsafeEnqueue(ch->job_queue, (uint32_t)jobs);
 
    if (status != OS_SUCCESS)
    {
#if COP_STATISTICS == ON
            cop_dev->cop_stat.failed_jobs += required_num_jobs;
#endif
            osHwiSwiftEnable();
            return status;
    }

    osHwiSwiftEnable();

    status = cop_dev->channel_dispatch(ch->lld_channel, jobs, num_jobs);

    osHwiSwiftDisable();

#if COP_STATISTICS == ON
    cop_dev->cop_stat.dispatched_jobs += *num_jobs;
    cop_dev->cop_stat.failed_jobs += (required_num_jobs - *num_jobs);
#endif

    if (status != OS_SUCCESS)
    {
        if (shared_queue)
            osQueueTailDequeue(ch->job_queue, (uint32_t *)&job);
        else
            osQueueUnsafeTailDequeue(ch->job_queue, (uint32_t *)&job);
    }

    osHwiSwiftEnable();


    return status;
}


/*****************************************************************************/
static inline cop_job_handle* copChannelDequeue(cop_channel_ *cop_ch)
{
    cop_job_handle  *job;


    osQueueUnsafeDequeue(cop_ch->job_queue, (uint32_t *)&job);
 
    return job;
}
/*****************************************************************************/
static inline cop_job_handle* copGetNextJob(cop_channel_ *cop_ch)
{
    cop_job_handle  *job;

    if (cop_ch->cb_job == NULL)
    {
        job = copChannelDequeue(cop_ch);
    }
    else
    {
        job = cop_ch->cb_job;
    }
 
    return job;
}


/*****************************************************************************/
void    copChannelDispatchCb(cop_channel_ *cop_ch, uint32_t error_status, uint32_t num_jobs)
{
    os_status       status;
    cop_job_handle  *head;
    cop_interface_t *cop_dev;

    bool      	    shared_queue;
 
    OS_ASSERT_COND(cop_ch != NULL);
    OS_ASSERT_COND(__COP_CAN_REAP(cop_ch));
 
    shared_queue = (copChannelType(cop_ch) != (COP_CH_REAP | COP_CH_DISPATCH));
    cop_dev = (cop_interface_t *)cop_ch->cop_dev;
 
    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COND(cop_dev->dispatch_callback != NULL);
    OS_ASSERT_COMPILER_COND(num_jobs > 0);

    /* Creating 2 loops instead of inline functions, function pointers or any other trick towards
     * code size; since the compiler generates the best code this way.
     */
    if (shared_queue)
    {
        while(num_jobs--)
        {
            osHwiSwiftDisable();
            if (cop_ch->cb_job == NULL)
            {
                status = osQueueDequeue(cop_ch->job_queue, (uint32_t *)&head);
                OS_ASSERT_COND(status == OS_SUCCESS);
            }
            else
            {
                head = cop_ch->cb_job;
            }

            OS_ASSERT_COND(head != NULL);
            cop_ch->cb_job = head->next;

#if COP_STATISTICS == ON
            cop_dev->cop_stat.reaped_jobs++;
#endif
            osHwiSwiftEnable();
            cop_ch->dispatch_callback(head, (void *)error_status);
        }
 
    }
    else
    {
        while(num_jobs--)
        {
            osHwiSwiftDisable();
            if (cop_ch->cb_job == NULL)
            {
                status = osQueueUnsafeDequeue(cop_ch->job_queue, (uint32_t *)&head);
                OS_ASSERT_COND(status == OS_SUCCESS);
            }
            else
            {
                head = cop_ch->cb_job;
            }

            OS_ASSERT_COND(head != NULL);
            cop_ch->cb_job = head->next;

#if COP_STATISTICS == ON
            cop_dev->cop_stat.reaped_jobs++;
#endif
            osHwiSwiftEnable();
            cop_ch->dispatch_callback(head, (void *)error_status);
        }
    }
 
 
}

/*****************************************************************************/
void    copChannelDispatchCbNoUserCb(cop_channel_ *cop_ch, uint32_t error_status, uint32_t num_jobs)
{
    os_status       status;
    cop_job_handle  *head;
    cop_interface_t *cop_dev;
    bool      	    shared_queue;
 
    VAR_UNUSED(error_status);
    OS_ASSERT_COND(cop_ch != NULL);
    OS_ASSERT_COND(__COP_CAN_REAP(cop_ch));
 
    shared_queue = (copChannelType(cop_ch) != (COP_CH_REAP | COP_CH_DISPATCH));
    cop_dev = (cop_interface_t *)cop_ch->cop_dev;
 
    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COMPILER_COND(num_jobs > 0);

    /* Creating 2 loops instead of inline functions, function pointers or any other trick towards
     * code size; since the compiler generates the best code this way.
     */
    if (shared_queue)
    {
        while(num_jobs--)
        {
            osHwiSwiftDisable();
            if (cop_ch->cb_job == NULL)
            {
                status = osQueueDequeue(cop_ch->job_queue, (uint32_t *)&head);
                OS_ASSERT_COND(status == OS_SUCCESS);
            }
            else
            {
                head = cop_ch->cb_job;
            }

            OS_ASSERT_COND(head != NULL);
            cop_ch->cb_job = head->next;

#if COP_STATISTICS == ON
            cop_dev->cop_stat.reaped_jobs++;
#endif
            osHwiSwiftEnable();
        }
 
    }
    else
    {
        while(num_jobs--)
        {
            osHwiSwiftDisable();
            if (cop_ch->cb_job == NULL)
            {
                status = osQueueUnsafeDequeue(cop_ch->job_queue, (uint32_t *)&head);
                OS_ASSERT_COND(status == OS_SUCCESS);
            }
            else
            {
                head = cop_ch->cb_job;
            }

            OS_ASSERT_COND(head != NULL);
            cop_ch->cb_job = head->next;

#if COP_STATISTICS == ON
            cop_dev->cop_stat.reaped_jobs++;
#endif
            osHwiSwiftEnable();
        }
    }
 
 
}

/*****************************************************************************/
void copResetDevice(cop_dev_handle *cop_handle)
{
    cop_interface_t   *cop_dev;
 
    OS_ASSERT_COND(cop_handle != NULL);
    cop_dev = (cop_interface_t *)cop_handle;
    cop_dev->status = OS_RESOURCE_ACQUIRED;
#if COP_STATISTICS == ON
    cop_dev->cop_stat.dispatched_jobs = 0;
    cop_dev->cop_stat.reaped_jobs = 0;
    cop_dev->cop_stat.failed_jobs = 0;
#endif
}
/*****************************************************************************/
os_status copChannelPeek(cop_channel_ *cop_ch, cop_job_handle** job)
{
    os_status       status = OS_SUCCESS;
    cop_interface_t *cop_dev;

    bool      		shared_queue;

    OS_ASSERT_COND(cop_ch != NULL);
    OS_ASSERT_COND(__COP_CAN_REAP(cop_ch));

    shared_queue = (copChannelType(cop_ch) != (COP_CH_REAP | COP_CH_DISPATCH));
    cop_dev = (cop_interface_t *)cop_ch->cop_dev;
 
    OS_ASSERT_COND(cop_dev != NULL);

    osHwiSwiftDisable();
    if (cop_ch->cb_job == NULL)
    {
        status = osQueuePeek(cop_ch->job_queue, (uint32_t *)job);
    }
    else
    {
        *job = cop_ch->cb_job;
    }
    osHwiSwiftEnable();

    return status;
}
