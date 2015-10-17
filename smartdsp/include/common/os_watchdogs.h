/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2013/06/10 11:39:02 $
 $Id: os_watchdogs.h,v 1.2 2013/06/10 11:39:02 b42006 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_watchdogs.h,v $
 $Revision: 1.2 $
******************************************************************************/
 

/**************************************************************************//**
 @File          os_watchdogs.h

 @Description   OS watchdog timer header file.

 @Cautions      None.
*//***************************************************************************/

#ifndef __OS_WATCHDOGS_H
#define __OS_WATCHDOGS_H

#if defined(B4860) || defined(B4420)
#include "sc39xx_watchdog.h" /**< includes core WDT for B4 family */
#else
#include "os_hw_watchdogs.h" /**< includes SOC WDT for other architectures */

#endif //B4860_FAMILY
#endif // __OS_WATCHDOGS_H
