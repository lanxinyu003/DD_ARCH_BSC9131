/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/02/20 13:05:37 $
 $Id: os_sync.c,v 1.21 2014/02/20 13:05:37 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_sync.c,v $
 $Revision: 1.21 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sync.c

 @Description   OS Synchronization module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_multicore_sync.h"
#include "os_sync_shared_.h"
#if (OS_MULTICORE == 1)
#include "os_l1_defense.h"
#endif //OS_MULTICORE

#ifdef SC39XX
#include "sc39xx_cache.h"
#endif

#if (OS_MULTICORE == 1)

/*****************************************************************************/
void osCentralBarrierWait(volatile os_barrier_t *barrier)
{
    register const uint8_t core_id = (const uint8_t)osGetCoreID();
    uint8_t  sensor;
    volatile uint8_t counter;
 
    sensor = barrier->sensors[core_id] = !barrier->sensors[core_id];
    osSpinLockSwiftIrqGet((os_spinlock_handle *)&barrier->guard);
    counter = ++(barrier->counter);
    osSpinLockSwiftIrqRelease((os_spinlock_handle *)&barrier->guard);
 
    if (counter == barrier->height)
    {
        barrier->counter = 0;
        barrier->comm_sensor = sensor;
#ifdef SC39XX
        osCacheStoreBarrier(NOT_CACHED);
#endif
    }
    else
    {
#ifdef SC39XX
        osCacheLoadBarrier(NOT_CACHED);
#endif
        while (barrier->comm_sensor != sensor)
        {
 
        }
    }
}

/*****************************************************************************/
static void osCentralBarrierRelease(volatile os_barrier_t *barrier)
{
    register const uint8_t core_id = (const uint8_t)osGetCoreID();
    uint8_t  sensor;
    volatile uint8_t counter;
 
    sensor = barrier->sensors[core_id] = !barrier->sensors[core_id];
    osSpinLockSwiftIrqGet((os_spinlock_handle *)&barrier->guard);
    counter = ++(barrier->counter);
    osSpinLockSwiftIrqRelease((os_spinlock_handle *)&barrier->guard);
 
    if (counter == barrier->height)
    {
        barrier->counter = 0;
        barrier->comm_sensor = sensor;
#ifdef SC39XX
        osCacheStoreBarrier(NOT_CACHED);
#endif
    }
}

/*****************************************************************************/
void osCentralBarrierVerify()
{
    if(shared_cores_barrier.sensors[osGetCoreID()] == shared_cores_barrier.comm_sensor)
    {
        if(shared_cores_barrier.counter > 0)
        {
            osCentralBarrierRelease(&shared_cores_barrier);
        }
    }
}

/*****************************************************************************/
void osWaitForAllCores()
{
    osCentralBarrierWait(&shared_cores_barrier);
}

#endif /* (OS_MULTICORE == 1) */
