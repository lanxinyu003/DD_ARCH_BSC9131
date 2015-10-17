/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_cache.h,v 1.40 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_cache.h,v $
 $Revision: 1.40 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_cache.h

 @Description   OS Cache common file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CACHE_H
#define __OS_CACHE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C




/**************************************************************************//**
 @Group         mem_cache_id Caches Control API

 @Description   Cache Control API for configuring and controlling all caches
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Anchor        cache_command
 
 @Collection    Cache commands
 
                A cache command may be any of the commands listed.
                In order to apply it to the entire cache, the command
                should be bitwise or'ed with #CACHE_GLOBAL_COMMAND
 @{
*//***************************************************************************/
#define CACHE_INVALIDATE 0x00000003
/**< Invalidate cache content */

#define CACHE_SYNCHRONIZE 0x00000005
/**< Write modified data to memory and save it in the cache */

#define CACHE_FLUSH 0x00000001
/**< Write modified data then invalidate */

#define CACHE_GLOBAL_COMMAND 0x00000020
/**< Apply command to all cache content - to be used with other commands */

/* @} */ /* end of cache_command */


/************************************************************//**
 @Collection    Cache Policy Options

 @{
*//*************************************************************/
#define NOT_CACHED      0x00000000
/**< Not cacheable in any level of cache */
#define L1_CACHED       0x00000001
/**< Cacheable in L1 cache only */
#define L2_CACHED       0x00000002
/**< Cacheable in L2 cache only */
#define L1_L2_CACHED    (L1_CACHED | L2_CACHED)
/**< Cacheable in L1 and L2 cache */

/* @} */ /* end of cache_opts */


/************************************************************//**
 @Collection    Cache Alignment options
 
                Some cache implementation will sweep an entire line,
                even if only a subset of the line needs to be swept.
                The cache functions will assert in such cases - to
                ensure that the user is aware of this hardware
                characteristic. These macros align an address range
                to the relevant cache line size

 @{
*//*************************************************************/
#define CACHE_OP_LOW_ADDR(VIRT_BASE, GRANULARITY)        \
    (((uint32_t)(VIRT_BASE)) & ~((GRANULARITY) - 1))
/**< Calculates the start address for cache operations
     based on the first address and the granularity
     of the cache sweep (e.g. cache line size) */

#define CACHE_OP_HIGH_ADDR(VIRT_BASE, SIZE, GRANULARITY) \
    (((uint32_t)(VIRT_BASE)+(SIZE)-1) | ((GRANULARITY) - 1))
/**< Calculates the last address for cache operations
     based on the first address, the size of the sweep
     and the granularity of the cache sweep (e.g. cache line size) */

/* @} */ /* end of cache_align */


/**************************************************************************//**
 @Collection    L2 Cache Software Prefetch Attributes
*//***************************************************************************/
#define L2_SWPF_INT_EN          0x00000008
/**< Generate an interrupt on prefetch */

/**************************************************************************//**
 @Description   L2 Cache prefetch configuration parameters
*//***************************************************************************/
typedef struct
{
    uint32_t    start_addr;     /**< Start address of prefetch */
    uint32_t    row_size;       /**< Prefetch row size */
    uint16_t    stride_size;    /**< Stride between rows */
    uint16_t    num_rows;       /**< Number of rows */
    uint32_t    int_en;         /**< Interrupt on this prefetch - #L2_SWPF_INT_EN or 0 */
    void        *arch;          /**< Architecture dependent params - Set to NULL */
}l2_cache_pf_t;

/* @} */ /* end of l2_prefetch */


