/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/19 18:00:34 $
 $Id: sc3x00_cache.c,v 1.11 2013/02/19 18:00:34 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_cache.c,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************

 @File          sc3x00_cache.c

 @Description   SC3X00 Cache implementation file.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "smartdsp_os_device.h"
#include "memmap.h"
#include "sc3x00_mmu.h"
#include "os_cache.h"
#include "os_mmu_.h"
#include "os_rm_.h"
#include "sc3x00_cache.h"

/* sweep operations are 256bytes aligned only for sc3X00 */
#define OS_ASSERT_SWEEP(addr_start,addr_end)                                    \
    {                                                                           \
        OS_ASSERT_COMPILER_COND(IS_ALIGNED(addr_start,ARCH_CACHE_LINE_SIZE));           \
        OS_ASSERT_COMPILER_COND(IS_ALIGNED((uint32_t)addr_end+1,ARCH_CACHE_LINE_SIZE));  \
    }


#define DC_CR1_IN_USE   0x00000001
#define DC_CR1_CMN      0x00000010  //data cache common operation
#define IC_CR1_IN_USE   0x00000001
#define L2C_CR1_IN_USE  0x00000001

#define DC_CR2_DCE  0x00000001  //Data cache enable
#define DC_CR2_DCGL 0x00000002  //Data cache global lock
#define IC_CR2_ICE  0x00000001  //Instruction cache enable
#define IC_CR2_ICGL 0x00000002  //Instruction cache global lock
#define L2_CR2_ICE  0x00000001  //L2 cache enable
#define L2_CR2_CGL  0x00000002  //L2 cache global lock

#define L2_ENABLE               0x00000001
#define M2_ENABLE               0x00000002
#define L2_M2_SIZE              0x80000 // size of combined L2/M2 (512KB)
#define L2_SEGMENT_MASK         0x000000FF
#define DEFAULT_OS_L2_MASK      0x000000FF // Mask to be used for default mapping

#define L2_SWPF_DATA            0x00000004
#define L2_SWPF_PROG            0x00000000
#define L2_SWPF_PRIV            0x00000002
#define L2_SWPF_INIT            0x00000001
#define L2_WAY_SIZE             0x10000

#define ADDR_IN_RANGE(addr, start, end) \
    (((uint32_t)(addr) >= (uint32_t)(start)) && ((uint32_t)(addr) <= (uint32_t)(end)))
#define M2_MEMORY_SIZE          0x01000000
#define M2_MEMORY_BASE(CORE_ID) (DSP_PRIVATE_M2_BASE + (CORE_ID * M2_MEMORY_SIZE))
#define M2_MEMORY_END(CORE_ID)  (M2_MEMORY_BASE(CORE_ID) + M2_MEMORY_SIZE - 1)

#define CACHE_POLL_NOP_CNT      21


static dsp_plat_dcache_map_t     *dcache   = (dsp_plat_dcache_map_t*)0xFFF00800;
static dsp_plat_icache_map_t     *icache   = (dsp_plat_icache_map_t*)0xFFF00C00;
static dsp_plat_l2cache_map_t    *l2cache  = (dsp_plat_l2cache_map_t*)0xFFF0F000;
uint32_t                         g_l2cache_size = 0;

/**< Array of structures containing the specific for L2 segment configuration */
static struct
{
    uint32_t phys_addr_start;
    uint32_t phys_addr_end;
    uint8_t  status;
} g_os_l2cache_segments[NUMBER_L2_CACHE_PARTITIONS];

/**< Utility structure for performing common (L1+L2) cache sweeps */
struct l1_l2_cache_s
{
    uint32_t l1_start;
    uint32_t l1_end;
    uint32_t l2_start;
    uint32_t l2_end;
    uint32_t task_id;
    uint32_t command;
};


os_status osL2CacheInitialize(uint32_t l2cache_size);

/******************************************************************/

os_status osCacheDataSweep( os_const_virt_ptr virt_addr_start, os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);

    osHwiSwiftDisable();

    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }

    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start | task_id ));
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, (((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | command));
    osHwiSwiftEnable();

    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    return OS_SUCCESS;
}

/******************************************************************/

os_status osCacheDataSweepSafe( os_const_virt_ptr virt_addr_start, os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    uint32_t dc_cr0;
    uint32_t dc_cr1;

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);

    osHwiSwiftDisable();

    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }

    dc_cr0 = GET_UINT32(dcache->dc_cr0);
    dc_cr1 = GET_UINT32(dcache->dc_cr1);

    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start | task_id ));
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, (((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | command));
    osHwiSwiftEnable();

    // wait to complete
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    // restore sweep state
    WRITE_UINT32(dcache->dc_cr0, dc_cr0);
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, dc_cr1);

    return OS_SUCCESS;
}

