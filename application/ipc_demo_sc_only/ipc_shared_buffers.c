/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/12/26 13:36:02 $
 $Id: ipc_shared_buffers.c,v 1.7 2012/12/26 13:36:02 b08551 Exp $
 $Source: /cvsdata/SmartDSP/demos/starcore/psc9x3x/ipc_demo_sc_only/ipc_shared_buffers.c,v $
 $Revision: 1.7 $
******************************************************************************/

/******************************************************************************
 
 @File          cpri_basic.c

 @Description

 
*//***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smartdsp_os.h"
#include "os_config.h"
#include "os_runtime.h"
#include "app_config.h"
#include "psc913x_heterogeneous.h"
#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_ipc.h"
#include "psc913x_ipc_init.h"

#pragma data_seg_name ".os_shared_data"
#pragma bss_seg_name  ".os_shared_data_bss"

os_het_ipc_bd_t bd1[MAX_BD_SIZE] = {0};
#if (IPC_CHANNELS_NUMBER > 1)
os_het_ipc_bd_t bd2[MAX_BD_SIZE] = {0};
#endif //(IPC_CHANNELS_NUMBER > 1)


os_het_ipc_channel_t het_channels[IPC_CHANNELS_NUMBER] =
{
     {
         OS_HET_UNINITIALIZED,      /**<  producer_initialized; */
         OS_HET_UNINITIALIZED,      /**<  consumer_initialized; */
         CHANNEL_ID_1,              /**< id */
         0,0,                       /**< tracker;*/
         MAX_BD_SIZE,               /**< bd_ring_size;*/
         0,                         /**< max_msg_size;*/
         0,                         /**< ch_type;*/
         (os_het_ipc_bd_t(*)[])&bd1,/**< (*bd_base)[];*/
         0,                         /**< ipc_ind;*/
         0,                         /**< ind_offset;*/
         0,                         /**< ind_value;*/
         0,                         /**< pa_reserved[2];*/
         NULL,                      /**< *semaphore_pointer;*/
     }
#if (IPC_CHANNELS_NUMBER > 1)
     ,{
        OS_HET_UNINITIALIZED,      /**<  producer_initialized; */
        OS_HET_UNINITIALIZED,      /**<  consumer_initialized; */
        CHANNEL_ID_2,              /**< id */
        0,0,                       /**< tracker;*/
        MAX_BD_SIZE,               /**< bd_ring_size;*/
        0,                         /**< max_msg_size;*/
        0,                         /**< ch_type;*/
        (os_het_ipc_bd_t(*)[])&bd2,/**< (*bd_base)[];*/
        0,                         /**< ipc_ind;*/
        0,                         /**< ind_offset;*/
        0,                         /**< ind_value;*/
        0,                         /**< pa_reserved[2];*/
        NULL,                      /**< *semaphore_pointer;*/
 
     }
#endif //(IPC_CHANNELS_NUMBER > 1)
};

os_het_ipc_t ipc =
{
     IPC_CHANNELS_NUMBER,
     MAX_BD_SIZE,
     (os_het_ipc_channel_t(*)[])&het_channels,
};
os_het_smartdsp_log_t smartsdp_log[SOC_MAX_NUM_OF_CORES];
os_het_control_t het_control =
{
    {
        OS_HET_INITIALIZED,     // pa_initialized
        OS_HET_UNINITIALIZED    // sa_initialized
    },
    0x2000,                     // shared_ctrl_size (8KB)
    /*  pa_shared_mem (16K) */
    {
         0x10000000,    // start_addr
         0x10000        // size
    },
    /*  sc_shared_mem (16K) */
    {
         0x10020000,    // start_addr
         0x00010000     // size
    },
    (os_het_ipc_t(*)[])&ipc,               /* os_het_ipc_t *ipc */
    NULL,               // *aic;
    (os_het_smartdsp_log_t(*)[])&smartsdp_log                // *sdos_debug
};


