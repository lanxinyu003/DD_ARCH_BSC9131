/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/03/28 16:59:39 $
 $Id: os_tick.h,v 1.13 2011/03/28 16:59:39 b33105 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_tick.h,v $
 $Revision: 1.13 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_tick.h

 @Description   OS Tick module header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_TICK_H
#define __OS_TICK_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         tick_id OS Tick API

 @Description   OS Tick access functions.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         tick_runtime_id OS Tick runtime API

 @Description   OS Tick runtime API.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osTickCount

 @Description   Retrieves the OS Tick count for the user.
 
 @Param[out]    up_time -  Receives the OS Tick count.

*//***************************************************************************/
void    osTickCount(uint32_t *up_time);


/**************************************************************************//**
 @Function      osTickTime

 @Description   Calculates the number of cycles since the Tick operation started.
 
 @Return        64 bit time stamp

*//***************************************************************************/
uint64_t    osTickTime();

/** @} */ // tick_runtime_id
/** @} */ // tick_id

_OS_END_EXTERN_C

#endif // __OS_TICK_H
