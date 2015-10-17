/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/06/05 19:46:24 $
 $Id: os_shared_data.c,v 1.11 2011/06/05 19:46:24 b17975 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_shared_data.c,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          os_shared_data.c

 @Description   OS shared data, for multicore systems.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"

#pragma data_seg_name ".os_shared_data"
#pragma bss_seg_name  ".os_shared_data_bss"

#define  _shared_data
#include "os_mem_shared_.h"

#if (OS_MULTICORE == 1)
#include "os_sync_shared_.h"
#include "os_queue_shared_.h"
#include "os_message_queue_shared_.h"
#endif // OS_MULTICORE

#undef   _shared_data



