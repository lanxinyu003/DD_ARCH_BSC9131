/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:50 $
 $Id: psc9x3x_clocks.h,v 1.7 2012/11/22 16:28:50 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/psc9x3x/psc9x3x_clocks.h,v $
 $Revision: 1.7 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_clocks.h

 @Cautions      None.
 
*//***************************************************************************/
#ifndef __PSC9X3X_CLOCKS_H
#define __PSC9X3X_CLOCKS_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         clock_9x3x_id    PSC9x3x Clock API

 @Description   API for reading the PSC9x3x Clock block
 
 @{
*//***************************************************************************/

#define OS_CLKIN_FACTOR       1000   /* (kHz) */
/**< All the clocks here are in kHZ */

extern uint32_t    g_core_clock;
/**< Floating point representation of the core clock */
extern uint32_t    g_clock_in;
/**< Floating point representation of the clkin signal */
extern uint32_t    g_periph_clock;
/**< Floating point representation of the peripherals clock */

/**************************************************************************//**
 @Function      osCoreClockGet

 @Description   Get the core frequency
 
 @Return        Frequency [MHz]
*//***************************************************************************/
#define osCoreClockGet()        (g_core_clock +0)
#define osGetCoreClock()        osCoreClockGet()
/**< see osCoreClockGet()*/

/**************************************************************************//**
 @Function      osClockClkinGet

 @Description   Get the clkin frequency
 
 @Return        Frequency [MHz]
*//***************************************************************************/
#define osClockClkinGet()       (g_clock_in +0)
#define osGetCLKinClock()       osClockClkinGet()
/**< see osClockClkinGet()*/

/**************************************************************************//**
 @Function      osSlowPeriphClockGet

 @Description   Get the slow system (peripherals) frequency
 
 @Return        Frequency [MHz]
*//***************************************************************************/
#define osSlowPeriphClockGet()  (g_periph_clock / 2)
#define osSlowSystemClockGet()  osSlowPeriphClockGet()
/**< see osSlowPeriphClockGet()*/
#define osGetSlowSystemClock()  osSlowPeriphClockGet()
/**<see osSlowPeriphClockGet()*/

/**************************************************************************//**
 @Function      psc9x3xInitializeClocks

 @Description   Calculate the clock frequencies in the system

                Called by the OS during osInitialize(). Can be called
                by the application if performed PLL re-locking
 
 @Param[in]     clk_in -  Frequency of the CLKIN [MHz].
 
 @Return        OS_SUCCESS if successful
*//***************************************************************************/
os_status psc9x3xInitializeClocks(float clk_in);

/** @} */ // end of clock_9x3x_id

_OS_END_EXTERN_C

#endif  // __PSC9X3X_CLOCKS_H
