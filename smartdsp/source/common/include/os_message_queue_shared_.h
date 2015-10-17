/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/06/01 10:23:28 $
 $Id: os_message_queue_shared_.h,v 1.2 2011/06/01 10:23:28 b17975 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_message_queue_shared_.h,v $
 $Revision: 1.2 $
******************************************************************************/

/******************************************************************************

 @File          os_message_queue_shared_.h

 @Description   OS Queues shared data header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MESSAGE_QUEUE_SHARED_H_
#define __OS_MESSAGE_QUEUE_SHARED_H_

#include "os_queue_.h"

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

os_shared os_spinlock_handle shared_queue_message_guard;
os_shared volatile os_queue_t** g_message_queue_list_shared
#ifdef _shared_data
= NULL
#endif
;


#endif // __OS_MESSAGE_QUEUE_SHARED_H_

