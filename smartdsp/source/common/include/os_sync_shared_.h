/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2010/03/21 12:20:17 $
 $Id: os_sync_shared_.h,v 1.7 2010/03/21 12:20:17 b06368 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_sync_shared_.h,v $
 $Revision: 1.7 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sync_shared_.h

 @Description   OS Synchronization module shared data header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SYNC_SHARED_H_
#define __OS_SYNC_SHARED_H_

#if (OS_MULTICORE == 1)

#include "os_multicore_sync.h"

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif


os_shared volatile os_barrier_t shared_cores_barrier
#ifdef _shared_data
= {0};
#else
;
#endif


#endif /* (OS_MULTICORE == 1) */

#endif /* __OS_SYNC_SHARED_H_ */


