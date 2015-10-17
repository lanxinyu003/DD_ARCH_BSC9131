/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_cache.h,v 1.7 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/include/arch/starcore/core_family/sc3X00/sc3x00_cache.h,v $
 $Revision: 1.7 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_cache.h

 @Cautions      None.
 
*//***************************************************************************/
#ifndef __SC3X00_CACHE_H
#define __SC3X00_CACHE_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

#include <prototype.h>

/**************************************************************************//**
 @Group         cache_sc3x00_id Data SC3x00 Cache API

 @Description   SC3x00 Cache operations defines, enumerations and API
 
 @{
*//***************************************************************************/

#define M2_CACHE_USE_OPCODE_SIZE       1024
/**< Size for which dflush on M2 should revert to cache sweep */
#define MEM_CACHE_USE_OPCODE_SIZE      2048
/**< Size for which dflush on M3/DDR should revert to cache sweep */

#define CACHE_OP_SIZE(start, end)      ((uint32_t)(end) - (uint32_t)(start) + 1)
/**< calculate range according to start and end addresses */

#define ARCH_L2_CACHE_LINE_SIZE         64      /**< SC3850 L2 cache line size */



/**************************************************************************//**
 @Function      osL2VirtCacheLegal

 @Description   Check the legality of virtual addresses for L2 cache operations

                Verify that no L2 operations are being operated on M2 memory.

 
 @Param[in]    virt_start -  Start Virtual address for the command.
 @Param[in]    virt_end -  End Virtual address for the command.
 
 @Return       TRUE for legal. False for illegal.
 
*//***************************************************************************/
bool osL2VirtCacheLegal(os_const_virt_ptr virt_start, os_const_virt_ptr virt_end);


/**************************************************************************//**
 @Function      osL2PhysCacheLegal

 @Description   Check the legality of physical addresses for L2 cache operations

                Verify that no L2 operations are being operated on M2 memory.

 
 @Param[in]     phys_start -  Start Physical address for the command.
 @Param[in]     phys_end -  End Physical address for the command.
 
 @Return        TRUE for legal. False for illegal.
 
*//***************************************************************************/
bool osL2PhysCacheLegal(os_const_phys_ptr phys_start, os_const_phys_ptr phys_end);



/**************************************************************************//**
 @Function      osCacheProgFetch

 @Description   Prefetches program in to the program cache
 
 @Param[in]     virt_addr -  Start virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheProgFetch(os_const_virt_ptr virt_addr)
{
    _pfetch((void *)virt_addr);
}
 
/**************************************************************************//**
 @Function      osCacheDataFetch

 @Description   Prefetches data in to the data cache
 
 @Param[in]     virt_addr -  Start virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheDataFetch(os_const_virt_ptr virt_addr)
{
    _dfetch((void *)virt_addr);
}


/**************************************************************************//**
 @Function      osCacheDataFetchForWrite

 @Description   Prefetches data with intention to write in to the data cache
 
 @Param[in]     virt_addr -  Start virtual address for the command.
 
*//***************************************************************************/
INLINE void osCacheDataFetchForWrite(os_const_virt_ptr virt_addr)
{
    agu_dfetchw((void *)virt_addr);
}



/**************************************************************************/
asm INLINE void perfom_asm_dflush(const void *virt_addr, uint32_t size)
{
asm_header
    .arg
    _virt_addr in $r0;
    _size in $d1;
    return in $d0;
    .reg $r0,$r1,$d1,$n0,$lc3,$sa3;
asm_body
 
    [
        lsrr.l   #<7,d1
        move.l #128,n0
    ]
    [
        sub #1,d1
        syncm
    ]
    [
        adda #64,r0,r1
        doensh3  d1
    ]
 
    LOOPSTART3
    [
        dflush (r0)+n0
        dflush (r1)+n0
    ]
    LOOPEND3
 
    dflush (r0)
    syncio dflush (r1)  ; last one with syncio
    syncm               ; CORESUB10 erratum
asm_end
}

/**************************************************************************/
asm INLINE void perfom_asm_dsync(const void *virt_addr, uint32_t size)
{
asm_header
    .arg
    _virt_addr in $r0;
    _size in $d1;
    return in $d0;
    .reg $r0,$r1,$d1,$n0,$lc3,$sa3;
asm_body
 
    [
        lsrr.l   #<7,d1
        move.l #128,n0
    ]
    [
        sub #1,d1
        syncm
    ]
    [
        adda #64,r0,r1
        doensh3  d1
    ]
 
    LOOPSTART3
    [
        dsync (r0)+n0
        dsync (r1)+n0
    ]
    LOOPEND3
 
    dsync (r0)
    syncio dsync (r1)   ; last one with syncio
    syncm               ; CORESUB10 erratum
asm_end
}


