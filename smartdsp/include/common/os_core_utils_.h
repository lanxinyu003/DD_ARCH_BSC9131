/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/10/27 18:48:31 $
 $Id: os_core_utils_.h,v 1.8 2011/10/27 18:48:31 dovl Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_core_utils_.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_core_utils_.h

 @Description   OS include file for architecture-specific macros.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CORE_UTILS__H
#define __OS_CORE_UTILS__H


#if defined(SC39XX) || defined(SC3X00) || defined(SC3X50) || defined(SC1XX) || defined(STARCORE) //STARCORE for backward compatibility
#include "starcore_utils_.h"
#elif defined(E300) || defined(E500)
#include "ppc_utils_.h"
#else
#error "Architecture definition is missing: Please define SC1XX or SC3X00."
#endif // STARCORE


#endif // __OS_CORE_UTILS__H

