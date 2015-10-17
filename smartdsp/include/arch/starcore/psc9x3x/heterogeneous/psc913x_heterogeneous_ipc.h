/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/10 12:50:54 $
 $Id: psc913x_heterogeneous_ipc.h,v 1.18 2013/02/10 12:50:54 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/heterogeneous/psc913x_heterogeneous_ipc.h,v $
 $Revision: 1.18 $
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

 @File          psc913x_heterogeneous_ipc.h

 @Description   IPC shared header file between SC and PA cores.

 @Cautions      Some functions MAY NOT be supported by all OS's; although they
                MUST provide at least one function for sending and one for
                receiving to/from the IPC channel.

*//***************************************************************************/

#ifndef __PSC913X_HETEROGENEOUS_IPC_H
#define __PSC913X_HETEROGENEOUS_IPC_H


#include "psc913x_heterogeneous_common.h"

_OS_BEGIN_EXTERN_C


/**************************************************************************//**
 @Description   Types of indications
 
                The IPC driver should use this to calculate the base address from
                which to generate the indication
 
*//***************************************************************************/
typedef enum
{
    OS_HET_VIRTUAL_INT = 0,
    /**< Type of indication is virtual interrupt; channel offset reletive to GIC */
    OS_HET_PA_MPIC,
    /**< Type of indication is MPIC message; channel offset reletive to MPIC */
    OS_HET_DSP_MESH,
    /**< Type of indication is by DSP mesh; channel offset reletive DSP subsystem address */
    OS_HET_NO_INT,
    /**< Type of indication is none */
    OS_HET_NUM_INDICATIONS
    /**< MUST BE LEFT LAST */
 
} os_het_ipc_ind_t;



#define MAX_UINT_32 0xFFFFFFFF
#define OS_HET_CH_EMPTY(CH) \
    ((CH)->tracker.producer_num == (CH)->tracker.consumer_num)
/**< Evaluate whether the channel is empty */

#define OS_HET_CH_FREE_BDS(CH) \
    (( ((CH)->tracker.producer_num >= (CH)->tracker.consumer_num)) ?    \
      ((CH)->bd_ring_size - ((CH)->tracker.producer_num - (CH)->tracker.consumer_num)) : \
      ((CH)->bd_ring_size - ((CH)->tracker.producer_num - (CH)->tracker.consumer_num + MAX_UINT_32 + 1)))
/**< Evaluate the number of free BD in the channel */

#define OS_HET_CH_FULL(CH)  (OS_HET_CH_FREE_BDS(CH) == 0)
/**< Evaluate whether the channel is full */

#define OS_HET_INCREMENT_INDEX(CH, INDEX)  ((CH)->tracker.INDEX)++
 
/**< Increment the producer/consumer index */

#define OS_HET_INCREMENT_CONSUMER(CH)   OS_HET_INCREMENT_INDEX(CH, consumer_num)
#define OS_HET_INCREMENT_PRODUCER(CH)   OS_HET_INCREMENT_INDEX(CH, producer_num)

#define OS_HET_UNSPECIFIED_LEN  0
/**< Use to indicate that the message consists of a pointer only */

/**************************************************************************//**
 @Description   IPC message descriptor
 
*//***************************************************************************/
typedef struct
{
    uintptr_t   msg_ptr;
    /**< Pointer to the message; as an offset from the base of the shared address space */
    uint32_t    msg_len;
    /**< Size of the message; may be set to OS_HET_UNSPECIFIED_LEN on
         OS_HET_IPC_POINTER_CH channel types only */
} os_het_ipc_bd_t;


/**************************************************************************//**
 @Description   Types of IPC channels
 
*//***************************************************************************/
typedef enum
{
    OS_HET_IPC_MESSAGE_CH = 0,
    /**< Indicates that the consumer will pre-populate the channel's
         ptr_bd_base[].msg_ptr entires; The producer MUST use the pre-populated
         pointers when producing messages on the channel */
    OS_HET_IPC_POINTER_CH = 1
    /** Indicated that the producer MUST provide pointers to the
        ptr_bd_base[].msg_ptr entries whenever a new messag in produced */
}os_het_ipc_ch_types_t;

/**************************************************************************//**
 @Description   IPC channel control structure
 
 @Cautions      Using the macros OS_HET_INCREMENT_CONSUMER() and OS_HET_INCREMENT_PRODUCER()
                is the recommended way of incrementing the tracker counters; Direct access
                should be avoided.
 
 
*//***************************************************************************/
typedef struct
{
    uint32_t                producer_initialized;
    /**< Indicates whether the mailbox is initialized by the producer;
         This field is written by the producer */
    uint32_t                consumer_initialized;
    /**< Indicates whether the mailbox is initialized by the consumer;
         This field is written by the consumer */
    uint32_t                id;
    /**< Mailbox ID - may be used by the applicaiton to identify the channel;
         This field is written by Linux during boot. Can be an arbitrary number
         provided that it is unique in the system */
    os_het_tracker_t        tracker;
    /**< Producer/consumer tracker;
         This field is set to {0} by Linux during boot */
    uint32_t                bd_ring_size;
    /**< Size of the mailbox BD ring;
         This field is set by the consumer. May not be larger than os_het_ipc_t.ipc_max_bd_size */
    uint32_t                max_msg_size;
    /**< Size (in Bytes) of the maximal message that can be passed on this channel;
         This field is set by the consumer ONLY if ch_type is OS_HET_IPC_MESSAGE_CH
         otherwise MUST be set to 0xFFFFFFFF */
    os_het_ipc_ch_types_t   ch_type;
    /**< Type of the channel; This field is set by the consumer */
    os_het_ipc_bd_t         (*bd_base)[];
    /**< Base of the mailbox pointer BD; as an offset from the base of the shared address space;
         For heterogeneous channels, this field is allocated by Linux during boot based on os_het_ipc_t.ipc_max_bd_size */
    os_het_ipc_ind_t        ipc_ind;
    /**< Type of indication to generate to the destination;
         This field is written by the consumer */
    uint32_t                ind_offset;
    /**< Offset address from base of specific indication register set;
         This field is written by the consumer */
    uint32_t                ind_value;
    /**< Value to write to ind_offset;
         This field is written by the consumer */
    uint32_t                pa_reserved[2];
    /**< reserved field for pa usage */
    void                    *semaphore_pointer;
    /**< Future compatibility semaphore pointer;
         This field is set to NULL by Linux during boot */
} os_het_ipc_channel_t;
 

/**************************************************************************//**
 @Description   IPC global control structure
 
*//***************************************************************************/
typedef struct
{
    uint32_t                num_ipc_channels;
    /**< Number of channels in the ipc_channels array;
         this is a bootarg to Linux and will be set by Linux accordingly */
    uint32_t                ipc_max_bd_size;
    /**< Maximal size that the ipc_channel.bd_ring_size may have;
         this is a bootarg to Linux and will be set by Linux accordingly */
    os_het_ipc_channel_t    (*ipc_channels)[];
    /**< Pointer to the mailboxes control structure array; has os_het_ipc_t.num_ipc_channels entries
         as an offset from the base of the shared address space */
} os_het_ipc_t;

_OS_END_EXTERN_C


#endif // __PSC913X_HETEROGENEOUS_IPC_H