/******************************************************************/

os_status osCacheDataSweepAsync( os_const_virt_ptr virt_addr_start, os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);

    osHwiSwiftDisable();

    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }

    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start | task_id ));
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, (((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | command));
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheDataSweepGlobal(uint32_t command)
{

    osHwiSwiftDisable();
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, CACHE_GLOBAL_COMMAND | command);
 
    // wait to complete
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    osHwiSwiftEnable();
    return OS_SUCCESS;
}

/******************************************************************/

bool osCacheDataEnable(bool enable)
{
    uint16_t dc_cr2 = GET_UINT16(dcache->dc_cr2);
    bool ret = !!(dc_cr2 & DC_CR2_DCE);

    /* Illegal to disable cache once enabled */
    OS_ASSERT_COND(!(ret & ~enable));

    if(enable)
        dc_cr2 |= DC_CR2_DCE; //Enable bit
    else
        dc_cr2 &= ~DC_CR2_DCE;

    WRITE_SYNCIO_UINT16(dcache->dc_cr2, dc_cr2);

    return ret;
}

/******************************************************************/

bool osCacheProgEnable(bool enable)
{
    uint32_t ic_cr2;
    bool ret;

    // enable L1 i-cache
    ic_cr2 = GET_UINT32(icache->ic_cr2);
    ret = !!(ic_cr2 & IC_CR2_ICE);

    /* Illegal to disable cache once enabled */
    OS_ASSERT_COND(!(ret & ~enable));

    if(enable)
        ic_cr2 |= IC_CR2_ICE; //Enable bit
    else
        ic_cr2 &= ~IC_CR2_ICE;

    WRITE_SYNCIO_UINT16(icache->ic_cr2, ic_cr2);

    return ret;
}

/******************************************************************/
os_status osCacheProgSweep( os_const_virt_ptr virt_addr_start, os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);


    osHwiSwiftDisable();
    while (GET_UINT32(icache->ic_cr1) & IC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }
    WRITE_UINT32(icache->ic_cr0, ((uint32_t)virt_addr_start | task_id));
    WRITE_SYNCIO_UINT32(icache->ic_cr1, (((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | command));
    osHwiSwiftEnable();

    // wait to complete
    while (GET_UINT32(icache->ic_cr1) & IC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    return OS_SUCCESS;
}

/******************************************************************/

os_status osCacheProgSweepGlobal(uint32_t command)
{

    osHwiSwiftDisable();
    while (GET_UINT32(icache->ic_cr1) & IC_CR1_IN_USE)
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }
    WRITE_SYNCIO_UINT32(icache->ic_cr1, CACHE_GLOBAL_COMMAND | command);
    osHwiSwiftEnable();

    // wait to complete
    while (GET_UINT32(icache->ic_cr1) & IC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    return OS_SUCCESS;
}

/******************************************************************/
bool osCacheProgGlobalLock(bool enable)
{
    uint16_t ic_cr2 = GET_UINT16(icache->ic_cr2);
    bool ret = !!(ic_cr2 & IC_CR2_ICGL);

    if(enable)
        ic_cr2 |= IC_CR2_ICGL; //Enable bit
    else
        ic_cr2 &= ~IC_CR2_ICGL;

    WRITE_SYNCIO_UINT16(icache->ic_cr2, ic_cr2);

    return ret;
}


/******************************************************************/

bool osCacheDataGlobalLock(bool enable)
{
    uint16_t dc_cr2 = GET_UINT16(dcache->dc_cr2);
    bool ret = !!(dc_cr2 & DC_CR2_DCGL);

    if(enable)
        dc_cr2 |= DC_CR2_DCGL; //Enable bit
    else
        dc_cr2 &= ~DC_CR2_DCGL;

    WRITE_SYNCIO_UINT16(dcache->dc_cr2, dc_cr2);

    return ret;
}

/******************************************************************/

bool osCacheDataInProgressSweep()
{
    return (bool)(GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE);
}

/******************************************************************/

void osCacheDataWaitSweep()
{
    // wait to complete
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }
}


/******************************************************************/
/* L2 cache functions */
/******************************************************************/

os_status osCacheL2UnifiedGlobalLock(bool enable)
{

    uint16_t l2_cr2 = GET_UINT16(l2cache->l2_cr2);
    bool ret = !!(l2_cr2 & L2_CR2_CGL);

    if(enable)
        l2_cr2 |= L2_CR2_CGL; //Enable bit
    else
        l2_cr2 &= ~L2_CR2_CGL;

    WRITE_SYNCIO_UINT16(l2cache->l2_cr2, l2_cr2);

    return ret;
}
/******************************************************************/

os_status osCacheL2UnifiedSegmentFind(uint32_t *segment_num)
{
    uint32_t i;
    os_status status = OS_ERR_L2_CACHE_UNAVAILABLE;


    osHwiSwiftDisable();
    for (i=0 ; i<NUMBER_L2_CACHE_PARTITIONS ; i++)
    {
        if (g_os_l2cache_segments[i].status < OS_RESOURCE_ACQUIRED)
        {
            g_os_l2cache_segments[i].status = OS_RESOURCE_ACQUIRED;
            *segment_num = i;
            status = OS_SUCCESS;
            break;
        }
    }
    osHwiSwiftEnable();
    return status;
}

/******************************************************************/
os_status osCacheL2UnifiedSegmentCreate(uint32_t segment_num,
     os_const_phys_ptr phys_addr_start,
     os_const_phys_ptr phys_addr_end,
    uint8_t way_mask)
{


#ifdef L2_CACHE_ERROR_CHECKING
    if (segment_num >= NUMBER_L2_CACHE_PARTITIONS)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_INVALID;
    }

    if (g_os_l2cache_segments[segment_num].status >= OS_RESOURCE_USED)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_ALREADY_CREATED;
    }

    /* M2 not cacheable by L2 except for segment 0 */
    if ((segment_num != 0) && !osL2PhysCacheLegal(phys_addr_start, phys_addr_end))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_NOT_CACHEABLE;
    }
