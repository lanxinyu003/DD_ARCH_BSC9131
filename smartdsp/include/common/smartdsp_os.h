/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/09/30 13:35:17 $
 $Id: smartdsp_os.h,v 1.44 2014/09/30 13:35:17 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/smartdsp_os.h,v $
 $Revision: 1.44 $
******************************************************************************/

/**************************************************************************//**

 @File          smartdsp_os.h

 @Description   SmartDSP OS main include file.

*//***************************************************************************/

#ifndef __SMARTDSP_OS_H
#define __SMARTDSP_OS_H
 
#include "os_cpp.h" /* keep this before other headers */

#include "os_datatypes.h"
#include "os_error.h"
#include "os_log.h"

#include "os_arch_macros.h" /* keep this before other API headers */

#include "os_accessors.h"
#include "os_general.h"
#include "os_swi.h"
#include "os_hwi.h"
#include "os_hw_timer.h"
#include "os_timer.h"
#include "os_tick.h"
#include "os_multicore_sync.h"
#include "os_mem.h"
#include "os_queue.h"
#include "os_frame.h"
#include "os_cio.h"
#include "os_bio.h"
#include "os_sio.h"
#include "os_cop.h"
#include "os_sysinfo.h"
#if !(defined(B4860) || defined(B4420) || defined(B3421))
#include "os_message.h"
#endif
#include "os_message_queue.h"
#include "os_watchdogs.h"
#include "os_task.h"
#include "os_event_semaphore.h"
#include "os_event_queue.h"
#include "os_debughooks.h"
#include "os_virq.h"

#endif // __SMARTDSP_OS_H