/**************************************************************************//**
 @Function      osCacheDataSweep

 @Description   Perform sweep operation on data cache

                This function performs sweep command and waits till it finishes
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheDataSweep(os_const_virt_ptr virt_addr_start,
                           os_const_virt_ptr virt_addr_end,
                           uint32_t task_id,
                           uint32_t command);


/**************************************************************************//**
 @Function      osCacheDataSweepAsync

 @Description   Perform sweep operation on data cache in non blocking manner

                This function waits till previous command finishes and performs
                sweep command. It does not block till it completed
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheDataSweepAsync(os_const_virt_ptr virt_addr_start,
                                os_const_virt_ptr virt_addr_end,
                                uint32_t task_id,
                                uint32_t command);


/**************************************************************************//**
 @Function      osCacheDataSweepSafe

 @Description   Perform sweep operation on data cache in safe way
 
                This function implements the fail-safe mechanism of performing
                a data cache sweep. Should be used in NMI

 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheDataSweepSafe(os_const_virt_ptr virt_addr_start,
                               os_const_virt_ptr virt_addr_end,
                               uint32_t task_id,
                               uint32_t command);


/**************************************************************************//**
 @Function      osCacheProgSweep

 @Description   Perform sweep operation on program cache
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheProgSweep(os_const_virt_ptr virt_addr_start,
                           os_const_virt_ptr virt_addr_end,
                           uint32_t task_id,
                           uint32_t command);


/**************************************************************************//**
 @Function      osCacheProgSweepGlobal

 @Description   Global sweep command on the program cache
 
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheProgSweepGlobal(uint32_t command);


/**************************************************************************//**
 @Function      osCacheDataEnable

 @Description   Enable /disable data cache
 
 @Param[in]     enable -  Set to TRUE to enable or FALSE to disable
 
 @Retval        TRUE if previous state - enabled
 @Retval        FALSE if previous state - disabled
 
*//***************************************************************************/
bool osCacheDataEnable(bool enable);


/**************************************************************************//**
 @Function      osCacheProgEnable

 @Description   Enable /disable data cache
 
 @Param[in]     enable -  Set to TRUE to enable or FALSE to disable
 
 @Retval        TRUE if previous state - enabled
 @Retval        FALSE if previous state - disabled
 
*//***************************************************************************/
bool osCacheProgEnable(bool enable);


/**************************************************************************//**
 @Function      osCacheDataGlobalLock

 @Description   Enable/Disable global cache lock

 @Param[in]     enable -  Set to TRUE to lock or FALSE to unlock
 
 @Retval        TRUE if previous state - locked
 @Retval        FALSE if previous state - unlocked
 
*//***************************************************************************/
bool osCacheDataGlobalLock(bool enable);


/**************************************************************************//**
 @Function      osCacheProgGlobalLock

 @Description   Enable/Disable global cache lock

 @Param[in]     enable -  Set to TRUE to lock or FALSE to unlock
 
 @Retval        TRUE if previous state - locked
 @Retval        FALSE if previous state - unlocked
 
*//***************************************************************************/
bool osCacheProgGlobalLock(bool enable);


/**************************************************************************//**
 @Function      osCacheDataSweepGlobal

 @Description   Global sweep command on the data cache
 
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS

*//***************************************************************************/
os_status osCacheDataSweepGlobal(uint32_t command);


/**************************************************************************//**
 @Function      osCacheDataInProgressSweep

 @Description   Check if data cache sweep is in progress

 @Retval        TRUE if a sweep is in progress
 @Retval        FALSE if a sweep is not in progress
 
*//***************************************************************************/
bool osCacheDataInProgressSweep();


/**************************************************************************//**
 @Function      osCacheDataWaitSweep

 @Description   Waits for end of sweep on the data cache
 
*//***************************************************************************/
void osCacheDataWaitSweep();


/**************************************************************************//**
 @Function      osCacheL2ProgEnable

 @Description   Enable /disable L2 program cache
 
 @Param[in]     enable -  Set to TRUE to enable or FALSE to disable
 
 @Retval        TRUE if previous state - enabled
 @Retval        FALSE if previous state - disabled
 
*//***************************************************************************/
bool osCacheL2ProgEnable(bool enable);


