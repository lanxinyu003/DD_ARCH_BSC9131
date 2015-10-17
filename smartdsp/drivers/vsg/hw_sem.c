/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/07/09 15:26:17 $
 $Id: hw_sem.c,v 1.5 2012/07/09 15:26:17 b17975 Exp $
 $Source: /cvsdata/SmartDSP/drivers/vsg/hw_sem.c,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          hw_sem_.c

 @Description   Hardware semaphores runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_runtime.h"
#include "hw_sem.h"
#include "vsg_memmap.h"


hs_map_t*   soc_hw_sem_module;


/**************************************************************************/
INLINE void sem_write(int sem_num, uint8_t val)
{
    // accesses MUST be of 32 bit even though the semaphore is 8 bit;
    // this is a hardware requirement
    WRITE_UINT32(soc_hw_sem_module[sem_num].hsmpr, val);
}

/**************************************************************************/
INLINE uint8_t sem_read(int sem_num)
{
    // accesses MUST be of 32 bit even though the semaphore is 8 bit;
    // this is a hardware requirement
    return (uint8_t)GET_UINT32(soc_hw_sem_module[sem_num].hsmpr);
}


/**************************************************************************/
void   hwSemaphoreTake(int sem_num, uint8_t val)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
    OS_ASSERT_COND(val != 0);
 
    do
    {
        sem_write(sem_num, val);
    } while (sem_read(sem_num) != val);
}

/**************************************************************************/
void hwSemaphoreIrqTake(int sem_num, uint8_t val)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
    OS_ASSERT_COND(val != 0);
 
    osHwiSwiftDisable();
 
    do
    {
        sem_write(sem_num, val);
    } while (sem_read(sem_num) != val);
}


/**************************************************************************/
void   hwSemaphoreRelease(int sem_num)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
 
    sem_write(sem_num, 0);
}

/**************************************************************************/
void   hwSemaphoreIrqRelease(int sem_num)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
 
    sem_write(sem_num, 0);
 
    osHwiSwiftEnable();
}


/**************************************************************************/
os_status   hwSemaphoreTryTake(int sem_num, uint8_t val)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
 
    sem_write(sem_num, val);
 
    if (sem_read(sem_num) == val)
        return OS_SUCCESS;
 
    return OS_FAIL;
}


/**************************************************************************/
uint8_t hwSemaphoreGetVal(int sem_num)
{
    OS_ASSERT_COND(sem_num < NUMBER_OF_HW_SEMAPHORES);
 
    return sem_read(sem_num);
}
