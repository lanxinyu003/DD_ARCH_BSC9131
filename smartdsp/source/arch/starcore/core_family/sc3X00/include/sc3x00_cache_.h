/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_cache_.h,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/include/sc3x00_cache_.h,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************
 
 @File          sc3x00_cache_.h

 @Cautions      None.
 
*//***************************************************************************/
#ifndef __SC3X00_CACHE_H_
#define __SC3X00_CACHE_H_

#include "os_cache.h"
#include "sc3x00_cache.h"


INLINE os_status cacheL1L2DataFlushAsync(int policy, const os_virt_ptr virt_addr_start, const os_virt_ptr virt_addr_end, uint32_t task_id)
{
    switch (policy)
    {
        case L1_CACHED:
            return osCacheDataFlush(virt_addr_start, virt_addr_end, task_id);
        case L2_CACHED:
        case L1_L2_CACHED:
            return osCacheL1L2DataFlush(virt_addr_start, virt_addr_end, task_id);
    }
    return OS_SUCCESS;
}

INLINE os_status cacheL1L2DataSynchAsync(int policy, const os_virt_ptr virt_addr_start, const os_virt_ptr virt_addr_end, uint32_t task_id)
{
    switch (policy)
    {
        case L1_CACHED:
            return osCacheDataSync(virt_addr_start, virt_addr_end, task_id);
        case L2_CACHED:
        case L1_L2_CACHED:
            return osCacheL1L2DataSync(virt_addr_start, virt_addr_end, task_id);
    }
    return OS_SUCCESS;
}

INLINE os_status cacheL1L2DataInvalidateAsync(int policy, const os_virt_ptr virt_addr_start, const os_virt_ptr virt_addr_end, uint32_t task_id)
{
    switch (policy)
    {
        case L1_CACHED:
            return osCacheDataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_INVALIDATE);
        case L2_CACHED:
        case L1_L2_CACHED:
            return osCacheL1L2DataSweep(virt_addr_start, virt_addr_end, task_id, CACHE_INVALIDATE);
    }
    return OS_SUCCESS;
}

INLINE void cacheL1L2DataWaitSweep(int policy)
{
    VAR_UNUSED(policy);
    /* The functions called are all blocking */
    return;
}




#endif // __SC3X00_CACHE_H_

