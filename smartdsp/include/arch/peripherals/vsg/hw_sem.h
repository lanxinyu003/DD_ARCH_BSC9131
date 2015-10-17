/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:21 $
 $Id: hw_sem.h,v 1.3 2012/11/22 16:28:21 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/peripherals/vsg/hw_sem.h,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_sem.h

 @Description   Hardware semaphore API file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef HW_SEM_H_
#define HW_SEM_H_

#include "vsg_memmap.h"
#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         hw_sem_id  Hardware Semaphore API
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      hwSemaphoreTake

 @Description   Performs a busy wait trying to acquire a hardware semaphore.

 @Param[in]     sem_num - Number of the semaphore to take.
 @Param[in]     val     - Value to write to the semaphore (0x01 - 0xFF)
 
*//***************************************************************************/
void   hwSemaphoreTake(int sem_num, uint8_t val);


/**************************************************************************//**
 @Function      hwSemaphoreIrqTake

 @Description   Disables interrupts and then performs a busy wait trying to acquire
                a hardware semaphore.
 
 @Param[in]     sem_num - Number of the semaphore to take.
 @Param[in]     val     - Value to write to the semaphore (0x01 - 0xFF)
 
*//***************************************************************************/
void   hwSemaphoreIrqTake(int sem_num, uint8_t val);

/**************************************************************************//**
 @Function      hwSemaphoreRelease

 @Description   Releases a hardware semaphore.

 @Param[in]     sem_num - Number of the semaphore to release.
 
 @Cautions      This function doesn't validate the the software entity
                 releasing the semaphore is the same as that acquired it
 
*//***************************************************************************/
void   hwSemaphoreRelease(int sem_num);


/**************************************************************************//**
 @Function      hwSemaphoreIrqRelease

 @Description   Releases a hardware semaphore and then enables interrupts

 @Param[in]     sem_num - Number of the semaphore to release.
 
 @Cautions      This function doesn't validate the the software entity
                 releasing the semaphore is the same as that acquired it
 
*//***************************************************************************/
void   hwSemaphoreIrqRelease(int sem_num);


/**************************************************************************//**
 @Function      hwSemaphoreTryTake

 @Description   Try taking a hardware semaphore.

 @Param[in]     sem_num - Number of the semaphore to take.
 @Param[in]     val     - Value to write to the semaphore (0x01 - 0xFF)

 @Retval        OS_SUCCESS - Semaphore taken successfully
 @Retval        OS_FAIL    - Semaphore already taken by other entity
 
*//***************************************************************************/
os_status   hwSemaphoreTryTake(int sem_num, uint8_t val);


/**************************************************************************//**
 @Function      hwSemaphoreGetVal

 @Description   Read the value currently written in a hardware semaphore.

 @Param[in]     sem_num - Number of the semaphore to read.

 @Return        Value currently written
 
*//***************************************************************************/
uint8_t hwSemaphoreGetVal(int sem_num);


/**************************************************************************//**
 @Function      hwSemaphoreIsTaken

 @Description   Checks if hardware semaphore is taken.

 @Param[in]     sem_num - Number of the semaphore to check.

 @Retval        TRUE  - Semaphore is taken
 @Retval        FALSE - Semaphore is not taken
 
*//***************************************************************************/
#define hwSemaphoreIsTaken(sem_num) (hwSemaphoreGetVal(sem_num) != 0)


/**************************************************************************//**
 @Function      hwSemaphoreIsFree

 @Description   Checks if hardware semaphore is free.

 @Param[in]     sem_num - Number of the semaphore to check.

 @Retval        FALSE - Semaphore is not taken
 @Retval        TRUE  - Semaphore is taken
 
*//***************************************************************************/
#define hwSemaphoreIsFree(sem_num) (hwSemaphoreGetVal(sem_num) == 0)


/** @} */ // end of hw_sem_id



_OS_END_EXTERN_C

#endif // HW_SEM_H_
