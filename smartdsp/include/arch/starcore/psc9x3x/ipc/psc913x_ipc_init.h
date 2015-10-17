/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/20 14:34:37 $
 $Id: psc913x_ipc_init.h,v 1.20 2013/01/20 14:34:37 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/ipc/psc913x_ipc_init.h,v $
 $Revision: 1.20 $
******************************************************************************/

/*
 * Copyright (c) 2011
 *  Freescale Semiconductor Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Freescale Semiconductor Inc nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */


/******************************************************************************

 @File          psc913x_ipc_init.h

 @Description   IPC shared header file between SC and PA cores.

 @Cautions      Some functions MAY NOT be supported by all OS's; although they
                MUST provide at least one function for sending and one for
                receiving to/from the IPC channel.

*//***************************************************************************/

#ifndef __PSC913X_HETEROGENEOUS_IPC_INIT_H
#define __PSC913X_HETEROGENEOUS_IPC_INIT_H

#include "psc913x_heterogeneous_common.h"
#include "os_cpp.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         ipc_id  IPC (inter process communication) API
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         ipc_913x_id  PSC913x IPC (inter process communication) API
 @{
*//***************************************************************************/

/* Types of indications */
typedef enum
{
    OS_IPC_PRODUCER = 0,
    OS_IPC_CONSUMER
} os_ipc_type;

#define OS_IPC_SINGLE_MODE_REGIONS_NUMBER    1
#define OS_IPC_SINGLE_MODE_REGION_INDEX      0

/**************************************************************************//**
 @Description   IPC channel consumer open parameters.

                This structure should be used for opening a channel when SDOS
                is the consumer .
*//***************************************************************************/
typedef struct
{
    void*                   ch;
    /**< Pointer to the relevant channel as received from osIpcChannelIdFind or osIpcMultimodeChannelIdFind*/
    os_het_ipc_ch_types_t   channel_type;
    /**< type of channel - message channel or pointer channel*/
    os_het_ipc_ind_t        indication_type;
    /**< the indication type to use*/
    os_hwi_priority         int_priority;
    /**< priority of interrupt, if used*/
    os_hwi_arg              int_argument;
    /**< argument of interrupt, if used*/
    uint32_t                bd_ring_size;
    /**< size of the mailbox BD ring*/
    uint32_t                max_msg_size;
    /**< maximal size of the message that can be passed on this channel*/
    void                    (*callback)(void* ch, void* data, uint32_t length);
    /**< callback function to call when receiving a message*/
    os_mem_part_t           *buffers_pool;
    /**< only for a message channel - the buffer manager to allocate
         the buffers from*/
    uint32_t                cache_policy;
    /**< caching policy to use in the cannel. according to os_cache.h defines*/
 
} os_ipc_channel_consumer_open_params_t;


/**************************************************************************//**
 @Description   IPC channel producer open parameters.

                This structure should be used for opening a channel when SDOS
                is the producer .
*//***************************************************************************/
typedef struct
{
    void*                   ch;
    /**< Pointer to the relevant channel as received from osIpcChannelIdFind or osIpcMultimodeChannelIdFind*/
    void*                   additional_params;
    /**< Pointer to future parameters for opening channel from Starcore to Starcore, should be NULL if not used*/
    uint32_t                cache_policy;
    /**< caching policy to use in the cannel. according to os_cache.h defines*/
} os_ipc_channel_producer_open_params_t;

/**************************************************************************//**
 @Description   IPC DSP to DSP channel parameter structure
 
                This structure should be used when defining a local channel to be used for
                DSP to DSP communication only
                For heterogeneous communication (DSP-PA) heterogeneous channels should be
                used (defined in heterogeneous control structure)
 
 
*//***************************************************************************/
typedef struct
{
    uint32_t                id;
    /**< Mailbox ID - may be used by the applicaiton to identify the channel;
         Can be an arbitrary number provided that it is unique in the ipc region */
    uint32_t                bd_ring_size;
    /**< Size of the mailbox BD ring;
         This field is set by the consumer. May not be larger than os_het_ipc_t.ipc_max_bd_size */
    os_het_ipc_bd_t         (*bd_base)[];
    /**< Base of the mailbox pointer BD; as an offset from the base of the shared address space;*/
} os_ipc_dsp_channel_t;

/**************************************************************************//**
 @Description   IPC DSP to DSP structure
 
                This structure should be passed in initialization parameters
                in order to initialize IPC for DSP-DSP communication
 
*//***************************************************************************/
typedef struct
{
    uint32_t                num_ipc_channels;
    /**< Number of channels in the ipc_channels array */
    uint32_t                ipc_max_bd_size;
    /**< Maximal size that the ipc_channel.bd_ring_size may have */
    os_ipc_dsp_channel_t    (*ipc_dsp_channels)[];
    /**< Pointer to the mailboxes control structure array; has os_het_ipc_t.num_ipc_channels entries
         as an offset from the base of the shared address space */
} os_ipc_dsp_t;

/**************************************************************************//**
 @Function      osIpcInit

 @Description   Initialize IPC. this function will check validiy of the IPC channels
                This function should be called by osInitialize
 
 @Param[in]     params - pointer for future enhancement parameters

 @Retval        OS_HETERO_SUCCESS if initializtion was done successfully
 @Retval        OS_HETERO_FAIL    if failed to initialize

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcInit(void* params);


/**************************************************************************//**
 @Function      osIpcMultimodeChannelIdFind

 @Description   search for a channel with specified ID in a specific IPC region. returns a pointer to the channel.
 
 @Param[in]     id  - channel ID for the needed channel
 @Param[in]     channel_region_index  - the channels region index to look for the channel on

 @Retval        Pointer to the relevant channel if was found successfully
 @Retval        NULL  if failed to find or already in use

 @Cautions      None
 
*//***************************************************************************/
void* osIpcMultimodeChannelIdFind(uint32_t id, uint32_t channel_region_index);

/**************************************************************************//**
 @Function      osIpcChannelIdFind

 @Description   search for a channel with specified ID. returns a pointer to the channel.
 
 @Param[in]     id  - channel ID for the needed channel

 @Retval        Pointer to the relevant channel if was found successfully
 @Retval        NULL  if failed to find or already in use

 @Cautions      None
 
*//***************************************************************************/
void* osIpcChannelIdFind(uint32_t id);

/**************************************************************************//**
 @Function      osIpcDspChannelIdFind

 @Description   search for a DSP-DSP channel with specified ID. returns a pointer to the channel.
 
 @Param[in]     id  - dsp channel ID for the needed channel

 @Retval        Pointer to the relevant channel if was found successfully
 @Retval        NULL  if failed to find or already in use

 @Cautions      None
 
*//***************************************************************************/
void* osIpcDspChannelIdFind(uint32_t id);


/**************************************************************************//**
 @Function      osIpcChannelConsumerOpen

 @Description   initialize a channel in which SDOS is a consumer.
 
 @Param[in]     params          - parameters for opening a channel as consumer
 
 @Retval        OS_HETERO_SUCCESS if initializtion was done successfully
 @Retval        OS_HETERO_FAIL    if failed to initialize

 @Cautions      If SC is the producer and consumer of a channel and MESH interrupt
                is used then osIpcChannelProducerOpen must be called before this function.
 
*//***************************************************************************/
os_het_status_t osIpcChannelConsumerOpen(os_ipc_channel_consumer_open_params_t *params);

/**************************************************************************//**
 @Function      osIpcChannelProducerOpen

 @Description   initialize a channel in which SDOS is a producer.
 
 @Param[in]     params          - parameters for opening a channel as producer
 
 @Retval        OS_HETERO_SUCCESS if initializtion was done successfully
 @Retval        OS_HETERO_FAIL    if failed to initialize

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcChannelProducerOpen(os_ipc_channel_producer_open_params_t *params);


/** @} */ //ipc_913x_id

/** @} */ // ipc_id

_OS_END_EXTERN_C


#endif // __PSC913X_HETEROGENEOUS_IPC_INIT_H


