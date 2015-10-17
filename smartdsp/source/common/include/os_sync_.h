/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/12 11:00:48 $
 $Id: os_sync_.h,v 1.1 2014/03/12 11:00:48 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_sync_.h,v $
 $Revision: 1.1 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sync_.h

 @Description   OS Synchronization module header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SYNC_H_
#define __OS_SYNC_H_

/**************************************************************************//**
 @Function      osSpinLockCoreReset

 @Description   Releases the spinlocks and spinlocks data
                according to mode 1 of warm reset for using
                in L1 defense feature.
 
*//***************************************************************************/
void osSpinLockCoreReset();

/**************************************************************************//**
 @Function      osSpinLockAllReset

 @Description   Releases the spinlocks and spinlocks data
                according to mode 2 of warm reset for using
                in L1 defense feature.
 
 @caution       It must be guaranteed that  this function is completed before any core
                tries to use any spinlock.
 
*//***************************************************************************/
void osSpinLockDataReset();


#endif /* __OS_SYNC_H_ */