#endif /* L2_CACHE_ERROR_CHECKING */

    if (segment_num != 0)
        OS_ASSERT_SWEEP(phys_addr_start, phys_addr_end);


    osHwiSwiftDisable();
    g_os_l2cache_segments[segment_num].status = OS_RESOURCE_USED;

    g_os_l2cache_segments[segment_num].phys_addr_start = (uint32_t)phys_addr_start;
    g_os_l2cache_segments[segment_num].phys_addr_end = (uint32_t)phys_addr_end;

    WRITE_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_psea, ((uint32_t)phys_addr_end & 0xFFFFFF00) | way_mask);
    WRITE_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_pssa, (uint32_t)phys_addr_start |
        (GET_UINT32(l2cache->l2_ps[segment_num].l2_pssa) & M2_ENABLE));

    osHwiSwiftEnable();
    return OS_SUCCESS;
}


/******************************************************************/
os_status osCacheL2UnifiedSegmentDelete(uint32_t segment_num)
{
    os_status status;


#ifdef L2_CACHE_ERROR_CHECKING
    if (segment_num >= NUMBER_L2_CACHE_PARTITIONS)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_INVALID;
    }

    if (g_os_l2cache_segments[segment_num].status < OS_RESOURCE_ACQUIRED)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_NOT_CREATED;
    }
#endif /* L2_CACHE_ERROR_CHECKING */

    osHwiSwiftDisable();


    /* Must flush the cache before removing a segment */
    status = osCacheL2UnifiedSweep(( os_const_phys_ptr)g_os_l2cache_segments[segment_num].phys_addr_start,
        ( os_const_phys_ptr)(g_os_l2cache_segments[segment_num].phys_addr_end), CACHE_FLUSH);

    OS_ASSERT_COND(status == OS_SUCCESS);
    if (status != OS_SUCCESS)
    {
        osHwiSwiftEnable();
        return status;
    }


    WRITE_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_pssa, 0);
    WRITE_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_psea, 0);

    g_os_l2cache_segments[segment_num].status = OS_RESOURCE_DELETED;

    osHwiSwiftEnable();
    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheL2UnifiedSegmentEnable(uint32_t segment_num)
{

#ifdef L2_CACHE_ERROR_CHECKING
    if (segment_num >= NUMBER_L2_CACHE_PARTITIONS)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_INVALID;
    }

    if (g_os_l2cache_segments[segment_num].status < OS_RESOURCE_USED)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_NOT_CREATED;
    }
#endif   /* L2_CACHE_ERROR_CHECKING */

    osHwiSwiftDisable();
    SET_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_pssa, L2_ENABLE);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}


/******************************************************************/
os_status osCacheL2UnifiedSegmentDisable(uint32_t segment_num)
{

#ifdef L2_CACHE_ERROR_CHECKING
    if (segment_num >= NUMBER_L2_CACHE_PARTITIONS)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_INVALID;
    }

    if (g_os_l2cache_segments[segment_num].status < OS_RESOURCE_USED)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_NOT_CREATED;
    }