/**************************************************************************//**
 @Function      osCacheL2ProgSweep

 @Description   Perform sweep operation on L2 program cache
 
 @Param[in]     phys_addr_start - Start Physical address for the command.
 @Param[in]     phys_addr_end   - End Physical address for the command.
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheL2ProgSweep(os_const_phys_ptr phys_addr_start,
                             os_const_phys_ptr phys_addr_end,
                             uint32_t command);


/**************************************************************************//**
 @Function      osCacheL2ProgSweepGlobal

 @Description   Perform a global sweep command on the L2 program cache
 
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheL2ProgSweepGlobal(uint32_t command);


/**************************************************************************//**
 @Function      osCacheL2UnifiedEnable

 @Description   Enable /disable L2 unified cache
 
 @Param[in]     enable -  Set to TRUE to enable or FALSE to disable
 
 @Retval        TRUE if previous state - enabled
 @Retval        FALSE if previous state - disabled
 
*//***************************************************************************/
bool osCacheL2UnifiedEnable(bool enable);


/**************************************************************************//**
 @Function      osCacheL2UnifiedGlobalLock

 @Description   Enable/Disable global L2 cache lock

 @Param[in]     enable -  Set to TRUE to lock or FALSE to unlock
 
 @Retval        TRUE if previous state - locked
 @Retval        FALSE if previous state - unlocked
 
*//***************************************************************************/
os_status osCacheL2UnifiedGlobalLock(bool enable);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSweep

 @Description   Perform sweep operation on L2 unified cache

                This function performs sweep command and waits till it finishes
 
 @Param[in]     phys_addr_start - Start Physical address for the command.
 @Param[in]     phys_addr_end   - End Physical address for the command.
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheL2UnifiedSweep(os_const_phys_ptr phys_addr_start,
                                os_const_phys_ptr phys_addr_end,
                                uint32_t command);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSweepGlobal

 @Description   Global sweep command on the L2 unified cache
 
 @Param[in]     command         - command (see @ref cache_command "Cache Commands")
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheL2UnifiedSweepGlobal(uint32_t command);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSegmentFind

 @Description   Find a free L2 unified cache segment
 
 @Param[out]    segment_num -  The L2 cache segment
 
 @Retval        OS_SUCCESS if a free segment was found
 @Retval        OS_ERR_L2_CACHE_UNAVAILABLE if there is no free segment
 
*//***************************************************************************/
os_status osCacheL2UnifiedSegmentFind(uint32_t *segment_num);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSegmentCreate

 @Description   Creates a L2 unified cache segment
 
 @Param[in]     segment_num     -  The L2 cache segment
 @Param[in]     phys_addr_start -  Start Physical address for the command.
 @Param[in]     phys_addr_end   -  End Physical address for the command.
 @Param[in]     way_mask        -  Mask of relevant ways in unified cache
 
 @Retval        OS_SUCCESS if a segment created
 @Retval        OS_ERR_L2_CACHE_INVALID if segment_num is illegal
 @Retval        OS_ERR_L2_CACHE_ALREADY_CREATED if segment_num is already created
 @Retval        OS_ERR_L2_CACHE_NOT_CACHEABLE address range is not cacheable
 
*//***************************************************************************/
os_status osCacheL2UnifiedSegmentCreate(uint32_t segment_num,
                                        os_const_phys_ptr phys_addr_start,
                                        os_const_phys_ptr phys_addr_end,
                                        uint8_t way_mask);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSegmentDelete

 @Description   Deletes a L2 unified cache segment

 @Param[in]     segment_num     -  The L2 cache segment
 
 @Retval        OS_SUCCESS if a segment deleted
 @Retval        OS_ERR_L2_CACHE_INVALID if segment_num is illegal
 @Retval        OS_ERR_L2_CACHE_NOT_CREATED if the segment wasn't created
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status osCacheL2UnifiedSegmentDelete(uint32_t segment_num);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSegmentEnable

 @Description   Enables a L2 unified cache segment
 
 @Param[in]     segment_num     -  The L2 cache segment
 
 @Retval        OS_SUCCESS if a segment deleted
 @Retval        OS_ERR_L2_CACHE_INVALID if segment_num is illegal
 @Retval        OS_ERR_L2_CACHE_NOT_CREATED if the segment wasn't created
 