/**************************************************************************//**
 @Function      osCacheDataFlush

 @Description   Perform data blocking flush operation on data the L1 cache

                This function will either perform the operation using
                core opcodes, or by calling osCacheDataSweep(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
INLINE os_status osCacheDataFlush(os_const_virt_ptr virt_addr_start, os_const_virt_ptr virt_addr_end, uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > M2_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheDataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_FLUSH);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dflush(virt_addr_start, size);

    return OS_SUCCESS;
}
 
/**************************************************************************//**
 @Function      osCacheDataFlushAsync

 @Description   Perform data non-blocking flush operation on the L1 data cache

                This function will either perform the operation using
                core opcodes, or by calling osCacheDataSweepAsync(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
 @Cautions      If the sweep is performed using core opcodes; it'll be blocking
 
*//***************************************************************************/
INLINE os_status osCacheDataFlushAsync(os_const_virt_ptr virt_addr_start,
                                       os_const_virt_ptr virt_addr_end,
                                       uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > M2_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheDataSweepAsync(virt_addr_start, virt_addr_end, task_id, CACHE_FLUSH);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dflush(virt_addr_start, size);

    return OS_SUCCESS;
}
 
/**************************************************************************//**
 @Function      osCacheL1L2DataFlush

 @Description   Perform data blocking flush operation on the L1/L2 data caches

                This function will either perform the operation using
                core opcodes, or by calling osCacheL1L2DataSweep(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataFlush(os_const_virt_ptr virt_addr_start,
                                      os_const_virt_ptr virt_addr_end,
                                      uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > MEM_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheL1L2DataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_FLUSH);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dflush(virt_addr_start, size);
 
    return OS_SUCCESS;
}

/**************************************************************************//**
 @Function      osCacheL1L2DataFlushAsync

 @Description   Perform data non-blocking flush operation on the L1/L2 data caches

                This function will either perform the operation using
                core opcodes, or by calling osCacheL1L2DataSweepAsync(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
 @Cautions      If the sweep is performed using core opcodes; it'll be blocking
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataFlushAsync(os_const_virt_ptr virt_addr_start,
                                           os_const_virt_ptr virt_addr_end,
                                           uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > MEM_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheL1L2DataSweepAsync(virt_addr_start, virt_addr_end, task_id, CACHE_FLUSH);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dflush(virt_addr_start, size);
 
    return OS_SUCCESS;
}


/**************************************************************************//**
 @Function      osCacheDataSync

 @Description   Perform data blocking sync operation on data the L1 cache

                This function will either perform the operation using
                core opcodes, or by calling osCacheDataSweep(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
INLINE os_status osCacheDataSync(os_const_virt_ptr virt_addr_start,  os_const_virt_ptr virt_addr_end, uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > M2_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheDataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_SYNCHRONIZE);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dsync(virt_addr_start, size);

    return OS_SUCCESS;
}
 
/**************************************************************************//**
 @Function      osCacheDataSyncAsync

 @Description   Perform data non-blocking sync operation on the L1 data cache

                This function will either perform the operation using
                core opcodes, or by calling osCacheDataSweepAsync(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
 @Cautions      If the sweep is performed using core opcodes; it'll be blocking
 
*//***************************************************************************/
INLINE os_status osCacheDataSyncAsync(os_const_virt_ptr virt_addr_start,  os_const_virt_ptr virt_addr_end, uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > M2_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheDataSweepAsync(virt_addr_start, virt_addr_end, task_id, CACHE_SYNCHRONIZE);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dsync(virt_addr_start, size);

    return OS_SUCCESS;
}
 
/**************************************************************************//**
 @Function      osCacheL1L2DataSync

 @Description   Perform data blocking sync operation on the L1/L2 data caches

                This function will either perform the operation using
                core opcodes, or by calling osCacheL1L2DataSweep(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataSync(os_const_virt_ptr virt_addr_start,  os_const_virt_ptr virt_addr_end, uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > MEM_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheL1L2DataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_SYNCHRONIZE);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dsync(virt_addr_start, size);
 
    return OS_SUCCESS;
}

/**************************************************************************//**
 @Function      osCacheL1L2DataSyncAsync

 @Description   Perform data non-blocking sync operation on the L1/L2 data caches

                This function will either perform the operation using
                core opcodes, or by calling osCacheL1L2DataSweepAsync(); based
                on the size of the sweep
 
 @Param[in]     virt_addr_start - Start Virtual address for the command.
 @Param[in]     virt_addr_end   - End Virtual address for the command.
 @Param[in]     task_id         - task ID (context ID)
 
 @Retval        OS_SUCCESS
 
 @Cautions      If the sweep is performed using core opcodes; it'll be blocking
 
*//***************************************************************************/
INLINE os_status osCacheL1L2DataSyncAsync(os_const_virt_ptr virt_addr_start,  os_const_virt_ptr virt_addr_end, uint32_t task_id)
{
    uint32_t size = CACHE_OP_SIZE(virt_addr_start, virt_addr_end);
 
    if (size > MEM_CACHE_USE_OPCODE_SIZE)
    {
        return osCacheL1L2DataSweepAsync(virt_addr_start, virt_addr_end, task_id, CACHE_SYNCHRONIZE);
    }
 
    OS_ASSERT_COND(IS_ALIGNED(virt_addr_start, ARCH_CACHE_LINE_SIZE));
    perfom_asm_dsync(virt_addr_start, size);
 
    return OS_SUCCESS;
}

/** @} */ // end of cache_sc3x00_id

_OS_END_EXTERN_C

#endif // __SC3X00_CACHE_H