#endif   /* L2_CACHE_ERROR_CHECKING */

    osHwiSwiftDisable();
    CLEAR_SYNCIO_UINT32(l2cache->l2_ps[segment_num].l2_pssa, L2_ENABLE);
    osHwiSwiftEnable();

    return OS_SUCCESS;
}

/******************************************************************/
bool osCacheL2UnifiedEnable(bool enable)
{
    uint32_t l2_cr2;
    bool ret;

    // enable L2 cache
    l2_cr2 = GET_UINT32(l2cache->l2_cr2);
    ret = !!(l2_cr2 & L2_CR2_ICE);

    /* Illegal to disable cache once enabled */
    OS_ASSERT_COND(!(ret & ~enable));

    if(enable)
        l2_cr2 |= L2_CR2_ICE; //Enable bit
    else
        l2_cr2 &= L2_CR2_ICE;

    WRITE_SYNCIO_UINT16(l2cache->l2_cr2, l2_cr2);

    return ret;
}


/******************************************************************/
/* Utility function for easing the breaking up of osCacheL2UnifiedSweep to
 * L2_WAY_SIZE alignable sweeps */
INLINE void cacheL2UnifiedSweep(uint32_t start_addr, uint32_t end_addr, uint32_t command)
{
    /* initiate sweep operation */
    WRITE_UINT32(l2cache->l2_cr0, start_addr & MMU_MATT_ADDRESS_MASK);
    WRITE_SYNCIO_UINT32(l2cache->l2_cr1, (end_addr & MMU_MATT_ADDRESS_MASK)| command );

    /* wait to complete */
    while (GET_UINT32(l2cache->l2_cr1) & IC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }
}


os_status osCacheL2UnifiedSweep( os_const_phys_ptr phys_addr_start, os_const_phys_ptr phys_addr_end, uint32_t command)
{
    uint32_t start_addr = (uint32_t)phys_addr_start;
    uint32_t end_addr = (uint32_t)phys_addr_end;
    unsigned int remaining_size = (unsigned int)(end_addr - start_addr + 1);
    unsigned int i, num_sweeps;

    OS_ASSERT_SWEEP(phys_addr_start,phys_addr_end);

    /* Errata (CORESUBS1) - Do not perform synchronize sweep on L2 cache alone in 8154/6 rev1. Check M_PIR[PVN] */
    if ((command == CACHE_SYNCHRONIZE) && ((GET_UINT32(g_dsp_plat_map->mmu.m_pir) & 0x0000FFFF) == 0x00000003))
        OS_ASSERT;

    /* First iteration has to remove the head of the sweep if the base address isn't aligned */
    if (!IS_ALIGNED(start_addr, L2_WAY_SIZE))
    {
        end_addr = (uint32_t)(ALIGN_ADDRESS(start_addr, L2_WAY_SIZE) - 1);
        end_addr = (end_addr > (uint32_t)phys_addr_end) ? (uint32_t)phys_addr_end : end_addr;

        remaining_size -= (end_addr - start_addr + 1);

        cacheL2UnifiedSweep(start_addr, end_addr, command);
        start_addr = end_addr + 1;

    }

    /* At this point the start address is aligned. Perform L2_WAY_SIZE sweeps  */
    num_sweeps = (remaining_size >> 16);

    for (i=0 ; i<num_sweeps ; i++)
    {
        end_addr = start_addr + L2_WAY_SIZE - 1;
        cacheL2UnifiedSweep(start_addr, end_addr, command);
        start_addr += L2_WAY_SIZE;
    }

    /* Sweep the remainder */
    if (remaining_size & (L2_WAY_SIZE -1))
    {
        end_addr = (uint32_t)phys_addr_end;
        cacheL2UnifiedSweep(start_addr, end_addr, command);
    }

    return OS_SUCCESS;
}


/******************************************************************/
os_status osCacheL2UnifiedSweepGlobal(uint32_t command)
{

    /* Errata (CORESUBS1) - Do not perform synchronize sweep on L2 cache alone in 8154/6 rev1. Check M_PIR[PVN] */
    if ((command == CACHE_SYNCHRONIZE) && ((GET_UINT32(g_dsp_plat_map->mmu.m_pir) & 0x0000FF00) == 0))
        OS_ASSERT;

    /* initiate sweep operation */
    WRITE_SYNCIO_UINT32(l2cache->l2_cr1,  CACHE_GLOBAL_COMMAND | command );

    /* wait to complete */
    while (GET_UINT32(l2cache->l2_cr1) & IC_CR1_IN_USE) SYNCIO;

    return OS_SUCCESS;
}