*//***************************************************************************/
os_status osCacheL2UnifiedSegmentEnable(uint32_t segment_num);


/**************************************************************************//**
 @Function      osCacheL2UnifiedSegmentDisable

 @Description   Disables a L2 unified cache segment
 
 @Param[in]     segment_num     -  The L2 cache segment
 
 @Retval        OS_SUCCESS if a segment deleted
 @Retval        OS_ERR_L2_CACHE_INVALID if segment_num is illegal
 @Retval        OS_ERR_L2_CACHE_NOT_CREATED if the segment wasn't created
 
*//***************************************************************************/
os_status osCacheL2UnifiedSegmentDisable(uint32_t segment_num);


/**************************************************************************//**
 @Function      osCacheL2DataPrefetch

 @Description   Perform a data prefetch for L2 cache

 
 @Param[in]     ch      -  The L2 channel to prefetch on
 @Param[in]     params  -  The prefetch parameters
 
 @Retval        OS_SUCCESS if a prefetch succeeded
 @Retval        OS_ERR_L2_CACHE_UNAVAILABLE if L2 cache is not configured
                or the channel is not legal
 @Retval        OS_ERR_L2_CACHE_WRONG_ALIGNMENT the prefetch addresses are
                not aligned to the hardware requirements
 
*//***************************************************************************/
os_status osCacheL2DataPrefetch(uint32_t ch, l2_cache_pf_t *params);


/**************************************************************************//**
 @Function      osCacheL2DataPrefetchConfigure

 @Description   Configure a prefetch channel for data prefetch in the L2 cache
 
                This channel can later be triggered by calling
                osCacheL2PrefetchStart()

 @Param[in]     ch      -  The L2 channel to prefetch on
 @Param[in]     params  -  The prefetch parameters
 
 @Retval        OS_SUCCESS if a prefetch configuration succeeded
 @Retval        OS_ERR_L2_CACHE_UNAVAILABLE if L2 cache is not configured
                or the channel is not legal
 @Retval        OS_ERR_L2_CACHE_WRONG_ALIGNMENT the prefetch addresses are
                not aligned to the hardware requirements
 
*//***************************************************************************/
os_status osCacheL2DataPrefetchConfigure(uint32_t ch, l2_cache_pf_t *params);


/**************************************************************************//**
 @Function      osCacheL2ProgPrefetch

 @Description   Perform a program prefetch for L2 cache
 
 @Param[in]     ch      -  The L2 channel to prefetch on
 @Param[in]     params  -  The prefetch parameters
 
 @Retval        OS_SUCCESS if a prefetch succeeded
 @Retval        OS_ERR_L2_CACHE_UNAVAILABLE if L2 cache is not configured
                or the channel is not legal
 @Retval        OS_ERR_L2_CACHE_WRONG_ALIGNMENT the prefetch addresses are
                not aligned to the hardware requirements
 
*//***************************************************************************/
os_status osCacheL2ProgPrefetch(uint32_t ch, l2_cache_pf_t *params);


/**************************************************************************//**
 @Function      osCacheL2ProgPrefetchConfigure

 @Description   Configure a prefetch channel for program prefetch in the L2 cache.
 
                This channel can later be triggered by calling
                osCacheL2PrefetchStart()

 @Param[in]     ch      -  The L2 channel to prefetch on
 @Param[in]     params  -  The prefetch parameters
 
 @Retval        OS_SUCCESS if a prefetch configuration succeeded
 @Retval        OS_ERR_L2_CACHE_UNAVAILABLE if L2 cache is not configured
                or the channel is not legal
 @Retval        OS_ERR_L2_CACHE_WRONG_ALIGNMENT the prefetch addresses are
                not aligned to the hardware requirements
 
*//***************************************************************************/
os_status osCacheL2ProgPrefetchConfigure(uint32_t ch, l2_cache_pf_t *params);


