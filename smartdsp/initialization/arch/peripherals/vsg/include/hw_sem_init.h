/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:23 $
 $Id: hw_sem_init.h,v 1.2 2012/11/22 16:28:23 sw Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/vsg/include/hw_sem_init.h,v $
 $Revision: 1.2 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_sem_init.h

 @Description   Hardware Semahore initialization file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef HW_SEM_INIT_H_
#define HW_SEM_INIT_H_

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         hw_sem_id  Hardware Semaphore API
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      hwSemaphoreInitialize

 @Description   Initializes the hardware semaphore module.
 
 @Retval        OS_SUCCESS          - The multicore synchronization module was
                                      initialized successfully.
 
*//***************************************************************************/
os_status   hwSemaphoreInitialize();

/** @} */ // end of class_map_id



_OS_END_EXTERN_C
#endif // HW_SEM_INIT_H_
