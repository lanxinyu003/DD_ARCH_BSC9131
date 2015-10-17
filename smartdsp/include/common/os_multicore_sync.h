/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/12 11:00:48 $
 $Id: os_multicore_sync.h,v 1.40 2014/03/12 11:00:48 b08551 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_multicore_sync.h,v $
 $Revision: 1.40 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_multicore_sync.h

 @Description   OS Multicore Synchronization header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_MULTICORE_SYNC_H
#define __OS_MULTICORE_SYNC_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

// Keep in sync with scxxxx_sync_asm.asm
#define OS_GUARD_DISABLE 0xFF00

#if (OS_MULTICORE == 1)

/**************************************************************************//**
 @Group         barriers_id Barriers API

 @Description   Multicore barrier points synchronization API.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Description   OS Synchronization Barrier Structure.
 
*//***************************************************************************/
typedef struct
{
    /* MUST be first */
    os_spinlock_handle  guard;          /**< Access synchronization element. */
    uint8_t             counter;        /**< Number of cores present. */
    uint8_t             height;         /**< Number of cores to wait for. */
    uint8_t             comm_sensor;    /**< Combined presence sensor. */
    uint8_t             *sensors;       /**< Senses each core's presence. */
    volatile uint8_t    init_flag;    /**< Initialization indicator (0/1). */
} os_barrier_t;
/** @} */ // barriers_id

/**************************************************************************//**
 @Group         spinlocks_id Spinlocks API

 @{
*//***************************************************************************/


/**************************************************************************//**
 @Group         spinlocks_init_id Spinlocks initialization functions

 @{
*//***************************************************************************/

typedef struct
{
    uint32_t spinlock_num;
    /**< current number of spinlocks are acquired in application */
    os_spinlock_handle **spinlock_handle;
    /**< array of spinlock handles */
}os_spinlock_data_t;

extern os_spinlock_data_t g_os_l1d_spinlock_data;
extern uint32_t g_os_max_num_of_spinlocks;

#if defined(PSC9131) || defined(PSC9132)
#define osSpinLockDataStore(X)
#define osSpinLockDataRelease(X)
#else
/**************************************************************************//**
 @Function      osSpinLockDataStore

 @Description   Stores all spinlock handles for using
                in L1 defense feature.
 
 @Param[in]     lock -  a pointer to spinlock handle.

*//***************************************************************************/
void osSpinLockDataStore(os_spinlock_handle *lock);


/**************************************************************************//**
 @Function      osSpinLockDataRelease

 @Description   Release the spinlock from the L1 defense spinlock lists.
 
 @Param[in]     lock -  a pointer to spinlock handle.

*//***************************************************************************/
void            osSpinLockDataRelease(os_spinlock_handle *lock);
#endif
/**************************************************************************//**
 @Function      osSpinLockInitialize
 
 @Description   Initialize the lock
 
 @Param[in]     lock -  a pointer to spinlock handle.
 
*//***************************************************************************/
INLINE void osSpinLockInitialize(os_spinlock_handle *lock)
{
    *lock = SPINLOCK_INIT;
    osSpinLockDataStore(lock);
}


/**************************************************************************//**
 @Function      osSpinLockDisable

 @Description   Disable the lock
 
 @Param[in]     lock -  a pointer to spinlock handle.
 
*//***************************************************************************/
INLINE void osSpinLockDisable(os_spinlock_handle *lock)
{
    uint16_t* lock_p = (uint16_t*)lock;
    *lock_p = OS_GUARD_DISABLE;
    osSpinLockDataRelease(lock);
}

/** @} */ // spinlocks_init_id



/**************************************************************************//**
 @Group         spinlocks_runtime_id Spinlocks runtime functions

 @{
*//***************************************************************************/

#ifdef VERBOSE

#define osSpinLockGet(address)
#define osSpinLockSwiftGet(address)
#define osSpinLockRelease(address)
#define osSpinLockIrqGet(address)           osHwiSwiftDisable()
#define osSpinLockSwitftIrqGet(address)     osHwiSwiftDisable()
#define osSpinLockIrqRelease(address)       osHwiSwiftEnable()