/**************************************************************************//**
 @Function      osCacheL2PrefetchChannelBusy

 @Description   Test if L2 cache prefetch channel is busy

 @Param[in]     ch      -  The L2 channel to test
 
 @Retval        TRUE if the channel is busy
 @Retval        FALSE if the channel is not busy

*//***************************************************************************/
bool osCacheL2PrefetchChannelBusy(uint32_t ch);

#define osCacheL2DataPrefetchChannelBusy(ch) osCacheL2PrefetchChannelBusy(ch)
/**< Test if L2 cache prefetch data channel is busy */

#define osCacheL2ProgPrefetchChannelBusy(ch) osCacheL2PrefetchChannelBusy(ch)
/**< Test if L2 cache prefetch program channel is busy */

/**************************************************************************//**
 @Function      osCacheL2PrefetchChannelWait

 @Description   Perform a busy wait until L2 cache prefetch channel is free

 @Param[in]     ch      -  The L2 channel to wait for
 
*//***************************************************************************/
void osCacheL2PrefetchChannelWait(uint32_t ch);
#define osCacheL2DataPrefetchChannelWait(ch) osCacheL2PrefetchChannelWait(ch)
/**< Perform a busy wait until L2 cache prefetch data channel is free */
#define osCacheL2ProgPrefetchChannelWait(ch) osCacheL2PrefetchChannelWait(ch)
/**< Perform a busy wait until L2 cache prefetch program channel is free */

/**************************************************************************//**
 @Function      osCacheL2PrefetchStart

 @Description   Start a L2 cache prefetch channel
 
                Starts a channel previously configured by either
                osCacheL2DataPrefetchConfigure() or osCacheL2ProgPrefetchConfigure()
 
 @Param[in]     ch      -  The L2 channel to start
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
os_status osCacheL2PrefetchStart(uint32_t ch);


/**************************************************************************//**
 @Function      osCacheL1L2DataSplitSweep

 @Description   Perform sweep operation on L1 and L2 data cache

                This function performs sweep command and waits till it finishes.
                The sweep is split to sub-sweeps based on L2$ way size and
                sweep alignment
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)
 @Param[in]     command          - command
 
 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      This function is not supported for invalidate operations
 
*//***************************************************************************/
os_status osCacheL1L2DataSplitSweep(os_const_virt_ptr virt_addr_start,
                                    os_const_virt_ptr virt_addr_end,
                                    uint32_t task_id,
                                    uint32_t command);


/**************************************************************************//**
 @Function      osCacheL1L2DataSweep

 @Description   Perform sweep operation on L1 and L2 data cache

                This function performs sweep command and waits till it finishes
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)
 @Param[in]     command          - command
 
 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status osCacheL1L2DataSweep(os_const_virt_ptr virt_addr_start,
                               os_const_virt_ptr virt_addr_end,
                               uint32_t task_id,
                               uint32_t command);


/**************************************************************************//**
 @Function      osCacheL1L2DataSweepAsync

 @Description   Perform sweep operation on L1+L2 data cache in non blocking manner

                This function waits till previous command finishes and performs
                sweep command. It does not block till it completed
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)
 @Param[in]     command          - command
 
 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status osCacheL1L2DataSweepAsync(os_const_virt_ptr virt_addr_start,
                                    os_const_virt_ptr virt_addr_end,
                                    uint32_t task_id,
                                    uint32_t command);


/**************************************************************************//**
 @Function      osCacheL1L2DataSweepSafe

 @Description   Perform sweep operation on L1+L2 data cache in safe way
 
                This function implements the fail-safe mechanism of performing
                a data cache sweep. Should be used in NMI

 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)
 @Param[in]     command          - command
 
 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors
 
*//***************************************************************************/
os_status osCacheL1L2DataSweepSafe(os_const_virt_ptr virt_addr_start,
                                   os_const_virt_ptr virt_addr_end,
                                   uint32_t task_id,
                                   uint32_t command);


