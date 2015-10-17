/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/01/15 14:07:36 $
 $Id: psc913x_ipc.h,v 1.22 2013/01/15 14:07:36 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/ipc/psc913x_ipc.h,v $
 $Revision: 1.22 $
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

 @File          psc913x_ipc.h

 @Description   IPC sdos header file holds the sdos private functionality and data structures
                (not shared with PA). control structure will be copied to local cacheable memory
                for faster run time.

 @Cautions      Some functions MAY NOT be supported by all OS's; although they
                MUST provide at least one function for sending and one for
                receiving to/from the IPC channel.

*//***************************************************************************/

#ifndef __PSC913X_HET_IPC_H
#define __PSC913X_HET_IPC_H

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

#define IPC_SEND_INDICATION         1
#define IPC_DO_NOT_SEND_INDICATION  0

typedef struct
{
    os_het_ipc_channel_t*   heterogeneous_channel;
    /**< pointer to heterogeneous shared channel */
    os_het_ipc_channel_t    local_channel;
    /**< copy of channel in local cacheable memory */
    uint32_t                producer_bd_index;
    /**< index of current producer position in BD */
    uint32_t                consumer_bd_index;
    /**< index of current consumer position in BD */
    uint32_t                producer_pointers_tracker;
    /**< producer tracker - relevant for message channels only
     should be used to know number of pointers given for unsent messages */
    int32_t                 producer_pointer_bd_index;
    /**< relevant for message channels only
     index of current producer pointer in BD */
    uint32_t                producer_index;
    /**< producer core index */
    uint32_t                consumer_index;
    /**< consumer core index */
    uint32_t                cache_policy;
    /**< caching policy to use in the cannel according to os_cache.h defines*/
    struct
    {
        uint32_t            is_producer_sc:1;
        /**< TRUE if producer is SC */
        uint32_t            is_consumer_sc:1;
        /**< TRUE if consumer is SC */
        uint32_t            rcv_callback_is_running:1;
        /**< only relevant for a message channel, tells if receive callback is currently running
         * osIpcMessageChannelBufferReplace can be called only if this variable is TRUE in order
         * to be clear which buffer should be replaced*/
    };
    void                    (*callback)(void* ch, void* data, uint32_t length);
    /**< callback to be used when receiving a message */
    int64_t                 virt_to_phys;
    /**< only relevant for a message channel, with SC as consumer
     * should be added for virtual to physical conversion
     * should be reduced for physical to virtual conversion */
    os_mem_part_t           *buffers_pool;
    /**< only relevant for a message channel, buffers pool to get buffers from */
    uint32_t                channel_region_index;
    /*number of IPC channels regions - can be more than 1 if running with multimode mode*/
} os_ipc_channel_t;

typedef struct
{
    os_ipc_channel_t (*region_channels)[];
} os_ipc_region_channels_t;

/**************************************************************************//**
 @Function      osIpcMessagePtrGet

 @Description   Get a pointer for sending a message on a "message channel"
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[out]    data           - Pointer to the data.
 
 @Retval        OS_HETERO_SUCCESS if succeedded in sending message
 @Retval        OS_HETERO_FAIL    if failed in sending message

 @Cautions      to be used on a "message channel" only. must be used before each
                call to osIpcMessageSendPtr
 
*//***************************************************************************/
os_het_status_t osIpcMessagePtrGet(
        void                    *ch,
        void                    **data
        );

/**************************************************************************//**
 @Function      osIpcMessageSendPtr

 @Description   Copy a message to the IPC channel
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[in]     data           - Pointer to the data (in message channel, the pointer
                                 received from osIpcMessagePtrGet)
 @Param[in]     length         - optional, the length of the data
 @Param[in]     indication     - Set to TRUE to send indication or FALSE no to.

 @Retval        OS_HETERO_SUCCESS if succeedded in sending message
 @Retval        OS_HETERO_FAIL    if failed in sending message

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcMessageSendPtr(
        void                    *ch,
        void                    *data,
        uint32_t                length,
        bool                    indication
        );

/**************************************************************************//**
 @Function      osIpcMessageReceiveCb

 @Description   Calls a callback function if a message exists on the channel

                This function incrememnts the consumer index after returning
                from the callback function.
                if callback is NULL, no calling is done (legal only in OS_HET_NO_INT channel)
 
 @Param[in]     ch             - Pointer to the relevant channel
 
 @Retval        OS_HETERO_SUCCESS if received a message
 @Retval        OS_HETERO_FAIL    if didn't receive a message

 @Cautions      None
 
*//***************************************************************************/
void osIpcMessageReceiveCb(os_hwi_arg ch);

/**************************************************************************//**
 @Function      osIpcChannelPeek

 @Description   peeks if channel contains a message to receive.
                if so, returns a pointer to it

                This function does not incrememnt the consumer index and does
                not call a callback function
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[out]    data_pointer   - Pointer to pointer to the buffer
 @Param[out]    data_length    - Pointer to the data length
 
 @Retval        OS_HETERO_SUCCESS if received a message
 @Retval        OS_HETERO_FAIL    if didn't receive a message

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcChannelPeek(void* ch, void **data_pointer, uint32_t *data_length);

/**************************************************************************//**
 @Function      osIpcMessageChannelPointerReplace

 @Description   relevant only for a message channel
                replace the current buffer received to a different buffer which
                will be used in the next time the current BD will be reached.
                This is useful when application receive a buffer and decides
                that it would like to keep the buffer, so it will not be overrun
 
 
 @Param[in]     ch             - Pointer to the relevant channel
 
 @Retval        OS_HETERO_SUCCESS if received a message
 @Retval        OS_HETERO_FAIL    if didn't receive a message

 @Cautions      can be called only during the running of the recieve callback function
 
*//***************************************************************************/
os_het_status_t osIpcMessageChannelBufferReplace(void* ch);

/**************************************************************************//**
 @Function      osIpcMessageChannelPointerRelease

 @Description   free a buffer that was kept by the application using
                osIpcMessageChannelPointerReplace
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[in]     ptr            - Pointer to the buffer to be released
 
 @Retval        OS_HETERO_SUCCESS if received a message
 @Retval        OS_HETERO_FAIL    if didn't receive a message

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcMessageChannelBufferRelease(void* ch, void* ptr);

/**************************************************************************//**
 @Function      osIpcIndicationSend

 @Description   Send an indication to the consumer
 
 @Param[in]     ch             - Pointer to the relevant channel

 @Retval        OS_HETERO_SUCCESS if indication was successfull
 @Retval        OS_HETERO_FAIL    if indication failed

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcIndicationSend(void *ch);


/**************************************************************************//**
 @Function      osIpcConsumerIndexGet

 @Description   Read the current consumer index
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[out]    index          - Pointer to where the index should be written

 @Retval        OS_HETERO_SUCCESS if succeeded
 @Retval        OS_HETERO_FAIL    if failed (illegal channel)

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcConsumerIndexGet(void *ch, uint32_t *index);


/**************************************************************************//**
 @Function      osIpcChannelIdGet

 @Description   Retrive the chanel ID
 
 @Param[in]     ch             - Pointer to the relevant channel
 @Param[out]    number         - Pointer to where the channel ID should be written

 @Retval        OS_HETERO_SUCCESS if succeeded
 @Retval        OS_HETERO_FAIL    if failed (illegal channel)

 @Cautions      None
 
*//***************************************************************************/
os_het_status_t osIpcChannelNumGet(void *ch, uint32_t *id);

/** @} */ //ipc_913x_id

/** @} */ // ipc_id

_OS_END_EXTERN_C


#endif // __PSC913X_HET_IPC_H


