/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/08 15:31:21 $
 $Id: hw_timers_shared_.h,v 1.3 2012/01/08 15:31:21 b08551 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/include/hw_timers_shared_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_timers_shared_.h

 @Description   Hardware Timers Shared controls.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS_SHARED_H_
#define __HW_TIMERS_SHARED_H_

#include "smartdsp_os_device.h"
#include "memmap.h"
#include "hw_timers_.h"

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

os_shared os_spinlock_handle g_os_hw_timers_guard
#ifdef _shared_data
= 0
#endif
;

#ifdef ARCH_SUPPORTS_TIMER16B
/* Bit mask of free 16 bit timers */
os_shared volatile uint32_t g_os_hw_timers_16b_free
#ifdef _shared_data
= (1 << NUM_OF_HW_TIMERS_16b) - 1
#endif
;
#endif

#ifdef ARCH_SUPPORTS_TIMER32B
/* Bit mask of free 16 bit timers */
os_shared volatile uint32_t g_os_hw_timers_32b_free
#ifdef _shared_data
= (1 << NUM_OF_HW_TIMERS_32b) - 1
#endif
;
#endif



#endif // __HW_TIMERS_SHARED_H_