/**************************************************************************//**
 @Function      osCacheProgFetch

 @Description   Perform Program fetch.

                Fetches one line from memory to the program cache, from the
                specified address until the end of the line. The number of
                bytes  that are fetched depends on the length of the line in
                the specific cache implementation
 
 @Param[in]    virt_addr -  Start Virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheProgFetch(os_const_virt_ptr virt_addr);


/**************************************************************************//**
 @Function      osCacheDataFetch

 @Description   Perform Data fetch.

                Fetches one line from memory to the data cache, from the
                specified address until the end of the line. The number of bytes
                that are fetched depends on the length of the line in the
                specific cache implementation
 
 @Param[in]    virt_addr -  Start Virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheDataFetch(os_const_virt_ptr virt_addr);


/**************************************************************************//**
 @Function      osCacheDataFetchForWrite

 @Description   Perform Data fetch with intention to write.

                Fetches one line from memory to the data cache, from the
                specified address until the end of the line. The instruction
                is identical to osCacheDataFetch(), only it instructs the L2
                cache to ignore the fetched lines. This is useful in case of
                data that is intended to be written over by the core and
                shortly written back to memory.
 
 @Param[in]    virt_addr -  Start Virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheDataFetchForWrite(os_const_virt_ptr virt_addr);

 
/**************************************************************************//**
 @Function      osCacheDataFlush

 @Description   Perform dflush operations.

                Flushes the L1 cache to physical memory using dflush. If
                sweep size is larger than the benchmark cutoff size will call
                osCacheDataSweep() with #CACHE_FLUSH.
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      The dflush opcode may operate on more than one cache level, however
                the osCacheDataSweep() only operates on L1
 
*//***************************************************************************/
INLINE os_status osCacheDataFlush(os_const_virt_ptr virt_addr_start,
                                  os_const_virt_ptr virt_addr_end,
                                  uint32_t task_id);


/**************************************************************************//**
 @Function      osCacheDataFlushAsync

 @Description   Perform dflush operations without waiting for the sweep to end

                Flushes the L1 cache to physical memory using dflush. If
                sweep size is larger than the benchmark cutoff size will call
                osCacheDataSweepAsync() with #CACHE_FLUSH. It does not block till
                it completed
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      The dflush opcode may operate on more than one cache level,
                however the osCacheDataSweepAsync() only operates on L1
 @Cautions      If dflush is called, the function blocks. It'll only not block
                if osCacheDataSweepAsync() is called
 
*//***************************************************************************/
INLINE os_status osCacheDataFlushAsync(os_const_virt_ptr virt_addr_start,
                                       os_const_virt_ptr virt_addr_end,
                                       uint32_t task_id);


/**************************************************************************//**
 @Function      osCacheDataSync

 @Description   Perform dsync operations

                Synchronizes the L1 cache to physical memory using dsync. If
                sweep size is larger than the benchmark cutoff size will call
                osCacheDataSweep() with #CACHE_SYNCHRONIZE.
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      The dsync opcode may operate on more than one cache level,
                however the osCacheDataSweep() only operates on L1
 
*//***************************************************************************/
INLINE os_status osCacheDataSync(os_const_virt_ptr virt_addr_start,
                                 os_const_virt_ptr virt_addr_end,
                                 uint32_t task_id);


