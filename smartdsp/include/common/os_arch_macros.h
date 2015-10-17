/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/02 09:24:02 $
 $Id: os_arch_macros.h,v 1.17 2012/01/02 09:24:02 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_arch_macros.h,v $
 $Revision: 1.17 $
******************************************************************************/

/******************************************************************************
 
 @File          os_arch_macros.h

 @Description   OS include file for architecture-specific macros.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_ARCH_MACROS_H
#define __OS_ARCH_MACROS_H


#if defined(SC1XX) || defined(STARCORE) //STARCORE for backward compatibility
#include "starcore_macros.h"
#include "sc1XX_macros.h"
#elif defined(SC3X00) || defined(SC3X50)
#include "sc3x00_compiler.h"
#include "starcore_macros.h"
#include "sc3X00_macros.h"
#elif defined(SC39XX)
#include "sc39xx_compiler.h"
#include "starcore_macros.h"
#include "sc39xx_macros.h"
#elif defined(E300)
#include "e300_macros.h"
#elif defined(E500)
#include "e500_macros.h"
#else
#error "Architecture definition is missing: Please define SC1XX, SC3X00 or SC3X50."
#endif // STARCORE


#endif // __OS_ARCH_MACROS_H


