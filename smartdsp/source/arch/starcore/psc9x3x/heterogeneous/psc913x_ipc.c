/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/20 14:34:37 $
 $Id: psc913x_ipc.c,v 1.29 2013/01/20 14:34:37 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/heterogeneous/psc913x_ipc.c,v $
 $Revision: 1.29 $
******************************************************************************/

/******************************************************************************
 
 @File          psc913x_heterogeneous_ipc.c

 @Description   SC850-specific intercore messages initialization functions.
 
 

 @Cautions
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "psc9x3x_hwi.h"
#include "psc9x3x_memmap.h"
#include "os_cache.h"
#include "sc3x00_cache_.h"
#include "psc913x_heterogeneous.h"
#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_ipc_.h"
#include "psc913x_ipc.h"

static inline os_het_status_t osIpcMessageChannelPointerAvailableCheck(os_ipc_channel_t *channel)
{
    if (channel->producer_pointers_tracker >= channel->local_channel.tracker.consumer_num)
       if (channel->producer_pointers_tracker - channel->local_channel.tracker.consumer_num == channel->local_channel.bd_ring_size)
            return OS_HETERO_FAIL;
    else
        if (channel->producer_pointers_tracker - channel->local_channel.tracker.consumer_num + MAX_UINT_32 + 1 == channel->local_channel.bd_ring_size)
            return OS_HETERO_FAIL;
    return OS_HETERO_SUCCESS;
}

static inline void osVerifyChannelProducerInitialized(os_ipc_channel_t *channel)
{
    if(channel->local_channel.producer_initialized != OS_HET_INITIALIZED)
    {
        //it is possible the producer initialized the channel just before sending a message
        if(channel->heterogeneous_channel->producer_initialized != OS_HET_INITIALIZED)
            OS_ASSERT;
        else
            channel->local_channel = *(channel->heterogeneous_channel);
    }
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_INITIALIZED);
}

static inline void osVerifyChannelConsumerInitialized(os_ipc_channel_t *channel)
{
    if(channel->local_channel.consumer_initialized != OS_HET_INITIALIZED)
    {
        //it is possible the consumer initialized the channel just before sending a message
        if(channel->heterogeneous_channel->consumer_initialized != OS_HET_INITIALIZED)
            OS_ASSERT;
        else
            channel->local_channel = *(channel->heterogeneous_channel);
    }
}

static inline void osCacheInvalidateAsync(void* data_pointer, uint32_t data_length, os_ipc_channel_t *channel)
{
    uint32_t         cache_addr_lo, cache_addr_hi;
 
    /* start synch of cache from memory*/
    /* pay attention - we flush surroundings too */
    cache_addr_lo = CACHE_OP_LOW_ADDR(data_pointer, ARCH_CACHE_LINE_SIZE);
    if (data_length != 0)
        cache_addr_hi = CACHE_OP_HIGH_ADDR(data_pointer, data_length, ARCH_CACHE_LINE_SIZE);
    else
        cache_addr_hi = CACHE_OP_HIGH_ADDR(data_pointer, channel->local_channel.max_msg_size, ARCH_CACHE_LINE_SIZE);
 
    cacheL1L2DataInvalidateAsync((int)channel->cache_policy, (void*)cache_addr_lo,(void*)cache_addr_hi,0);
 
}

static inline bool osIsChannelEmpty(os_ipc_channel_t *channel)
{
    if (OS_HET_CH_EMPTY(&channel->local_channel))
    {
        //procucer_num may have been changed by consumer
        channel->local_channel.tracker.producer_num = channel->heterogeneous_channel->tracker.producer_num;
        if (OS_HET_CH_EMPTY(&channel->local_channel))
        {
            osHwiSwiftEnable();
            return 1;
        }
    }
    return 0;
}

