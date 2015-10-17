/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: sc3x00_multicore_sync.c,v 1.2 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_multicore_sync.c,v $
 $Revision: 1.2 $
******************************************************************************/

/******************************************************************************

 @File          sc3x00_multicore_sync.c

 @Description   sc3x00 multicore synchronization file.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_l1_defense.h"
#include "os_multicore_sync.h"

/*****************************************************************************/
void            osSpinLockDataStore(os_spinlock_handle *lock)
{
    VAR_UNUSED(lock);
}
