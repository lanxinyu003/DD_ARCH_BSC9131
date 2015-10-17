/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_compiler.h,v 1.6 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3x00_compiler.h,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************
 
 @File          sc3x00_compiler.h

 @Description   This file includes macros that enable SmartDSP to run with
                multiple compilers
 
*//***************************************************************************/

#ifndef __SC3X00_COMPILER_H
#define __SC3X00_COMPILER_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#ifdef _ENTERPRISE_C_
#include <prototype.h>
#define EI()    ei()
#define DI()    di()
#define DEBUG() debug()
#define NOP()   asm(" nop")
#else // _ENTERPRISE_C_
#define EI()    asm(" ei")
#define DI()    asm(" di")
#define DEBUG() asm(" debug")
#define NOP()   asm(" nop")
#endif // _ENTERPRISE_C_

#define OS_SHARED_DATA              ".shared_data_ddr0_cacheable"
#define OS_SHARED_DATA_BSS          ".shared_data_ddr0_cacheable_bss"

_OS_END_EXTERN_C

#endif // __SC3X00_COMPILER_H