os_het_status_t osIpcMessagePtrGet(void *ch, void **data)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->producer_index == osGetCoreID());
    OS_ASSERT_COND(channel->local_channel.producer_initialized == OS_HET_INITIALIZED);
    osVerifyChannelConsumerInitialized(channel);
    OS_ASSERT_COND(channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH);
 
    osHwiSwiftDisable();
 
    //consumer_num may have been changed by consumer
    channel->local_channel.tracker.consumer_num = channel->heterogeneous_channel->tracker.consumer_num;
 
    if (osIpcMessageChannelPointerAvailableCheck(channel) != OS_HETERO_SUCCESS)
    {
        //consumer_num may have been changed by consumer
        channel->local_channel.tracker.consumer_num = channel->heterogeneous_channel->tracker.consumer_num;
        if (osIpcMessageChannelPointerAvailableCheck(channel) != OS_HETERO_SUCCESS)
        {
            //no assertion here, as this situation may happen during normal usage
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
    }

    *data = (void*)(((*channel->local_channel.bd_base)[channel->producer_pointer_bd_index++]).msg_ptr - channel->virt_to_phys);
    if (channel->producer_pointer_bd_index == channel->local_channel.bd_ring_size)
        channel->producer_pointer_bd_index = 0;
    channel->producer_pointers_tracker++;

    osHwiSwiftEnable();
    return OS_HETERO_SUCCESS;
}

os_het_status_t osIpcMessageSendPtr(void *ch, void *data, uint32_t length, bool indication)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
    os_het_status_t   status = OS_HETERO_SUCCESS;
    uint32_t          cache_addr_lo, cache_addr_hi;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->producer_index == osGetCoreID());
    OS_ASSERT_COND(channel->local_channel.producer_initialized == OS_HET_INITIALIZED);
    OS_ASSERT_COND(IS_ALIGNED(data, IPC_DEFAULT_ALIGNMENT));
    osVerifyChannelConsumerInitialized(channel);
    osHwiSwiftDisable();
    //if message channel make sure osIpcMessagePtrGet was called before this function
    if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH)
        if(channel->producer_pointers_tracker == channel->local_channel.tracker.producer_num)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
 
    if (OS_HET_CH_FULL(&channel->local_channel))
    {
        //consumer_num may have been changed by consumer
        channel->local_channel.tracker.consumer_num = channel->heterogeneous_channel->tracker.consumer_num;
        if (OS_HET_CH_FULL(&channel->local_channel))
        {
            //no assertion here, as this situation may happen during normal usage
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
    }
 
    if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH)
    {
        OS_ASSERT_COND((void*)((uint32_t)data + channel->virt_to_phys) == (void*)((*channel->local_channel.bd_base)[channel->producer_bd_index]).msg_ptr);
    }
    else
    {
        ((*channel->local_channel.bd_base)[channel->producer_bd_index]).msg_ptr = (uintptr_t)data + (uintptr_t)channel->virt_to_phys;
    }
 
    if ((channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH) && (length > channel->local_channel.max_msg_size))
    {
        OS_ASSERT;
        osHwiSwiftEnable();
        return OS_HETERO_FAIL;
    }
    ((*channel->local_channel.bd_base)[channel->producer_bd_index]).msg_len = length;
 
    if (channel->cache_policy != NOT_CACHED)
    {
        /* start synch of memory from cache */
        /* pay attention - we flush surroundings too */
        cache_addr_lo = CACHE_OP_LOW_ADDR(data, ARCH_CACHE_LINE_SIZE);
        if (length != 0)
            cache_addr_hi = CACHE_OP_HIGH_ADDR(data, length, ARCH_CACHE_LINE_SIZE);
        else
            cache_addr_hi = CACHE_OP_HIGH_ADDR(data, channel->local_channel.max_msg_size, ARCH_CACHE_LINE_SIZE);
 
        cacheL1L2DataSynchAsync((int)channel->cache_policy, (void*)cache_addr_lo,(void*)cache_addr_hi,0);
    }

    channel->producer_bd_index++;
    if (channel->producer_bd_index == channel->local_channel.bd_ring_size)
        channel->producer_bd_index = 0;
    OS_HET_INCREMENT_PRODUCER(&channel->local_channel);
    channel->heterogeneous_channel->tracker.producer_num = channel->local_channel.tracker.producer_num;
 
    if (indication == IPC_SEND_INDICATION)
    {
        status = osIpcIndicationSend(ch);
    }
 