/******************************************************************/
os_status osL2CacheInitialize(uint32_t l2cache_size)
{
    uint32_t mask=0, l2_psea = 0, l2_pssa = 0;
    os_status status = OS_SUCCESS;
    uint32_t l2_segment_num = NUMBER_L2_CACHE_PARTITIONS;
    uint32_t rule_set = L2_M2_SIZE - l2cache_size;

    g_l2cache_size = l2cache_size;

#ifdef L2_CACHE_ERROR_CHECKING
    /* L2 cache size has to bealigned to 64KB */
    if (l2cache_size & 0xFFFF)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_ALIGNMENT;
    }
    /* L2 cache size has to be less than 512KB */
    if (l2cache_size > L2_M2_SIZE)
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_SIZE;
    }
#endif /* L2_CACHE_ERROR_CHECKING */

    /* If have to set aside area for M2 - lower addresses are M2. */
    if (l2cache_size < L2_M2_SIZE)
    {
        rule_set >>= 16; // divide by 64 KB
        mask = (uint32_t)(L2_SEGMENT_MASK & ~(L2_SEGMENT_MASK << rule_set));
    }

    /* If there is space for M2, configure L2 cache partition 0 */
    if (l2cache_size != L2_M2_SIZE)
    {
        status = osCacheL2UnifiedSegmentFind(&l2_segment_num);
        OS_ASSERT_COND(status == OS_SUCCESS);
        OS_ASSERT_COND(l2_segment_num == 0);

        // Preset this to M2 configuration
        SET_SYNCIO_UINT32(l2cache->l2_ps[0].l2_pssa, M2_ENABLE);

        status = osCacheL2UnifiedSegmentCreate(l2_segment_num, NULL, NULL, (uint8_t)mask);
        OS_ASSERT_COND(status == OS_SUCCESS);

        status = osCacheL2UnifiedSegmentEnable(l2_segment_num);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }

    return status;
}



/******************************************************************/
static inline void cacheL2Prefetch(uint32_t ch, l2_cache_pf_t *params, uint32_t type)
{
	OS_ASSERT_COND(ch < NUMBER_L2_CACHE_PF_CHANNELS);
    WRITE_UINT32(l2cache->l2_pf[ch].l2_pf_st_addr, params->start_addr);
    WRITE_UINT32(l2cache->l2_pf[ch].l2_pf_rs, params->row_size);
    WRITE_UINT32(l2cache->l2_pf[ch].l2_pf_st, params->stride_size);
    WRITE_UINT32(l2cache->l2_pf[ch].l2_pf_rows_num, params->num_rows);
    WRITE_SYNCIO_UINT32(l2cache->l2_pf[ch].l2_pf_ctrl, (type | params->int_en | L2_SWPF_PRIV));
}

/******************************************************************/
os_status osCacheL2DataPrefetch(uint32_t ch, l2_cache_pf_t *params)
{
#ifdef L2_CACHE_ERROR_CHECKING
    if ((g_l2cache_size == 0) || (ch > NUMBER_L2_CACHE_PF_CHANNELS))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_UNAVAILABLE;
    }
    if (!(IS_ALIGNED(params->start_addr, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->row_size, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->stride_size, ALIGNED_8_BYTES)))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_ALIGNMENT;
    }
#endif

    osCacheL2DataPrefetchChannelWait(ch);
    cacheL2Prefetch(ch, params, L2_SWPF_DATA | L2_SWPF_INIT);

    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheL2DataPrefetchConfigure(uint32_t ch, l2_cache_pf_t *params)
{
#ifdef L2_CACHE_ERROR_CHECKING
    if ((g_l2cache_size == 0) || (ch > NUMBER_L2_CACHE_PF_CHANNELS))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_UNAVAILABLE;
    }
    if (!(IS_ALIGNED(params->start_addr, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->row_size, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->stride_size, ALIGNED_8_BYTES)))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_ALIGNMENT;
    }
#endif

    osCacheL2DataPrefetchChannelWait(ch);
    cacheL2Prefetch(ch, params, L2_SWPF_DATA);

    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheL2ProgPrefetch(uint32_t ch, l2_cache_pf_t *params)
{
#ifdef L2_CACHE_ERROR_CHECKING
    if ((g_l2cache_size == 0) || (ch > NUMBER_L2_CACHE_PF_CHANNELS))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_UNAVAILABLE;
    }
    if (!(IS_ALIGNED(params->start_addr, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->row_size, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->stride_size, ALIGNED_8_BYTES)))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_ALIGNMENT;
    }