/**************************************************************************//**
 @Function      osCacheDataSyncAsync

 @Description   Perform dsync operations without waiting for the sweep to end

                Synchronizes the L1 cache to physical memory using dsync. If
                sweep size is larger than the benchmark cutoff size will call
                osCacheDataSweepAsync() with #CACHE_SYNCHRONIZE. It does not
                block till it completed
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      The dsync opcode may operate on more than one cache level,
                however the osCacheDataSweepAsync() only operates on L1
 @Cautions      If dsync is called, the function blocks. It'll only not block
                if osCacheDataSweepAsync() is called
 
*//***************************************************************************/
INLINE os_status osCacheDataSyncAsync(os_const_virt_ptr virt_addr_start,
                                      os_const_virt_ptr virt_addr_end,
                                      uint32_t task_id);


/**************************************************************************//**
 @Function      osCacheL1L2DataFlush

 @Description   Perform dflush operations

                Flushes the L1 and L2 caches to physical memory using dflush.
                If sweep size is larger than the benchmark cutoff size will
                call osCacheL1L2DataSweep() with #CACHE_FLUSH.
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      Calling this function on an area not cacheable in L2 will
                result in system degradation if osCacheL1L2DataSweep() is
                invoked.
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataFlush(os_const_virt_ptr virt_addr_start,
                                      os_const_virt_ptr virt_addr_end,
                                      uint32_t task_id);
 


/**************************************************************************//**
 @Function      osCacheL1L2DataFlushAsync

 @Description   Perform dflush operations without waiting for the sweep to end

                Flushes the L1 and L2 caches to physical memory using dflush.
                If sweep size is larger than the benchmark cutoff size will
                call osCacheL1L2DataSweepAsync() with #CACHE_FLUSH. It does
                not block till it completed
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      Calling this function on an area not cacheable in L2 will
                result in system degradation if osCacheL1L2DataSweepAsync()
                is invoked.
 @Cautions      If dflush is called, the function blocks. It'll only not block
                if osCacheDataSweepAsync() is called
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataFlushAsync(os_const_virt_ptr virt_addr_start,
                                           os_const_virt_ptr virt_addr_end,
                                           uint32_t task_id);
 


/**************************************************************************//**
 @Function      osCacheL1L2DataSync

 @Description   Perform dsync operations

                Synchronizes the L1 and L2 caches to physical memory using
                dsync. If sweep size is larger than benchmark cutoff size
                will call osCacheL1L2DataSweep() with #CACHE_SYNCHRONIZE.
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      Calling this function on an area not cacheable in L2 will result
                in system degradation if osCacheL1L2DataSweep() is invoked.
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataSync(os_const_virt_ptr virt_addr_start,
                                     os_const_virt_ptr virt_addr_end,
                                     uint32_t task_id);


/**************************************************************************//**
 @Function      osCacheL1L2DataSyncAsync

 @Description   Perform dsync operations without waiting for the sweep to end

                Synchronizes the L1 and L2 caches to physical memory using
                dsync. If sweep size is larger than benchmark cutoff size
                will call osCacheL1L2DataSweepAsync() with #CACHE_SYNCHRONIZE.
                It does not block till it completed
 
 @Param[in]     virt_addr_start  - Start Virtual address for the command.
 @Param[in]     virt_addr_end    - End Virtual address for the command.
 @Param[in]     task_id          - task ID (context ID)

 @Retval        OS_SUCCESS if sweep was successful
 @Return        Error status, encoded in os_error.h, for other errors

 @Cautions      Calling this function on an area not cacheable in L2 will result
                in system degradation if osCacheL1L2DataSweepAsync() is invoked.
 @Cautions      If dsync is called, the function blocks. It'll only not block
                if osCacheDataSweepAsync() is called

*//***************************************************************************/
INLINE os_status osCacheL1L2DataSyncAsync(os_const_virt_ptr virt_addr_start,
                                          os_const_virt_ptr virt_addr_end,
                                          uint32_t task_id);


/** @} */ // end of mem_cache_id

_OS_END_EXTERN_C

#endif // __OS_CACHE_H
