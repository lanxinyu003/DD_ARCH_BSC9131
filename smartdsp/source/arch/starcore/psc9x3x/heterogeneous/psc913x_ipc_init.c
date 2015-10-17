/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/04/04 13:12:59 $
 $Id: psc913x_ipc_init.c,v 1.44 2013/04/04 13:12:59 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/heterogeneous/psc913x_ipc_init.c,v $
 $Revision: 1.44 $
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
#include "os_datatypes.h"
#include "os_virq.h"
#include "os_cache.h"
#include "sc3x00_cache_.h"
#include "psc913x_heterogeneous.h"
#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_ipc_.h"
#include "psc913x_ipc.h"
#include "psc913x_ipc_init.h"


extern os_het_control_t  *g_os_het_control;

uint32_t                    num_ipc_regions;
os_ipc_region_channels_t    (*channels)[] __attribute__ ((section(".os_shared_data_bss")));                 //heterogeneous channels
uint32_t                    (*ipc_channels_number)[] __attribute__ ((section(".os_shared_data_bss")));      //heterogeneous channels

os_ipc_channel_t            (*dsp_channels)[] __attribute__ ((section(".os_shared_data_bss")));             //dsp to dsp channels
uint32_t                    ipc_dsp_channels_number __attribute__ ((section(".os_shared_data_bss")));  //dsp to dsp channels

os_het_status_t osIpcInit(void* params)
{
    uint32_t channel_num, channel_region_index;
    os_ipc_channel_t    (*region_channels)[];
    os_het_ipc_channel_t *het_channel;
    os_ipc_channel_t     *channel;
    os_mem_type          mem_type;
    uint32_t             num_ipc_chnls;
    os_het_ipc_t         *ipc_instance;
 
 
    os_ipc_dsp_t         *dsp_ipc;

    if (OS_MULTICORE == ON)
        mem_type = OS_MEM_SHARED;
    else //single core
        mem_type = OS_MEM_LOCAL;
 
    if (g_os_het_control->initialized.pa_initialized == OS_HET_INITIALIZED)
    {
        num_ipc_regions = OS_IPC_SINGLE_MODE_REGIONS_NUMBER;
    }
    else if (g_os_het_control->initialized.pa_initialized == OS_HET_INITIALIZED_MULTIMODE)
    {
        num_ipc_regions = g_os_het_control->num_ipc_regions;
        OS_ASSERT_COND(num_ipc_regions != 0);
    }
    else
        OS_ASSERT;
 
    /*initialize hetrogeneous channels if defined*/
    if ((osGetCoreID() == osGetMasterCore()) && (g_os_het_control->ipc != NULL))
    {
        channels = (os_ipc_region_channels_t(*)[]) osAlignedMalloc((sizeof(os_ipc_region_channels_t) * num_ipc_regions) ,mem_type, IPC_DEFAULT_ALIGNMENT);
        OS_ASSERT_COND(channels != NULL);

        ipc_channels_number = (uint32_t(*)[]) osAlignedMalloc((sizeof(uint32_t) * num_ipc_regions) ,mem_type, IPC_DEFAULT_ALIGNMENT);
        OS_ASSERT_COND(ipc_channels_number != NULL);
 
        for (channel_region_index = 0 ; channel_region_index < num_ipc_regions ; channel_region_index++)
        {
            ipc_instance = &(*(g_os_het_control->ipc))[channel_region_index];
            num_ipc_chnls = ipc_instance->num_ipc_channels;
            (*ipc_channels_number)[channel_region_index] = num_ipc_chnls;
 
            region_channels = (os_ipc_channel_t(*)[]) osAlignedMalloc((sizeof(os_ipc_channel_t) * num_ipc_chnls) ,mem_type, IPC_DEFAULT_ALIGNMENT);
            OS_ASSERT_COND(region_channels != NULL);
 
            (*channels)[channel_region_index].region_channels = region_channels;
 
            for (channel_num = 0 ; channel_num < num_ipc_chnls ; channel_num++)
            {
                channel = &((*region_channels)[channel_num]);
                het_channel = &((*ipc_instance->ipc_channels)[channel_num]);
                channel->heterogeneous_channel  = het_channel;
                channel->local_channel          = *het_channel;
                channel->is_producer_sc         = FALSE; //to be initialized in channel producer open
                channel->is_consumer_sc         = FALSE; //to be initialized in channel consumer open
                channel->cache_policy           = NOT_CACHED; //default value. can be changed by osIpcChannelConsumerOpen or osIpcChannelProducerOpen
                channel->channel_region_index   = channel_region_index;
            }
        }
    }
 
    /*initialize DSP only channels if defined*/
    if ((osGetCoreID() == osGetMasterCore()) && (params != NULL))
    {
        dsp_ipc = (os_ipc_dsp_t*)params;
        if (osGetCoreID() == osGetMasterCore())
        {
 
            ipc_dsp_channels_number = dsp_ipc->num_ipc_channels;
            dsp_channels = (os_ipc_channel_t(*)[]) osAlignedMalloc((sizeof(os_ipc_channel_t) * ipc_dsp_channels_number) ,mem_type, IPC_DEFAULT_ALIGNMENT);
            OS_ASSERT_COND(dsp_channels != NULL);
 
            for (channel_num = 0 ; channel_num < ipc_dsp_channels_number ; channel_num++)
            {
                channel = &((*dsp_channels)[channel_num]);
 
                channel->heterogeneous_channel              = &(channel->local_channel); //points to itself since there is no heterogeneous copy of the channel
                channel->local_channel.id                   = (*dsp_ipc->ipc_dsp_channels)[channel_num].id;
                channel->local_channel.bd_ring_size         = (*dsp_ipc->ipc_dsp_channels)[channel_num].bd_ring_size;
                channel->local_channel.bd_base              = (*dsp_ipc->ipc_dsp_channels)[channel_num].bd_base;
                channel->is_producer_sc                     = FALSE; //to be initialized in channel producer open
                channel->is_consumer_sc                     = FALSE; //to be initialized in channel consumer open
                channel->cache_policy                       = NOT_CACHED; //default value. can be changed by osIpcChannelConsumerOpen or osIpcChannelProducerOpen
                channel->channel_region_index               = 0; //only a single region for DSP-DSP communication
            }
        }
    }
 
    osWaitForAllCores();
    return OS_HETERO_SUCCESS;
}

