/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: hw_timers32_shared_.h,v 1.5 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/include/hw_timers32_shared_.h,v $
 $Revision: 1.5 $
******************************************************************************/
 

/******************************************************************************
 
 @File          hw_timers32_shared_.h

 @Description   Hardware Timers Shared controls.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS32_SHARED_H_
#define __HW_TIMERS32_SHARED_H_

#include "smartdsp_os_device.h"
#include "hw_timers_.h"

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

os_shared os_spinlock_handle g_os_hw_timers32_guard
#ifdef _shared_data
= 0
#endif
;

/* Bit mask of free timers */
os_shared volatile uint32_t g_os_hw_timers32_free[NUM_OF_HW_TIMER_32b_MODULES];

/* l1 defense data */
os_shared hw_timers32_data_t g_os_hw_timers32_l1d_data;

#endif // __HW_TIMERS_SHARED_H_
