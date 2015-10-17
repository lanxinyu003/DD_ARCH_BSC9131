/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/04/22 12:44:00 $
 $Id: psc9x3x_shared_mem.c,v 1.12 2013/04/22 12:44:00 b42006 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/psc9x3x/psc9x3x_shared_mem.c,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_shared_mem.c

 @Description   Reserves shared memory space for the operating system.

 @Cautions      The shared memory variable name must remain as defined.

*//***************************************************************************/

#include "smartdsp_os.h"
#include "os_config.h"
#include "smartdsp_init.h"
#include "psc9x3x_init.h"

#if (OS_MULTICORE == ON)

#pragma data_seg_name ".os_shared_data"
#pragma bss_seg_name  ".os_shared_data_bss"


/* shared memory for OS use - the variable name must remain as defined ! */
uint8_t g_shared_mem[ALIGN_SIZE(OS_SHARED_MEM_SIZE,8)];
#pragma align g_shared_mem 8

#ifndef OS_SHARED_HEAP_MNGMNT_SIZE
#define OS_SHARED_HEAP_MNGMNT_SIZE OS_SHARED_HEAP_MNGMNT_DEFAULT_SIZE
#endif

#if OS_SHARED_HEAP_MNGMNT_SIZE > 0
#ifdef OS_SHARED_HEAP_MNGMNT_SECTION
uint8_t g_shared_mngmnt_space[OS_SHARED_HEAP_MNGMNT_SIZE] __attribute__ ((aligned (8))) __attribute__ ((section (OS_SHARED_HEAP_MNGMNT_SECTION)));
#else //default section
uint8_t g_shared_mngmnt_space[OS_SHARED_HEAP_MNGMNT_SIZE] __attribute__ ((aligned (8)));
#endif
#endif

/* shared memory for OS use - the variable name must remain as defined ! */
#ifdef OS_SHARED_MEM_M3_SIZE_CACHEABLE
#if (OS_SHARED_MEM_M3_SIZE_CACHEABLE > 0)
extern uint8_t    g_shared_mem_heap_m3_cacheable[OS_SHARED_MEM_M3_SIZE_CACHEABLE];
#if defined(PSC9131)
#error "M3 Not available on PSC9131"
#endif // PSC913x
#endif
#else
#define OS_SHARED_MEM_M3_SIZE_CACHEABLE 0
#endif

/* shared memory for OS use - the variable name must remain as defined ! */
#ifdef OS_SHARED_MEM_DDR0_SIZE
#if (OS_SHARED_MEM_DDR0_SIZE > 0)
extern uint8_t    g_shared_mem_heap_ddr0[ALIGN_SIZE(OS_SHARED_MEM_DDR0_SIZE,8)];
#ifdef PSC9131
#error "DDR0 Not available on PSC9131"
#endif // PSC9131
#endif
#else
#define OS_SHARED_MEM_DDR0_SIZE 0
#endif

/* shared memory for OS use - the variable name must remain as defined ! */
#ifdef OS_SHARED_MEM_DDR0_SIZE_CACHEABLE
#if (OS_SHARED_MEM_DDR0_SIZE_CACHEABLE > 0)
extern uint8_t    g_shared_mem_heap_ddr0_cacheable[OS_SHARED_MEM_DDR0_SIZE_CACHEABLE];
#ifdef PSC9131
#error "DDR0 Not available on PSC9131"
#endif // PSC9131
#endif
#else
#define OS_SHARED_MEM_DDR0_SIZE_CACHEABLE 0
#endif

/* shared memory for OS use - the variable name must remain as defined ! */
#ifdef OS_SHARED_MEM_DDR1_SIZE
#if (OS_SHARED_MEM_DDR1_SIZE > 0)
extern uint8_t    g_shared_mem_heap_ddr1[ALIGN_SIZE(OS_SHARED_MEM_DDR1_SIZE,8)];
#endif
#else
#define OS_SHARED_MEM_DDR1_SIZE 0
#endif

/* shared memory for OS use - the variable name must remain as defined ! */
#ifdef OS_SHARED_MEM_DDR1_SIZE_CACHEABLE
#if (OS_SHARED_MEM_DDR1_SIZE_CACHEABLE > 0)
extern uint8_t    g_shared_mem_heap_ddr1_cacheable[OS_SHARED_MEM_DDR1_SIZE_CACHEABLE];
#endif
#else
#define OS_SHARED_MEM_DDR1_SIZE_CACHEABLE 0
#endif

/* shared memory heap list for OS use - the variable name must remain as defined ! */
os_mem_heap_t g_mem_heap_shared[] =
{
    (volatile uint32_t*)(g_shared_mem + sizeof(g_shared_mem)),
    sizeof(g_shared_mem),
    OS_MEM_SHARED,
    (volatile void*)NULL,
    (volatile void*)NULL,

#if OS_SHARED_MEM_M3_SIZE_CACHEABLE > 0
    (volatile uint32_t*)(g_shared_mem_heap_m3_cacheable + sizeof(g_shared_mem_heap_m3_cacheable)),
    sizeof(g_shared_mem_heap_m3_cacheable),
    OS_MEM_M3_SHARED_CACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_SHARED_MEM_DDR0_SIZE > 0
    (volatile uint32_t*)(g_shared_mem_heap_ddr0 + sizeof(g_shared_mem_heap_ddr0)),
    sizeof(g_shared_mem_heap_ddr0),
    OS_MEM_DDR0_SHARED,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_SHARED_MEM_DDR0_SIZE_CACHEABLE > 0
    (volatile uint32_t*)(g_shared_mem_heap_ddr0_cacheable + OS_SHARED_MEM_DDR0_SIZE_CACHEABLE),
    OS_SHARED_MEM_DDR0_SIZE_CACHEABLE,
    OS_MEM_DDR0_SHARED_CACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_SHARED_MEM_DDR1_SIZE > 0
    (volatile uint32_t*)(g_shared_mem_heap_ddr1 + sizeof(g_shared_mem_heap_ddr1)),
    sizeof(g_shared_mem_heap_ddr1),
    OS_MEM_DDR1_SHARED,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

#if OS_SHARED_MEM_DDR1_SIZE_CACHEABLE > 0
    (volatile uint32_t*)(g_shared_mem_heap_ddr1_cacheable + OS_SHARED_MEM_DDR1_SIZE_CACHEABLE),
    OS_SHARED_MEM_DDR1_SIZE_CACHEABLE,
    OS_MEM_DDR1_SHARED_CACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,
#endif

    0x0, // dummy value, it will be initialized after psc9x3xHetInitialize()
    0x0,
    OS_MEM_HET_DDR1_CACHEABLE,
    (volatile void*)NULL,
    (volatile void*)NULL,

};

int g_num_of_shared_heaps = sizeof(g_mem_heap_shared)/sizeof(g_mem_heap_shared[0]);


#endif // OS_MULTICORE