#else // NOT VERBOSE

/**************************************************************************//**
 @Function      osSpinLockGet

 @Description   Acquires the given spinlock (blocking operation).
                Ignored in single-core.
 
 @Param[in]     address - Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockSwiftGet

 @Description   Acquires the given spinlock (blocking operation).
                No checking for OS_GUARD_DISABLE.
                Ignored in single-core.
 
 @Param[in]    address -  Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockSwiftGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockTryGet

 @Description   Tries to acquire the given spinlock. Returns whether or not
                the spinlock is acquired (non-blocking operation).
                In single-core equals to OS_SUCCESS.
 
 @Param[in]    address -  Address upon which to apply atomic operations.

 @Retval        0   - Spinlock was not acquired.
 @Retval        1   - Spinlock was acquired successfully.
*//***************************************************************************/
uint32_t        osSpinLockTryGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockSwiftTryGet

 @Description   Tries to acquire the given spinlock. Returns whether or not
                the spinlock is acquired (non-blocking operation).
                No checking for OS_GUARD_DISABLE.
                In single-core equals to OS_SUCCESS.

 @Param[in]     address - Address upon which to apply atomic operations.

 @Retval        0   - Spinlock was not acquired.
 @Retval        1   - Spinlock was acquired successfully.
*//***************************************************************************/
uint32_t        osSpinLockSwiftTryGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockRelease

 @Description   Releases the given spinlock.
                Ignored in single-core.

 @Param[in]     address -  Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockRelease(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockIrqGet

 @Description   Disables interrupts and then acquires the given spinlock
                (blocking operation).
                In single-core equals to osHwiSwiftDisable().

 @Param[in]     address -  Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockIrqGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockSwiftIrqGet

 @Description   Disables interrupts and then acquires the given spinlock
                (blocking operation).
                No checking for OS_GUARD_DISABLE.
                In single-core equals to osHwiSwiftDisable().
 
 @Param[in]     address - Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockSwiftIrqGet(os_spinlock_handle *address);


/**************************************************************************//**
 @Function      osSpinLockIrqRelease

 @Description   Releases the given spinlock and then enables interrupts.
                In single-core equals to osHwiSwiftEnable().
 
 @Param[in]     address -  Address upon which to apply atomic operations.
*//***************************************************************************/
void            osSpinLockIrqRelease(os_spinlock_handle *address);

/** @} */ // spinlocks_runtime_id
/** @} */ // spinlocks_id

#endif // VERBOSE

/**************************************************************************//**
 @Group         barriers_id Barriers API

 @Description   Multicore barrier points synchronization API.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         barriers_init_id Barriers initialization functions

 @Description   Multicore barrier points synchronization initialization functions.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osCentralBarrierInit

 @Description   Initializes the given central barrier structure.
 
 @Param[in,out] barrier         -  Address of the central barrier to initialize.
 @Param[in]     num_of_cores    -  Number of cores that will use the barrier.

 @Retval        OS_SUCCESS          - The barrier was initialized successfully.
 @Retval        OS_ERR_NO_MEMORY    - Not enough memory.

 @Cautions      Initialization of a barrier allocates a small amount of memory
                that is never freed. It is therefore recommended that you do
                not call this function repeatedly; instead, call the function
                only in your application's in initialization code.
*//***************************************************************************/
os_status       osCentralBarrierInit(volatile os_barrier_t  *barrier, uint8_t num_of_cores);
/** @} */ // barriers_init_id


/**************************************************************************//**
 @Group         barriers_runtime_id Barriers runtime functions

 @Description   Multicore barrier points synchronization runtime functions.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osCentralBarrierWait

 @Description   Waits on the given central barrier.
 
                Waits for an amount of cores to reach the place in the code
                where this function is called.
 
                The amount of cores is set by the @a num_of_cores parameter
                of osCentralBarrierInit().
 
 @Param[in]     barrier -  Address of the central barrier to wait on.

 @Cautions      You must first initialize the barrier using
                osCentralBarrierInit().
*//***************************************************************************/
void            osCentralBarrierWait(volatile os_barrier_t *barrier);

/**************************************************************************//**
 @Function      osCentralBarrierVerify

 @Description   Releases the central barrier according
                To mode 1 of warm reset for using in
                L1 defense feature.
 
*//***************************************************************************/
void            osCentralBarrierVerify ();


/**************************************************************************//**
 @Function      osWaitForAllCores

 @Description   Waits for all cores to reach the place in the code where this
                function is called.
 
                This function uses an internal central barrier that is
                initialized when the synchronization module is initialized.
                There is no need to initialize an additional central barrier
                for this function.
 
                Ignored in single-core.
*//***************************************************************************/
void            osWaitForAllCores();

/** @} */ // barriers_runtime_id

/** @} */ // barriers_id

#else // Single core

#define osWaitForAllCores()
#define osSpinLockTryGet(address)           OS_SUCCESS
#define osSpinLockSwiftTryGet(address)      OS_SUCCESS
#define osSpinLockGet(address)
#define osSpinLockSwiftGet(address)
#define osSpinLockRelease(address)
#define osSpinLockIrqGet(address)           osHwiSwiftDisable()
#define osSpinLockSwiftIrqGet(address)      osHwiSwiftDisable()
#define osSpinLockIrqRelease(address)       osHwiSwiftEnable()
#define osSpinLockInitialize(lock)
#define osSpinLockDisable(lock)

#endif // OS_MULTICORE

/**************************************************************************//**
 @Group         spinlocks_id Spinlocks API

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         spinlocks_runtime_id Spinlocks runtime functions

 @{
*//***************************************************************************/

/**************************************************************************//**
 @Function      osSpinLockSwiftRelease

 @Description   Releases the given spinlock. No checking for OS_GUARD_DISABLE
 
 @Param[in]     address -  Address upon which to apply atomic operations.
*//***************************************************************************/
#define osSpinLockSwiftRelease(address) osSpinLockRelease(address)


/**************************************************************************//**
 @Function      osSpinLockSwiftIrqRelease

 @Description   Releases the given spinlock and then enables interrupts.
                No checking for OS_GUARD_DISABLE
 
 @Param[in]     address -  Address upon which to apply atomic operations.
*//***************************************************************************/
#define osSpinLockSwiftIrqRelease(address) osSpinLockIrqRelease(address)


/**************************************************************************//**
 @Function      osSharedResourceLock

 @Description   Locks a protected structure.
 
                This function can be used on structures that contain a spinlock
                as their first field. In a multicore system it would acquire
                the spinlock and disable interrupts. In a single core system it
                would just disable interrupts.
 
 @Param[in]     resource -  Pointer to the resource that should be locked.

 @Cautions      The locked structure must have a spinlock (uint32_t) as the first
                field in that structure.
*//***************************************************************************/
#define osSharedResourceLock(resource)      osSpinLockIrqGet((os_spinlock_handle *)(resource))


/**************************************************************************//**
 @Function      osSharedResourceUnlock

 @Description   Unlocks a protected structure.
 
                This function can be used on structures that contain a spinlock
                as their first field. In a multicore system it would release
                the spinlock and enable interrupts. In a single core system it
                would just enable interrupts.
 
 @Param[in]     resource -  Pointer to the resource that should be unlocked.

 @Cautions      The unlocked structure must have a spinlock (uint32_t) as the
                first field in that structure.
*//***************************************************************************/
#define osSharedResourceUnlock(resource)    osSpinLockIrqRelease((os_spinlock_handle *)(resource))


/** @} */ // spinlocks_runtime_id
/** @} */ // spinlocks_id

_OS_END_EXTERN_C

#endif // __OS_MULTICORE_SYNC_H


