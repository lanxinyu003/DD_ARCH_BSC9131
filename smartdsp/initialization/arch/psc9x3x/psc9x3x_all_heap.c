/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/07/15 16:46:56 $
 $Id: psc9x3x_all_heap.c,v 1.4 2012/07/15 16:46:56 b05127 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/psc9x3x_all_heap.c,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_all_heap.c

 @Description   All heaps.

 @Cautions      The shared memory variable name must remain as defined.

*//***************************************************************************/

#include "smartdsp_os.h"
#include "os_config.h"
#include "smartdsp_init.h"


#define HEAP_ATTR(SEC, ALIGN)   __attribute__ ((aligned (ALIGN))) __attribute__ ((section (SEC)))
/**< Sets the section and alignment attributes */
#define HEAP_MIN_ALIGN          8
/**< The minimal heap alignment requirement */


/********************************************************************************/
/********************************* Local heaps **********************************/
/********************************************************************************/

/********************************* Local M3 (non-cacheable) *********************/
#ifdef OS_HEAP_M3_NOCACHEABLE_SIZE
#if (OS_HEAP_M3_NOCACHEABLE_SIZE > 0)
uint8_t    g_heap_m3_nocacheable[ALIGN_SIZE(OS_HEAP_M3_NOCACHEABLE_SIZE,8)] HEAP_ATTR(".local_data_m3_nocacheable_bss", HEAP_MIN_ALIGN);
#endif  // OS_HEAP_M3_NOCACHEABLE_SIZE
#endif  // (OS_HEAP_M3_NOCACHEABLE_SIZE > 0)


/********************************* Local M3 cacheable ****************************/
#ifdef OS_HEAP_M3_SIZE
#if (OS_HEAP_M3_SIZE > 0)
uint8_t    g_heap_m3[ALIGN_SIZE(OS_HEAP_M3_SIZE,ARCH_CACHE_LINE_SIZE)] HEAP_ATTR(".local_data_m3_bss", ARCH_CACHE_LINE_SIZE);
#endif
#endif


/********************************* Local DDR0 (non-cacheable) ********************/
#ifdef OS_HEAP_DDR0_NOCACHEABLE_SIZE
#if (OS_HEAP_DDR0_NOCACHEABLE_SIZE > 0)
uint8_t    g_heap_ddr0_nocacheable[ALIGN_SIZE(OS_HEAP_DDR0_NOCACHEABLE_SIZE,8)] HEAP_ATTR(".local_data_ddr0_nocacheable_bss", HEAP_MIN_ALIGN);
#endif  // (OS_HEAP_DDR0_NOCACHEABLE_SIZE > 0)
#endif  // OS_HEAP_DDR0_NOCACHEABLE_SIZE


/********************************* Local DDR0 cacheable **************************/
#ifdef OS_HEAP_DDR0_SIZE
#if (OS_HEAP_DDR0_SIZE > 0)
uint8_t    g_heap_ddr0[ALIGN_SIZE(OS_HEAP_DDR0_SIZE,ARCH_CACHE_LINE_SIZE)] HEAP_ATTR(".local_data_ddr0_bss", ARCH_CACHE_LINE_SIZE);
#endif  // (OS_HEAP_DDR0_SIZE > 0)
#endif  // OS_HEAP_DDR0_SIZE


/********************************* Local DDR1 (non-cacheable) ********************/
#ifdef OS_HEAP_DDR1_NOCACHEABLE_SIZE
#if (OS_HEAP_DDR1_NOCACHEABLE_SIZE > 0)
uint8_t    g_heap_ddr1_nocacheable[ALIGN_SIZE(OS_HEAP_DDR1_NOCACHEABLE_SIZE,8)] HEAP_ATTR(".local_data_ddr1_nocacheable_bss", HEAP_MIN_ALIGN);
#endif  // OS_HEAP_DDR1_NOCACHEABLE_SIZE
#endif  // (OS_HEAP_DDR1_NOCACHEABLE_SIZE > 0)


/********************************* Local DDR1 cacheable **************************/
#ifdef OS_HEAP_DDR1_SIZE
#if (OS_HEAP_DDR1_SIZE > 0)
uint8_t    g_heap_ddr1[ALIGN_SIZE(OS_HEAP_DDR1_SIZE,ARCH_CACHE_LINE_SIZE)] HEAP_ATTR(".local_data_ddr1_bss", ARCH_CACHE_LINE_SIZE);
#endif
#endif