void* osIpcMultimodeChannelIdFind(uint32_t id, uint32_t channel_region_index)
{
    os_ipc_channel_t     *channel;
    uint32_t             channel_num;
    os_ipc_channel_t    (*region_channels)[];
 
    OS_ASSERT_COND(channel_region_index <= num_ipc_regions);
    region_channels = (*channels)[channel_region_index].region_channels;
 
    for (channel_num = 0 ; channel_num < (*ipc_channels_number)[channel_region_index] ; channel_num++)
    {
        channel = &(*region_channels)[channel_num];
        OS_ASSERT_COND(channel != NULL);
        if (channel->local_channel.id == id)
            return channel;
    }
    return NULL;
}


void* osIpcChannelIdFind(uint32_t id)
{
    OS_ASSERT_COND(num_ipc_regions == OS_IPC_SINGLE_MODE_REGIONS_NUMBER); //for multimode, need to use osIpcMultimodeChannelIdFind
    return osIpcMultimodeChannelIdFind(id, OS_IPC_SINGLE_MODE_REGION_INDEX);
}

void* osIpcDspChannelIdFind(uint32_t id)
{
    os_ipc_channel_t     *channel;
    uint32_t channel_num;
 
    for (channel_num = 0 ; channel_num < ipc_dsp_channels_number ; channel_num++)
    {
        channel = &((*dsp_channels)[channel_num]);
        OS_ASSERT_COND(channel != NULL);
        if (channel->local_channel.id == id)
            return channel;
    }
    return NULL;
}


