/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/07/15 16:46:50 $
 $Id: psc913x_ipc_.h,v 1.7 2012/07/15 16:46:50 b05127 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/ipc/psc913x_ipc_.h,v $
 $Revision: 1.7 $
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

 @File          psc913x_heterogeneous_ipc_.h

 @Description   IPC shared header file between SC and PA cores.

 @Cautions      Some functions MAY NOT be supported by all OS's; although they
                MUST provide at least one function for sending and one for
                receiving to/from the IPC channel.

*//***************************************************************************/

#ifndef __PSC913X_HETEROGENEOUS_IPC_H_
#define __PSC913X_HETEROGENEOUS_IPC_H_


_OS_BEGIN_EXTERN_C


#define IPC_DEFAULT_ALIGNMENT  4
#define IPC_VIRQ_GEN_VALUE(index) (((index) & 0x7) | (((index) & 0x18)<<5))
#define IPC_MESH_INT_NUM(src_core, msg_num) (OS_INT_FROM_CORE0_0 + (SOC_MAX_NUM_OF_MESSAGES * src_core) + msg_num)

#define REGISTERS_TYPE   psc9x3x_ccsr_t
#define DEVICE_REGISTERS g_dsp_ccsr_map

_OS_END_EXTERN_C


#endif // __PSC913X_HETEROGENEOUS_IPC_H_