#if (OS_MULTICORE == ON)

/********************************************************************************/
/********************************* Shared heaps *********************************/
/********************************************************************************/

/****************************** Shared M3 cacheable *****************************/
#ifdef OS_SHARED_MEM_M3_SIZE_CACHEABLE
#if (OS_SHARED_MEM_M3_SIZE_CACHEABLE > 0)
uint8_t    g_shared_mem_heap_m3_cacheable[OS_SHARED_MEM_M3_SIZE_CACHEABLE] HEAP_ATTR(".shared_data_m3_cacheable_bss", ARCH_CACHE_LINE_SIZE);

#if (OS_SHARED_MEM_M3_SIZE_CACHEABLE & (ARCH_CACHE_LINE_SIZE - 1))
#error "Cacheable heap size must be aligned to cache line"
#endif

#endif  // (OS_SHARED_MEM_M3_SIZE_CACHEABLE > 0)
#endif  // OS_SHARED_MEM_M3_SIZE_CACHEABLE


/********************************* Shared DDR0 (non-cacheable) ******************/
#ifdef OS_SHARED_MEM_DDR0_SIZE
#if (OS_SHARED_MEM_DDR0_SIZE > 0)
uint8_t    g_shared_mem_heap_ddr0[ALIGN_SIZE(OS_SHARED_MEM_DDR0_SIZE,8)] HEAP_ATTR(".shared_data_ddr0_bss", HEAP_MIN_ALIGN);
#endif  // (OS_SHARED_MEM_DDR0_SIZE > 0)
#endif  // OS_SHARED_MEM_DDR0_SIZE


/********************************* Shared DDR0 cacheable ************************/
#ifdef OS_SHARED_MEM_DDR0_SIZE_CACHEABLE
#if (OS_SHARED_MEM_DDR0_SIZE_CACHEABLE > 0)
uint8_t    g_shared_mem_heap_ddr0_cacheable[OS_SHARED_MEM_DDR0_SIZE_CACHEABLE] HEAP_ATTR(".shared_data_ddr0_cacheable_bss", ARCH_CACHE_LINE_SIZE);

/* check - cacheable size must be 256 aligned    */
#if (OS_SHARED_MEM_DDR0_SIZE_CACHEABLE & (ARCH_CACHE_LINE_SIZE - 1))
#error "Cacheable heap size must be aligned to cache line"
#endif

#endif  // (OS_SHARED_MEM_DDR0_SIZE_CACHEABLE > 0)
#endif  // OS_SHARED_MEM_DDR0_SIZE_CACHEABLE


/********************************* Shared DDR1 (non-cacheable) ******************/
#ifdef OS_SHARED_MEM_DDR1_SIZE
#if (OS_SHARED_MEM_DDR1_SIZE > 0)
uint8_t    g_shared_mem_heap_ddr1[ALIGN_SIZE(OS_SHARED_MEM_DDR1_SIZE,8)] HEAP_ATTR(".shared_data_ddr1_bss", HEAP_MIN_ALIGN);
#endif  // (OS_SHARED_MEM_DDR1_SIZE > 0)
#endif  // OS_SHARED_MEM_DDR1_SIZE


/********************************* Shared DDR1 cacheable ************************/
#ifdef OS_SHARED_MEM_DDR1_SIZE_CACHEABLE
#if (OS_SHARED_MEM_DDR1_SIZE_CACHEABLE > 0)
uint8_t    g_shared_mem_heap_ddr1_cacheable[OS_SHARED_MEM_DDR1_SIZE_CACHEABLE] HEAP_ATTR(".shared_data_ddr1_cacheable_bss", ARCH_CACHE_LINE_SIZE);

#if (OS_SHARED_MEM_DDR1_SIZE_CACHEABLE & (ARCH_CACHE_LINE_SIZE - 1))
#error "Cacheable heap size must be aligned to cache line"
#endif

#endif  // (OS_SHARED_MEM_DDR1_SIZE_CACHEABLE > 0)
#endif  // OS_SHARED_MEM_DDR1_SIZE_CACHEABLE


#endif // OS_MULTICORE

