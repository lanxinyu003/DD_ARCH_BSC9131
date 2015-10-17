/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/09/20 06:50:52 $
 $Id: os_init.h,v 1.18 2012/09/20 06:50:52 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/common/os_init.h,v $
 $Revision: 1.18 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_init.h

 @Description   Defines memory segment for initialization code.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_INIT_H
#define __OS_INIT_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C


/* Memory map definitions to place OS code in correct locations. */
#if defined(SC1XX) || defined(STARCORE)
#pragma pgm_seg_name  ".oskernel_text_initialization"
#pragma data_seg_name ".oskernel_local_data"
#pragma bss_seg_name  ".oskernel_local_data_bss"
#pragma rom_seg_name  ".oskernel_rom"
#elif defined(SC3X00) || defined(SC3X50) || defined(SC39XX)
#pragma pgm_seg_name  ".oskernel_text_initialization", "overlay"
#pragma data_seg_name ".oskernel_local_data"
#pragma bss_seg_name  ".oskernel_local_data_bss"
#pragma rom_seg_name  ".oskernel_rom"
#else
#error "Architecture definition is missing: Please define SC1XX, SC3X00 or SC3X50."
#endif


_OS_END_EXTERN_C

#endif // __OS_INIT_H


