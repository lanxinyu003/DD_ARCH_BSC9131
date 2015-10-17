/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/08 15:31:21 $
 $Id: hw_timers_.h,v 1.3 2012/01/08 15:31:21 b08551 Exp $
 $Source: /cvsdata/SmartDSP/drivers/timers/rev1/include/hw_timers_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_timers_.h

 @Description   Hardware Timers internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __HW_TIMERS_H_
#define __HW_TIMERS_H_

#include "smartdsp_os_device.h"
#include "memmap.h"

#ifndef NUM_OF_HW_TIMER_32b_MODULES
#define NUM_OF_HW_TIMER_32b_MODULES 0
#endif

#ifndef NUM_OF_HW_TIMER_16b_MODULES
#define NUM_OF_HW_TIMER_16b_MODULES 0
#endif

#ifndef ARCH_SUPPORTS_TIMER32B
#if defined(NUM_OF_HW_TIMER_32b_MODULES) && (NUM_OF_HW_TIMER_32b_MODULES > 0)
#define ARCH_SUPPORTS_TIMER32B
#endif
#endif

#ifndef ARCH_SUPPORTS_TIMER16B
#if defined(NUM_OF_HW_TIMER_16b_MODULES) && (NUM_OF_HW_TIMER_16b_MODULES > 0)
#define ARCH_SUPPORTS_TIMER16B
#endif
#endif

#if !defined(ARCH_SUPPORTS_TIMER16B) && !defined(ARCH_SUPPORTS_TIMER32B)
#error " There is no supported HW timers !"
#endif

#if defined(ARCH_SUPPORTS_TIMER16B) && (NUM_OF_HW_TIMER_16b_MODULES > 0)
#define __SOC_DEFAULT_TIMER   HWT_SOC_16BIT
#elif defined(ARCH_SUPPORTS_TIMER32B)
#define __SOC_DEFAULT_TIMER   HWT_SOC_32BIT
#endif


#endif // __HW_TIMERS_H_