#ifdef ENABLE_DEBUG_HOOKS
    {
        os_het_ipc_bd_t   debug_hook_bd;
        debug_hook_bd.msg_ptr = (uintptr_t)((uintptr_t)data + channel->virt_to_phys);
        debug_hook_bd.msg_len = length;
        if (debug_hook_functions[OS_DEBUG_IPC_BASIC_SEND])
            debug_hook_functions[OS_DEBUG_IPC_BASIC_SEND]((debug_hook_arg)channel->heterogeneous_channel, (debug_hook_arg)&debug_hook_bd);
    }
#endif

    osHwiSwiftEnable();
    return status;
}

void osIpcMessageReceiveCb(os_hwi_arg ch)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
    void*            data_pointer;
    uint32_t         data_length;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->consumer_index == osGetCoreID());
    osVerifyChannelProducerInitialized(channel);
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_INITIALIZED);
    osHwiSwiftDisable();
 
    if (osIsChannelEmpty(channel))
        return;
 
    data_pointer = (void*)(((*channel->local_channel.bd_base)[channel->consumer_bd_index]).msg_ptr - channel->virt_to_phys);
    OS_ASSERT_COND(IS_ALIGNED(data_pointer, IPC_DEFAULT_ALIGNMENT));
    data_length = ((*channel->local_channel.bd_base)[channel->consumer_bd_index]).msg_len;
 
#ifdef ENABLE_DEBUG_HOOKS
    {
        os_het_ipc_bd_t  debug_hook_bd;
        debug_hook_bd.msg_ptr = (uintptr_t)data_pointer;
        debug_hook_bd.msg_len = data_length;
        if (debug_hook_functions[OS_DEBUG_IPC_BASIC_RECEIVE])
            debug_hook_functions[OS_DEBUG_IPC_BASIC_RECEIVE]((debug_hook_arg)channel->heterogeneous_channel, (debug_hook_arg)&debug_hook_bd);
    }
#endif
 
    if (channel->callback != NULL)
    {
        OS_ASSERT_COND(data_length <= channel->local_channel.max_msg_size);
        if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH )
            channel->rcv_callback_is_running = TRUE;
 
        if (channel->cache_policy != NOT_CACHED)
            osCacheInvalidateAsync(data_pointer, data_length, channel);
 
        //user callback function should be run by default with interrupts enabled
        osHwiSwiftEnable();
        channel->callback(channel, data_pointer, data_length);
        osHwiSwiftDisable();
 
        if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH )
            channel->rcv_callback_is_running = FALSE;
    }
 
    channel->consumer_bd_index++;
    if (channel->consumer_bd_index == channel->local_channel.bd_ring_size)
        channel->consumer_bd_index = 0;
    OS_HET_INCREMENT_CONSUMER(&channel->local_channel);
    channel->heterogeneous_channel->tracker.consumer_num = channel->local_channel.tracker.consumer_num;
 
    osHwiSwiftEnable();
}

os_het_status_t osIpcChannelPeek(void* ch, void **data_pointer, uint32_t *data_length)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->consumer_index == osGetCoreID());
    osVerifyChannelProducerInitialized(channel);
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_INITIALIZED);
 
    osHwiSwiftDisable();
 
    if (osIsChannelEmpty(channel))
        return OS_HETERO_FAIL;
 
    *data_pointer = (void*)(((*channel->local_channel.bd_base)[channel->consumer_bd_index]).msg_ptr - channel->virt_to_phys);
    *data_length = ((*channel->local_channel.bd_base)[channel->consumer_bd_index]).msg_len;
    OS_ASSERT_COND(*data_length <= channel->local_channel.max_msg_size);
    if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH )
        channel->rcv_callback_is_running = TRUE;

    if (channel->cache_policy != NOT_CACHED)
        osCacheInvalidateAsync(*data_pointer, *data_length, channel);
 
    osHwiSwiftEnable();
    return OS_HETERO_SUCCESS;
 
}

