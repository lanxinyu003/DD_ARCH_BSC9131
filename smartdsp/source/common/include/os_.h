/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_.h,v 1.8 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_.h

 @Description   OS general functions and definitions.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_H_
#define __OS_H_


//*****************************************************************************
// OS Flag Definitions - These indicate OS status
//*****************************************************************************

#define OS_FLG_HWI_ACTIVE            0x0001
#define OS_FLG_BGD_ACTIVE            0x0002
#define OS_FLG_SWI_ACTIVE            0x0004

#define OS_FLG_SWI_ENABLE            0x0100
#define OS_FLG_SWI_REQ               0x0200
#define OS_FLG_TSK_REQ               0x1000

extern volatile uint16_t g_os_flags;
extern os_status         g_os_last_error;


#define OS_HWI_ACTIVE (g_os_flags & (OS_FLG_HWI_ACTIVE | OS_FLG_SWI_ACTIVE))

#define OS_HWI_INACTIVE (!(OS_HWI_ACTIVE))

void    osHwiContextSwitch();

#ifdef OS_LOAD_CALCULATION
#define OS_MEASURE_LOAD_TICKS   100

void    osCalcLoad();
void    osCalcRefCounter();
#endif // OS_LOAD_CALCULATION


#endif // __OS_H_


