/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:49 $
 $Id: psc913x_heterogeneous_debug.h,v 1.5 2012/11/22 16:28:49 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/heterogeneous/psc913x_heterogeneous_debug.h,v $
 $Revision: 1.5 $
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

 @File          psc913x_heterogeneous_debug.h

 @Description   Debug shared header file between SC and PA cores.

*//***************************************************************************/

#ifndef __PSC913X_HETEROGENEOUS_DEBUG_H
#define __PSC913X_HETEROGENEOUS_DEBUG_H


#include "psc913x_heterogeneous_common.h"

_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Description   SmartDSP Event log
 
*//***************************************************************************/
typedef struct
{
    uintptr_t   base_address;
    /**< Pointer to the core's event log */
    uint32_t    size;
    /**< Size of the event log (in bytes) */
    uint32_t    *last_error;
    /**< Pointer to the last error in SmartDSP. A value of 1 (OS_SUCCESS) means no error */
} os_het_smartdsp_log_t;



_OS_END_EXTERN_C

#endif // __PSC913X_HETEROGENEOUS_DEBUG_H
