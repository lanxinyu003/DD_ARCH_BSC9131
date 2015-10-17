/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/10 09:24:52 $
 $Id: maple_memmap.h,v 1.19 2012/01/10 09:24:52 b17975 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/maple/rev1/maple_memmap.h,v $
 $Revision: 1.19 $
******************************************************************************/

/******************************************************************************
 
 @File          maple_memmap.h

 @Description   MAPLE Memory map file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __MAPLE_MEMMAP_H
#define __MAPLE_MEMMAP_H

#include "smartdsp_os_device.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#ifdef MAPLE_B2
#include "maple_b2_memmap.h"
#elif defined(MAPLE_B2F) || defined(MAPLE_B2P)
#include "maple_b2f_b2p_memmap.h"
#else
#include "maple_b_memmap.h"
#endif

_OS_END_EXTERN_C

#endif // __MAPLE_MEMMAP_H