#endif

    osCacheL2ProgPrefetchChannelWait(ch);
    cacheL2Prefetch(ch, params, L2_SWPF_PROG | L2_SWPF_INIT);

    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheL2ProgPrefetchConfigure(uint32_t ch, l2_cache_pf_t *params)
{
#ifdef L2_CACHE_ERROR_CHECKING
    if ((g_l2cache_size == 0) || (ch > NUMBER_L2_CACHE_PF_CHANNELS))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_UNAVAILABLE;
    }
    if (!(IS_ALIGNED(params->start_addr, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->row_size, ALIGNED_8_BYTES)) ||
        !(IS_ALIGNED(params->stride_size, ALIGNED_8_BYTES)))
    {
        OS_ASSERT;
        return OS_ERR_L2_CACHE_WRONG_ALIGNMENT;
    }
#endif

    osCacheL2ProgPrefetchChannelWait(ch);
    cacheL2Prefetch(ch, params, L2_SWPF_PROG);

    return OS_SUCCESS;
}

/******************************************************************/
os_status osCacheL2PrefetchStart(uint32_t ch)
{
    OS_ASSERT_COND(ch < NUMBER_L2_CACHE_PF_CHANNELS);

    osCacheL2DataPrefetchChannelWait(ch);
    SET_SYNCIO_UINT32(l2cache->l2_pf[ch].l2_pf_ctrl, L2_SWPF_INIT);
 
    return OS_SUCCESS;
}

/******************************************************************/
bool osCacheL2PrefetchChannelBusy(uint32_t ch)
{
    OS_ASSERT_COND(ch < NUMBER_L2_CACHE_PF_CHANNELS);
    return ((GET_UINT16(l2cache->l2_pf_status) & (1<<ch)) != 0);
}

/******************************************************************/
void osCacheL2PrefetchChannelWait(uint32_t ch)
{
    OS_ASSERT_COND(ch < NUMBER_L2_CACHE_PF_CHANNELS);
    osHwiSwiftDisable();
    while (GET_SYNCIO_UINT16(l2cache->l2_pf_status) & (1<<ch))
    {
        osHwiSwiftEnable();
        osHwiSwiftDisable();
    }
    osHwiSwiftEnable();
}


/******************************************************************/
// Triggers the sweep of both caches. Data or program rides on the address of l1_cr1
asm INLINE void cacheL1L2SweepTrigger(volatile void* l1_cr1, volatile void* l2_cr1)
{
asm_header
    .arg
    _l1_cr1 in $r0;
    _l2_cr1 in $r1;
    .reg $r0, $r1;
asm_body

    /* Trigger L1 cache */
    bmtset.w    #1,(r0)
    bt          <*-$4


    /* Trigger L2 cache */
    bmtset.w    #1,(r1)
    bt          <*-$4
asm_end
}

/******************************************************************/
/* Utility function for easing the breaking up of osCacheL1L2DataSweep to
 * L2_WAY_SIZE alignable sweeps */
INLINE void cacheL1L2DataSweep(struct l1_l2_cache_s * l1_l2_cache)
{
    /* L1 sweep - common bit set */
    WRITE_UINT32(dcache->dc_cr0, l1_l2_cache->l1_start | l1_l2_cache->task_id);
    WRITE_SYNCIO_UINT32(dcache->dc_cr1,
                        (l1_l2_cache->l1_end & MMU_MATT_ADDRESS_MASK) | (l1_l2_cache->command  /*| DC_CR1_IN_USE*/ | DC_CR1_CMN));

    /* L2 sweep */
    WRITE_UINT32(l2cache->l2_cr0, l1_l2_cache->l2_start);
    WRITE_SYNCIO_UINT32(l2cache->l2_cr1,
                        (l1_l2_cache->l2_end & MMU_MATT_ADDRESS_MASK) | (l1_l2_cache->command  /*| L2C_CR1_IN_USE*/));
    osHwiSwiftEnable();

    // Wait to complete. Signaled by L1 cache
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }
    osHwiSwiftDisable();

}

/******************************************************************/
os_status osCacheL1L2DataSplitSweep(os_const_virt_ptr virt_addr_start,os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    os_status   status;
    void        *phys_start;
    uint32_t    phys_end;
    struct l1_l2_cache_s l1_l2_cache;
    unsigned int remaining_size;
    unsigned int i, num_sweeps;

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & 0x7 ) != CACHE_INVALIDATE);

    status = osMmuDataVirtToPhys(virt_addr_start, &phys_start);
    OS_ASSERT_COND(status == OS_SUCCESS);

    phys_end = (uint32_t)phys_start + ((uint32_t)virt_addr_end - (uint32_t)virt_addr_start);

    l1_l2_cache.l1_start = (uint32_t)virt_addr_start;
/*    l1_l2_cache.l1_end   = (uint32_t)virt_addr_end; // marked out, since runtime calculation will take place */
    l1_l2_cache.l2_start = (uint32_t)phys_start;
