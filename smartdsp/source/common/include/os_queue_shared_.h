/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/03/10 16:51:29 $
 $Id: os_queue_shared_.h,v 1.10 2013/03/10 16:51:29 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_queue_shared_.h,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************
 
 @File          os_queue_shared_.h

 @Description   OS Queues shared data header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_QUEUE_SHARED_H_
#define __OS_QUEUE_SHARED_H_

#include "os_queue_.h"

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

os_shared os_spinlock_handle _shared_queue_guard;
os_shared os_queue_t         *g_squeue_list
#ifdef _shared_data
= NULL
#endif
;
os_shared uint16_t           g_total_squeues;


#endif // __OS_QUEUE_SHARED_H_