os_het_status_t osIpcChannelConsumerOpen(os_ipc_channel_consumer_open_params_t *params)
{
    os_ipc_channel_t *channel;
    uint32_t         msg_num, i;
    void             *virt_addr = NULL, *phys_addr = NULL;
    os_status        status;
    uint32_t         interrupt_handle = 0, interrupt_index;
    bool             interrupt_created = 0;
 
    OS_ASSERT_COND(params != NULL);
 
    channel = (os_ipc_channel_t*)params->ch;
    OS_ASSERT_COND(channel != NULL);
    OS_ASSERT_COND(channel->local_channel.consumer_initialized == OS_HET_UNINITIALIZED);
    OS_ASSERT_COND((params->callback != NULL) || (params->indication_type == OS_HET_NO_INT));
 
    osHwiSwiftDisable();
    channel->consumer_index = (uint32_t)osGetCoreID();
    channel->is_consumer_sc = TRUE;
    channel->local_channel.ch_type = channel->heterogeneous_channel->ch_type = params->channel_type;
    if (params->channel_type == OS_HET_IPC_MESSAGE_CH)
    {
        channel->producer_pointers_tracker = 0;
        channel->producer_pointer_bd_index = 0;
    }
    channel->local_channel.ipc_ind = channel->heterogeneous_channel->ipc_ind = params->indication_type;
    if (num_ipc_regions == OS_IPC_SINGLE_MODE_REGIONS_NUMBER) //single mode
    {
        OS_ASSERT_COND(params->bd_ring_size <= (&(*(g_os_het_control->ipc))[OS_IPC_SINGLE_MODE_REGION_INDEX])->ipc_max_bd_size);
    }
    else //multi mode
    {
        OS_ASSERT_COND(params->bd_ring_size <= (&(*(g_os_het_control->ipc))[channel->channel_region_index])->ipc_max_bd_size);
    }

    channel->local_channel.bd_ring_size = channel->heterogeneous_channel->bd_ring_size = params->bd_ring_size;
    channel->local_channel.max_msg_size = channel->heterogeneous_channel->max_msg_size = params->max_msg_size;
    channel->local_channel.tracker.producer_num = channel->heterogeneous_channel->tracker.producer_num = 0;
    channel->local_channel.tracker.consumer_num = channel->heterogeneous_channel->tracker.producer_num = 0;
    channel->producer_bd_index = channel->consumer_bd_index = 0;
    channel->callback = params->callback;
    if (channel->cache_policy != NOT_CACHED)
    {
         OS_ASSERT_COND(channel->cache_policy == params->cache_policy);
    }
    else
        channel->cache_policy = params->cache_policy;
 
    OS_ASSERT_COND(params->indication_type != OS_HET_PA_MPIC)
    switch (params->indication_type)
    {
    case OS_HET_VIRTUAL_INT:
        status = osVirtualInterruptFind(&interrupt_handle);
        if(status != OS_SUCCESS)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
 
        status = osHwiCreate((os_hwi_handle)(interrupt_handle),
            params->int_priority,
            EDGE_MODE,
            &osIpcMessageReceiveCb,
            (os_hwi_arg)channel);

        if(status != OS_SUCCESS)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
 
        interrupt_index = interrupt_handle - OS_INT_VIRQ0;
        channel->local_channel.ind_offset  = channel->heterogeneous_channel->ind_offset = (uint32_t)&g_dsp_ccsr_map->gic.vigr - (uint32_t)&g_dsp_ccsr_map->gic;
        channel->local_channel.ind_value  = channel->heterogeneous_channel->ind_value  = IPC_VIRQ_GEN_VALUE(interrupt_index);
        break;
 
    case OS_HET_DSP_MESH:
        for (msg_num = 0 ; msg_num < SOC_MAX_NUM_OF_MESSAGES ; msg_num++)
        {
            interrupt_index = IPC_MESH_INT_NUM(channel->producer_index, msg_num);
            status = osHwiIsCreated((os_hwi_handle)interrupt_index, &interrupt_created);
            OS_ASSERT_COND(status == OS_SUCCESS);
            if (interrupt_created == FALSE)
            {
                status = osHwiCreate((os_hwi_handle)(interrupt_index),
                    params->int_priority,
                    EDGE_MODE,
                    &osIpcMessageReceiveCb,
                    (os_hwi_arg)channel);
                if(status != OS_SUCCESS)
                {
                    OS_ASSERT;
                    osHwiSwiftEnable();
                    return OS_HETERO_FAIL;
                }
                break;
            }
        }
        if (interrupt_created == TRUE)
        {
            OS_ASSERT;
            osHwiSwiftEnable();
            return OS_HETERO_FAIL;
        }
 
        channel->local_channel.ind_offset = channel->heterogeneous_channel->ind_offset = (uint32_t)&g_dsp_plat_map->mmu.m_gpr0 - (uint32_t)&g_dsp_plat_map->mmu;
        channel->local_channel.ind_value  = channel->heterogeneous_channel->ind_value  = 1 << (SOC_MAX_NUM_OF_MESSAGES * (channel->consumer_index) + msg_num);
        break;
 
    case OS_HET_NO_INT:
        break;
 
    default:
        OS_ASSERT;
        osHwiSwiftEnable();
        return OS_HETERO_FAIL;
    }
 
    if (params->channel_type == OS_HET_IPC_MESSAGE_CH)
    {
        channel->buffers_pool = params->buffers_pool;
        OS_ASSERT_COND(params->buffers_pool != NULL);
        for (i = 0 ; i < channel->local_channel.bd_ring_size ; i++ )
        {
            virt_addr = osMemBlockSyncGet(params->buffers_pool);
            OS_ASSERT_COND(virt_addr != NULL);
            OS_ASSERT_COND(channel->local_channel.max_msg_size <= params->buffers_pool->block_size);
            status = osMmuDataVirtToPhys(virt_addr, &phys_addr);
            OS_ASSERT_COND(status == OS_SUCCESS);
            (*channel->local_channel.bd_base)[i].msg_ptr = (uintptr_t)(phys_addr);
        }
        channel->virt_to_phys = (uint32_t)phys_addr - (uint32_t)virt_addr;
    }
    channel->local_channel.consumer_initialized = channel->heterogeneous_channel->consumer_initialized = OS_HET_INITIALIZED;
    osHwiSwiftEnable();
    return OS_HETERO_SUCCESS;
}