/*    l1_l2_cache.l2_end   = phys_end;                // marked out, since runtime calculation will take place */
    l1_l2_cache.command  = command;
    l1_l2_cache.task_id  = task_id;

    remaining_size = (unsigned int)(phys_end - l1_l2_cache.l2_start + 1);

    osHwiSwiftDisable();

    while ((GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE) || (GET_UINT32(l2cache->l2_cr1) & L2C_CR1_IN_USE))
    {
        osHwiSwiftEnable();
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
        osHwiSwiftDisable();
    }

    /* First iteration has to remove the head of the sweep if the base address isn't aligned */
    if (!IS_ALIGNED(l1_l2_cache.l2_start, L2_WAY_SIZE))
    {
        unsigned int sweep_size;
        l1_l2_cache.l2_end = (uint32_t)(ALIGN_ADDRESS(l1_l2_cache.l2_start, L2_WAY_SIZE) - 1);
        l1_l2_cache.l2_end = (l1_l2_cache.l2_end > phys_end) ? phys_end : l1_l2_cache.l2_end;

        sweep_size = (l1_l2_cache.l2_end - l1_l2_cache.l2_start + 1);
        l1_l2_cache.l1_end = l1_l2_cache.l1_start + sweep_size - 1;

        remaining_size -= sweep_size;

        cacheL1L2DataSweep(&l1_l2_cache);
        l1_l2_cache.l1_start = l1_l2_cache.l1_end + 1;
        l1_l2_cache.l2_start = l1_l2_cache.l2_end + 1;

    }

    /* At this point the start address is aligned. Perform L2_WAY_SIZE sweeps  */
    num_sweeps = (remaining_size >> 16);

    for (i=0 ; i<num_sweeps ; i++)
    {
        l1_l2_cache.l1_end = l1_l2_cache.l1_start + L2_WAY_SIZE - 1;
        l1_l2_cache.l2_end = l1_l2_cache.l2_start + L2_WAY_SIZE - 1;

        cacheL1L2DataSweep(&l1_l2_cache);

        l1_l2_cache.l1_start += L2_WAY_SIZE;
        l1_l2_cache.l2_start += L2_WAY_SIZE;
    }

    /* Sweep the remainder */
    if (remaining_size & (L2_WAY_SIZE -1))
    {
        l1_l2_cache.l1_end = (uint32_t)virt_addr_end;
        l1_l2_cache.l2_end = phys_end;
        cacheL1L2DataSweep(&l1_l2_cache);
    }

    osHwiSwiftEnable();
    return OS_SUCCESS;

}

/******************************************************************/
os_status osCacheL1L2DataSweep(os_const_virt_ptr virt_addr_start,os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    os_status   status;
    void        *phys_start;
    uint32_t    phys_end;
 
    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);

    status = osMmuDataVirtToPhys(virt_addr_start, &phys_start);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    phys_end = (uint32_t)phys_start + ((uint32_t)virt_addr_end - (uint32_t)virt_addr_start);
 
    osHwiSwiftDisable();

    while ((GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE) || (GET_UINT32(l2cache->l2_cr1) & L2C_CR1_IN_USE))
    {
        osHwiSwiftEnable();
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
        osHwiSwiftDisable();
    }
 

    /* L1 sweep - common bit set */
    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start) | task_id);
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, ((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | (command  | DC_CR1_IN_USE | DC_CR1_CMN));

    /* L2 sweep */
    WRITE_UINT32(l2cache->l2_cr0, (uint32_t)phys_start);
    WRITE_SYNCIO_UINT32(l2cache->l2_cr1, ((uint32_t)phys_end & MMU_MATT_ADDRESS_MASK) | (command  | L2C_CR1_IN_USE));
    osHwiSwiftEnable();
 
 
    // Wait to complete. Signaled by L1 cache
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }
 

    return OS_SUCCESS;
}


