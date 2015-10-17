/******************************************************************************
 Copyright � 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/12/26 13:36:03 $
 $Id: psc913x_heterogeneous.h,v 1.15 2012/12/26 13:36:03 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/heterogeneous/psc913x_heterogeneous.h,v $
 $Revision: 1.15 $
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

 @File          psc913x_heterogeneous.h

 @Description   Main shared header file between SC and PA cores.

 @Cautions      None.

*//***************************************************************************/

#ifndef __PSC913X_HETEROGENEOUS_H
#define __PSC913X_HETEROGENEOUS_H


#include "psc913x_heterogeneous_common.h"

_OS_BEGIN_EXTERN_C

#include "psc913x_heterogeneous_ipc.h"
#include "psc913x_heterogeneous_mem.h"
#include "psc913x_heterogeneous_debug.h"
#include "psc913x_heterogeneous_debug_print.h"


#define OS_HET_IPC_HW_SEMAPHORE_NUM     0
/**< Hardware semaphore to use in case of need for mutual exclusion in the IPC module */
#define OS_HET_BOOT_HW_SEMAPHORE_NUM    1
/**< Hardware semapohore for synchronizing the boot processes between SC and PA */


/**************************************************************************//**
 @Description   Heterogeneous OS global control structure
 
*//***************************************************************************/
typedef struct
{
    os_het_init_t           initialized;
    /**< Initialization indication strcuture */
    uint32_t                shared_ctrl_size;
    /**< Size of the shared memory for control information in bytes -
         starts at the base of the PA managed shared memory.
         Mimumum (and default) size is 4 KB */
    os_het_mem_t            pa_shared_mem;
    /**< PA shared memory region */
    os_het_mem_t            sc_shared_mem;
    /**< SC shared memory region */
    os_het_ipc_t            (*ipc)[];
    void                    *aic;
    /**< Pointer to shared AIC configuration control structure;
         as an offset from the base of the shared address space */
    os_het_smartdsp_log_t   (*smartdsp_debug)[];
    /**< Pointer to where SmartDSP logs system events; PA initializes
         an array with the number of entries as there is SC cores*/
    os_het_debug_print_t    *het_debug_print;

    uint32_t                num_ipc_regions;
    /**< Number of IPC regions - this parameter is only for multimode usages, and should be accessed
         ONLY if initialized.pa_initialized equals OS_HET_INITIALIZED_MULTIMODE   */
} os_het_control_t;
 
extern os_het_control_t  *g_os_het_control;
/**< Pointer to the base address of the heterogeneous OS control strcuture */
 

_OS_END_EXTERN_C


#endif // __PSC913X_HETEROGENEOUS_H