os_het_status_t osIpcChannelProducerOpen(os_ipc_channel_producer_open_params_t *params)
{
    os_ipc_channel_t *channel = (os_ipc_channel_t*)params->ch;
 
    OS_ASSERT_COND(channel != NULL);
 
    if (num_ipc_regions == OS_IPC_SINGLE_MODE_REGIONS_NUMBER) //single mode
    {
        OS_ASSERT_COND(channel->local_channel.bd_ring_size <= (&(*(g_os_het_control->ipc))[OS_IPC_SINGLE_MODE_REGION_INDEX])->ipc_max_bd_size);
    }
    else //multi mode
    {
        OS_ASSERT_COND(channel->local_channel.bd_ring_size <= (&(*(g_os_het_control->ipc))[channel->channel_region_index])->ipc_max_bd_size);
    }
 
    OS_ASSERT_COND(channel->local_channel.producer_initialized == OS_HET_UNINITIALIZED);

    osHwiSwiftDisable();
    channel->producer_index = (uint32_t)osGetCoreID();
    channel->is_producer_sc = TRUE;
    if (channel->local_channel.ch_type == OS_HET_IPC_MESSAGE_CH)
        channel->producer_pointers_tracker = 0;
    channel->producer_bd_index = channel->consumer_bd_index = 0;
    if (channel->cache_policy != NOT_CACHED)
    {
        OS_ASSERT_COND(channel->cache_policy == params->cache_policy);
    }
    else
        channel->cache_policy = params->cache_policy;
 
    channel->local_channel.producer_initialized = channel->heterogeneous_channel->producer_initialized = OS_HET_INITIALIZED;
    osHwiSwiftEnable();
    return OS_HETERO_SUCCESS;
}