/******************************************************************/
os_status osCacheL1L2DataSweepAsync(os_const_virt_ptr virt_addr_start,os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    os_status   status;
    void        *phys_start;
    uint32_t    phys_end;

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);
    OS_ASSERT_COND((task_id & MMU_MATT_ADDRESS_MASK) == 0);
    OS_ASSERT_COND((command & MMU_MATT_ADDRESS_MASK) == 0);

    status = osMmuDataVirtToPhys(virt_addr_start, &phys_start);
    OS_ASSERT_COND(status == OS_SUCCESS);

    phys_end = (uint32_t)phys_start + ((uint32_t)virt_addr_end - (uint32_t)virt_addr_start);

    osHwiSwiftDisable();

    while ((GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE) || (GET_UINT32(l2cache->l2_cr1) & L2C_CR1_IN_USE))
    {
        osHwiSwiftEnable();
        SYNCIO;
        osHwiSwiftDisable();
    }


    /* L1 sweep - common bit set */
    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start) | task_id);
    WRITE_SYNCIO_UINT32(dcache->dc_cr1, ((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | (command  | DC_CR1_IN_USE | DC_CR1_CMN));

    /* L2 sweep */
    WRITE_UINT32(l2cache->l2_cr0, (uint32_t)phys_start);
    WRITE_SYNCIO_UINT32(l2cache->l2_cr1, ((uint32_t)phys_end & MMU_MATT_ADDRESS_MASK) | (command  | L2C_CR1_IN_USE));
    osHwiSwiftEnable();


    return OS_SUCCESS;

}




/******************************************************************/
os_status osCacheL1L2DataSweepSafe(os_const_virt_ptr virt_addr_start,os_const_virt_ptr virt_addr_end, uint32_t task_id, uint32_t command)
{
    os_status   status;
    void        *phys_start;
    uint32_t    dc_cr0, dc_cr1, l2_cr0, l2_cr1, phys_end;

    OS_ASSERT_SWEEP(virt_addr_start,virt_addr_end);

    status = osMmuDataVirtToPhys(virt_addr_start, &phys_start);
    OS_ASSERT_COND(status == OS_SUCCESS);

    phys_end = (uint32_t)phys_start + ((uint32_t)virt_addr_end - (uint32_t)virt_addr_start);

    osHwiSwiftDisable();

    /* Save old values in registers */
    READ_UINT32(l2_cr0, l2cache->l2_cr0);
    READ_UINT32(l2_cr1, l2cache->l2_cr1);
    READ_UINT32(dc_cr0, dcache->dc_cr0);
    READ_UINT32(dc_cr1, dcache->dc_cr1);

    /* Prepare L2 cache */
    WRITE_UINT32(l2cache->l2_cr0, ((uint32_t)phys_start & MMU_MATT_ADDRESS_MASK));
    WRITE_UINT32(l2cache->l2_cr1, ((uint32_t)phys_end & MMU_MATT_ADDRESS_MASK) | (command & ~L2C_CR1_IN_USE));

    /* Prepare the L1 sweep */
    WRITE_UINT32(dcache->dc_cr0, ((uint32_t)virt_addr_start & MMU_MATT_ADDRESS_MASK) | (task_id & ~MMU_MATT_ADDRESS_MASK));
    WRITE_UINT32(dcache->dc_cr1, ((uint32_t)virt_addr_end & MMU_MATT_ADDRESS_MASK) | (command & ~DC_CR1_IN_USE) | DC_CR1_CMN);

    /* Start both cache sweeps in succession */
    cacheL1L2SweepTrigger(&dcache->dc_cr1, &l2cache->l2_cr1);

    osHwiSwiftEnable();


    // Wait to complete. Signalled by L1 cache
    while (GET_UINT32(dcache->dc_cr1) & DC_CR1_IN_USE)
    {
        OS_WAIT(CACHE_POLL_NOP_CNT);
        SYNCIO;
    }

    /* Restore old configuration */
    WRITE_UINT32(l2cache->l2_cr0, l2_cr0);
    WRITE_UINT32(l2cache->l2_cr1, l2_cr1);
    WRITE_UINT32(dcache->dc_cr0, dc_cr0);
    WRITE_UINT32(dcache->dc_cr1, dc_cr1);

    return OS_SUCCESS;

}


/******************************************************************/
bool osL2PhysCacheLegal(os_const_phys_ptr phys_start,  os_const_phys_ptr phys_end)
{
    if (ADDR_IN_RANGE(phys_start, M2_MEMORY_BASE(osGetCoreID()), M2_MEMORY_END(osGetCoreID())) ||
        ADDR_IN_RANGE(phys_end, M2_MEMORY_BASE(osGetCoreID()), M2_MEMORY_END(osGetCoreID())))
    {
        OS_ASSERT;
        return FALSE;
    }
    return TRUE;
}

/******************************************************************/
bool osL2VirtCacheLegal( os_const_virt_ptr virt_start,  os_const_virt_ptr virt_end)
{
    os_status   status;
    void        *phys_start, *phys_end;

    status = osMmuDataVirtToPhys(virt_start, &phys_start);
    OS_ASSERT_COND(status == OS_SUCCESS);
    status = osMmuDataVirtToPhys(virt_end, &phys_end);
    OS_ASSERT_COND(status == OS_SUCCESS);

    return osL2PhysCacheLegal(phys_start, phys_end);

}