os_het_status_t osIpcMessageChannelBufferReplace(void* ch)
{
    void             *addr;
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->consumer_index == osGetCoreID());
    OS_ASSERT_COND(channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH);
    //this function can only be called during application receive callback function
    OS_ASSERT_COND(channel->rcv_callback_is_running == TRUE);
 
    osHwiSwiftDisable();
    addr = osMemBlockSyncGet(channel->buffers_pool);
    if (addr == NULL)
    {
        OS_ASSERT;
        osHwiSwiftEnable();
        return OS_HETERO_FAIL;
    }
 
    addr = (void*)((uint32_t)addr + (uint32_t)channel->virt_to_phys);
 
    (*channel->local_channel.bd_base)[channel->consumer_bd_index].msg_ptr = (uintptr_t)(addr);
 
    osHwiSwiftEnable();
    return OS_HETERO_SUCCESS;
}

os_het_status_t osIpcMessageChannelBufferRelease(void* ch, void* ptr)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->local_channel.producer_initialized == OS_HET_INITIALIZED);
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_INITIALIZED);
    OS_ASSERT_COND(channel->consumer_index == osGetCoreID());
    OS_ASSERT_COND(channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH);
 
    ptr = (void*)((uint32_t)ptr - (uint32_t)channel->virt_to_phys);
    osMemBlockSyncFree(channel->buffers_pool, ptr);
 
    return OS_HETERO_SUCCESS;
}


os_het_status_t osIpcIndicationSend(void *ch)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
    uint32_t          indication_addr = 0;
 
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->local_channel.producer_initialized == OS_HET_INITIALIZED);
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_INITIALIZED);
    OS_ASSERT_COND(channel->producer_index == osGetCoreID());
 
    switch (channel->local_channel.ipc_ind)
    {
        case OS_HET_VIRTUAL_INT:
            indication_addr = (uint32_t)&g_dsp_ccsr_map->gic + channel->local_channel.ind_offset;
            WRITE_SYNCIO_UINT32(*((uint32_t*)indication_addr), channel->local_channel.ind_value);
            break;
        case OS_HET_PA_MPIC:
            indication_addr = (uint32_t)&g_dsp_pa_ccsr_map->pic + channel->local_channel.ind_offset;
            WRITE_SYNCIO_UINT32(*((uint32_t*)indication_addr), channel->local_channel.ind_value);
            break;
        case OS_HET_DSP_MESH:
            indication_addr = (uint32_t)&g_dsp_plat_map->mmu + channel->local_channel.ind_offset;
            WRITE_SYNCIO_UINT32(*((uint32_t*)indication_addr), channel->local_channel.ind_value);
            WRITE_SYNCIO_UINT32(*((uint32_t*)indication_addr), 0);
            break;
        default:
            OS_ASSERT;
            break;
    }
 
    return OS_HETERO_SUCCESS;
}


os_het_status_t osIpcConsumerIndexGet(void *ch, uint32_t *index)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;
    OS_ASSERT_COND(channel != NULL);

    if(channel->local_channel.consumer_initialized != OS_HET_INITIALIZED)
    {
        //it is possible the consumer initialized the channel up to this point
        if(channel->heterogeneous_channel->consumer_initialized != OS_HET_INITIALIZED)
            OS_ASSERT;
        else
            channel->local_channel = *(channel->heterogeneous_channel);
    }

    if(channel->local_channel.producer_initialized != OS_HET_INITIALIZED)
    {
        //it is possible the producer initialized the channel up to this point
        if(channel->heterogeneous_channel->producer_initialized != OS_HET_INITIALIZED)
            OS_ASSERT;
        else
            channel->local_channel = *(channel->heterogeneous_channel);
    }
 
    *index = channel->heterogeneous_channel->tracker.consumer_num;
    return OS_HETERO_SUCCESS;
}

os_het_status_t osIpcChannelNumGet(void *ch, uint32_t *id)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)ch;

    OS_ASSERT_COND(channel != NULL);
    *id = channel->local_channel.id;
    return OS_HETERO_SUCCESS;
}
 
