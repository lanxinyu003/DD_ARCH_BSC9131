/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/05/19 11:00:26 $
 $Id: sc3x00_system_mmu.c,v 1.10 2013/05/19 11:00:26 b08551 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_system_mmu.c,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************
 
 @File          sc3x00_system_mmu.c

 @Cautions      None.
 
*//***************************************************************************/
#include "smartdsp_os_.h"
#include "os_init.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "sc3x00_mmu.h"
#include "sc3x00_mmu_.h"

#include <att_mmu.h>
#include <overlay.h>
#include <os_mmu.h>

#define SC3X00_MMU   (SOC_DSP_PLAT_DEFAULT + 0x6000)

/* For cases that the stack is not placed with the .att_mmu
 * The value should be even (because of 2n & 2n+1 assumption of the heap & heap non cacheable).
 * If value is changed, psc9x3x_asm_start.asm should be changed as well (MMU address).
 */
#define STACK_SEGMENT_OTHER_THAN_0    2

/* The MMU initialization code is separated into two parts:  */
/* 1. this file implements linker (cw) dependant code  */
/* 2. sc3x00_mmu.c file includes code that does not depend on the linker */
/* This initilization routines should be called only once at initialization phase */

/* It initializes MMU according to linker settings                        */
/* and sets up global table for each segment  os_global_prog_segments and */
/* os_global_data_segments.                                                   */
/* Those tables contain information on what context ID each segment belongs to */
/* MMU segments that defined as "SYSTEM" are enabled, all other only programmed */

extern int os_global_num_of_data_seg;
extern int os_global_num_of_prog_seg;
extern uint8_t os_global_prog_segments [NUMBER_OF_MMU_PROG_MATT];
extern uint8_t os_global_data_segments [NUMBER_OF_MMU_DATA_MATT];

/* non-cacheable part of local memory           */
/* it must be specially treated in MMU mappings */
extern uint8_t   g_heap_nocache[];
extern uint32_t  g_heap_nocache_size;

/* kernel awerness log buffer location */
extern volatile uint32_t   *_os_base_pointer;
extern volatile uint32_t   *_os_log_end_pointer;

/* this functions are called by start-up code */
void __target_setting();   // second hook
void __target_c_start();   // third hook


/* this is linker defined symbols for start of Local Segment & stack segment */
extern uint32_t _LNK_LocalData_c0_b;
extern uint32_t _LNK_LocalData_c1_b;
extern uint32_t _LNK_LocalData_c2_b;
extern uint32_t _LNK_LocalData_c3_b;
extern uint32_t _LNK_LocalData_c4_b;
extern uint32_t _LNK_LocalData_c5_b;
extern uint32_t _LNK_Local_StackStart_c0;
extern uint32_t _LNK_Local_StackStart_c1;
extern uint32_t _LNK_Local_StackStart_c2;
extern uint32_t _LNK_Local_StackStart_c3;
extern uint32_t _LNK_Local_StackStart_c4;
extern uint32_t _LNK_Local_StackStart_c5;

/* this function is called as part of SDOS init */
os_status sysInit();


asm static void mattWrite(unsigned int *adr_d_a,uint32_t matt_a,
        unsigned int *adr_d_b,uint32_t matt_b,
        unsigned int *adr_d_c,uint32_t matt_c)
{
asm_header
    .arg
        _adr_d_a in $r0;
        _matt_a in $d0;
        _adr_d_b in $r1;
        _matt_b in $d1;
        _adr_d_c in $r2;
        _matt_c in $d2;
    .reg $d0,$r0,$d1,$r1,$d2,$r2;
asm_body
 
    move.l d0,(r0)
    move.l d1,(r1)
    move.l d2,(r2)
 
    syncio
asm_end
}

static uint32_t getLocalStackAddress(uint8_t core_id)
{
    OS_ASSERT_COND((0 <= core_id) && (5 >= core_id));
    if (core_id == 0)
        return (uint32_t) &_LNK_Local_StackStart_c0;
    else if (core_id == 1)
        return (uint32_t) &_LNK_Local_StackStart_c1;
    else if (core_id == 2)
        return (uint32_t) &_LNK_Local_StackStart_c2;
    else if (core_id == 3)
        return (uint32_t) &_LNK_Local_StackStart_c3;
    else if (core_id == 4)
        return (uint32_t) &_LNK_Local_StackStart_c4;
    else if (core_id == 5)
        return (uint32_t) &_LNK_Local_StackStart_c5;
 
    debug();
    return 0;
}

static uint32_t getLocalDataAddress(uint8_t core_id)
{
    OS_ASSERT_COND((0 <= core_id) && (5 >= core_id));
 
    if (core_id == 0)
        return (uint32_t) &_LNK_LocalData_c0_b;
    else if (core_id == 1)
        return (uint32_t) &_LNK_LocalData_c1_b;
    else if (core_id == 2)
        return (uint32_t) &_LNK_LocalData_c2_b;
    else if (core_id == 3)
        return (uint32_t) &_LNK_LocalData_c3_b;
    else if (core_id == 4)
        return (uint32_t) &_LNK_LocalData_c4_b;
    else if (core_id == 5)
        return (uint32_t) &_LNK_LocalData_c5_b;
    debug();
    return 0;
}

/***********************************************************************************/

/* The method unmaps all segments except for first (0) and possibly stack segment (depends on boolean) */
static void mmuUnMapAll(bool stack_segment_exclusion_needed)
{
    dsp_plat_mmu_map_t* mmu = (dsp_plat_mmu_map_t*)SC3X00_MMU;
    int matt_index;

    /* unmap all data segments except first, local data */
    for(matt_index = 1; matt_index <NUMBER_OF_MMU_DATA_MATT; matt_index++)
    {
        if((matt_index == STACK_SEGMENT_OTHER_THAN_0) && stack_segment_exclusion_needed)
            continue;

        mattWrite((unsigned int *)&(mmu->m_dsd[matt_index].m_dsda),(uint32_t)0,
                (unsigned int *)&(mmu->m_dsd[matt_index].m_dsdc),(uint32_t)0,
                (unsigned int *)&(mmu->m_dsd[matt_index].m_dsdb),(uint32_t)0);
 
    }

    /* unmap all program segments */
    for(matt_index = 0; matt_index <NUMBER_OF_MMU_PROG_MATT; matt_index++)
    {
        mattWrite((unsigned int *)&(mmu->m_psd[matt_index].m_psda),0,
                    (unsigned int *)&(mmu->m_psd[matt_index].m_psdc),0,
                    (unsigned int *)&(mmu->m_psd[matt_index].m_psdb),0);
 
    }

}

/***********************************************************************************/

/* find mmu segment with vaddr within */
static int mmuFindDataSegment(uint32_t vaddr, struct att_mmu* att_mmu_table,int mmu_table_size)
{
    uint32_t vaddr_offset;
    int i;

    for (i=0; i<mmu_table_size ; i++)
    {
        /* this is program entry */
        if(att_mmu_table[i].other & OVL_OTHER_EXEC) continue;

        /* check if address within limits */
        vaddr_offset = vaddr - ((uint32_t)att_mmu_table[i].base_address);
        if(vaddr_offset < att_mmu_table[i].physical_size) return i;
    }

    /* not found */
    return -1;
}


/***********************************************************************************/
static void mmuMapDataSegment(  int matt_index, struct att_mmu* att_mmu_entry)
{
    dsp_plat_mmu_map_t* mmu = (dsp_plat_mmu_map_t*)SC3X00_MMU;
    uint32_t address_mask = att_mmu_entry->physical_size-1;
    uint32_t matt_a, matt_b, matt_c = 0;

    // check the minimal alignment
    if(((uint32_t)att_mmu_entry->base_address     & 0xFF) ||
       ((uint32_t)att_mmu_entry->physical_address & 0xFF))
       asm("    debug");

    OS_ASSERT_COND(matt_index < NUMBER_OF_MMU_DATA_MATT);

    if (att_mmu_entry->other & OVL_OTHER_FLEXIBLE_SEGMENT_MODE)
    {
        // check size
        if ((att_mmu_entry->physical_size < 256) ||
            (att_mmu_entry->physical_size > (0x800000 - 0x10000)))
            asm(" debug");
 
        if (att_mmu_entry->physical_size & 0xFF)
            asm(" debug");
 
        matt_a = (uint32_t)att_mmu_entry->base_address;
        matt_a |= att_mmu_entry->attribute & 0x3F;
 
        matt_c = att_mmu_entry->physical_size >> 8;
        matt_c |= att_mmu_entry->attribute & 0xF0000000;

    }
    else
    {
        // check size (must be power 2 and bigger 256)
        if(( att_mmu_entry->physical_size < 256) ||
           (att_mmu_entry->physical_size & address_mask))
           asm("    debug");

        // check alignment
        if(((uint32_t)att_mmu_entry->base_address     & address_mask) ||
           ((uint32_t)att_mmu_entry->physical_address & address_mask))
           asm("    debug");
 
        matt_a = att_mmu_entry->physical_size>>1;
        matt_a |= (uint32_t)att_mmu_entry->base_address;
        matt_a |= att_mmu_entry->attribute & 0x3F;
 
    }
 
    matt_b =  (uint32_t)att_mmu_entry->physical_address;
    matt_b |= (att_mmu_entry->attribute >> 16) & 0xFF;
 
    // commit changes to mmu
    mattWrite((unsigned int *)&(mmu->m_dsd[matt_index].m_dsda),matt_a,
                (unsigned int *)&(mmu->m_dsd[matt_index].m_dsdb),matt_b,
                (unsigned int *)&(mmu->m_dsd[matt_index].m_dsdc),matt_c);
        return;


}

/***********************************************************************************/

static void mmuMapProgramSegment(int matt_index, struct att_mmu* att_mmu_entry)
{
    dsp_plat_mmu_map_t* mmu = (dsp_plat_mmu_map_t*)SC3X00_MMU;
    uint32_t address_mask = att_mmu_entry->physical_size-1;
    uint32_t matt_a, matt_b, matt_c = 0;

    // check the minimal alignment
    if(((uint32_t)att_mmu_entry->base_address     & 0xFF) ||
       ((uint32_t)att_mmu_entry->physical_address & 0xFF))
       asm("    debug");

    OS_ASSERT_COND(matt_index < NUMBER_OF_MMU_PROG_MATT);
 
    if (att_mmu_entry->other & OVL_OTHER_FLEXIBLE_SEGMENT_MODE)
    {
        // check size
        if ((att_mmu_entry->physical_size < 256) ||
            (att_mmu_entry->physical_size > (0x800000 - 0x10000)))
            asm(" debug");
 
        if (att_mmu_entry->physical_size & 0xFF)
            asm(" debug");
 
        matt_a = (uint32_t)att_mmu_entry->base_address;
        matt_a |= att_mmu_entry->attribute & 0x3F;
 
        matt_c = att_mmu_entry->physical_size >> 8;
        matt_c |= att_mmu_entry->attribute & 0xF0000000;

    }
    else
    {
        // check size (must be power 2 and bigger 256)
        if(( att_mmu_entry->physical_size < 256) ||
           (att_mmu_entry->physical_size & address_mask))
           asm("    debug");

        // check alignment
        if(((uint32_t)att_mmu_entry->base_address     & address_mask) ||
           ((uint32_t)att_mmu_entry->physical_address & address_mask))
           asm("    debug");
 
        matt_a = att_mmu_entry->physical_size>>1;
        matt_a |= (uint32_t)att_mmu_entry->base_address;
        matt_a |= att_mmu_entry->attribute & 0x3F;
 
    }
 
    matt_b =  (uint32_t)att_mmu_entry->physical_address;
    matt_b |= (att_mmu_entry->attribute >> 16) & 0xFF;
 
    // commit changes to mmu
    mattWrite((unsigned int *)&(mmu->m_psd[matt_index].m_psdc),matt_c,
                (unsigned int *)&(mmu->m_psd[matt_index].m_psdb),matt_b,
                (unsigned int *)&(mmu->m_psd[matt_index].m_psda),matt_a);

 
}

/***********************************************************************************/


/***********************************************************************************//*

     This code is running at second hook of SC3400 startup sequence.

     No global variable access is permitted at this stage.
 
     We map only system data segments to allow .bss and .rom section initialization.
 
     This hook can be disabled when compiled for non-standalone mode.
 
*//***********************************************************************************/

void __target_c_start()
{
    struct att_mmu* address_translation_table_mmu = _address_translation_table_mmu;
    int address_translation_table_mmu_count = (int)_address_translation_table_mmu_count;
    struct att_mmu  att_mmu_segment;
    int    d_matt_counter = 0;
    int    att_mmu_segment_index;
    int    stack_data_segment_index;
    int    i;
    uint32_t* reg_ptr = (uint32_t*)0xFFF06028;
    uint8_t core_id = (uint8_t)((*reg_ptr & 0x00FF0000) >> 16);
    uint32_t stack_addr = (uint32_t) getLocalStackAddress(core_id); // compute address according to core offset
    uint32_t local_data_addr = (uint32_t) getLocalDataAddress(core_id);
 
    /* find stack segment */
    stack_data_segment_index = mmuFindDataSegment((uint32_t)stack_addr,address_translation_table_mmu,address_translation_table_mmu_count);
    if(stack_data_segment_index < 0) asm("  debug");

    /* find .att_mmu segment */
    att_mmu_segment_index = mmuFindDataSegment(local_data_addr,address_translation_table_mmu,address_translation_table_mmu_count);
    if(att_mmu_segment_index < 0) asm(" debug");

    /* allocate data descriptor 0 for .att_mmu & enable it */
    att_mmu_segment = address_translation_table_mmu[att_mmu_segment_index];
    att_mmu_segment.attribute |= MMU_MATT_ENABLE | (MMU_SYSTEM_PID << 24);
    if (att_mmu_segment.other & OVL_OTHER_FLEXIBLE_SEGMENT_MODE)
        att_mmu_segment.attribute |= MMU_FLEX_SEGMENT_MODEL;

    mmuMapDataSegment(d_matt_counter,&att_mmu_segment);
    d_matt_counter++;

    /* now map rest of segments */
    for (i=0; i<address_translation_table_mmu_count; i++)
    {
        /* get segment entry */
        att_mmu_segment = address_translation_table_mmu[i];

        /* set MMU_FLEX_SEGMENT_MODEL indication where necessary */
        if (att_mmu_segment.other & OVL_OTHER_FLEXIBLE_SEGMENT_MODE)
        {
            address_translation_table_mmu[i].attribute |= MMU_FLEX_SEGMENT_MODEL;
            att_mmu_segment.attribute |= MMU_FLEX_SEGMENT_MODEL;
        }
 
        /* skip .att_mmu segment & stack segment, already mapped */
        if((i == att_mmu_segment_index) || (i == stack_data_segment_index))
            continue;

        /* when the att_mmu and the stack are not in the same segment,
           that the stack will be in segment STACK_SEGMENT_OTHER_THAN_0 */
        if((d_matt_counter == STACK_SEGMENT_OTHER_THAN_0) && (att_mmu_segment_index != stack_data_segment_index))
            ++d_matt_counter;
 
        /* only system data segment are required at this point */
        if(!(att_mmu_segment.attribute & MMU_DATA_DEF_SYSTEM)) continue;
 
        /* enable system data segment */
        att_mmu_segment.attribute |= MMU_MATT_ENABLE | (MMU_SYSTEM_PID << 24);
        mmuMapDataSegment(d_matt_counter, &att_mmu_segment);
        d_matt_counter++;
    }
}

/***********************************************************************************/


/***********************************************************************************//*

     This code is running at third hook of SC3400 startup sequence.

     All global and .bss variables are initiated.
 
     We initialize MMU system settings in HW and OS structures.
 
     This hook is mandatory for SmartDSP OS
 
*//***********************************************************************************/
void __target_setting()
{
    struct att_mmu* address_translation_table_mmu = _address_translation_table_mmu;
    int address_translation_table_mmu_count = (int)_address_translation_table_mmu_count;
    struct att_mmu  att_mmu_segment;
    int  p_matt_counter = 0, d_matt_counter = 0;
    int  att_mmu_segment_index;
    int  stack_data_segment_index;
    uint32_t g_heap_nocache_phys;
    int    i;
    bool mmu_and_stack_in_diff_segments;
    uint32_t* reg_ptr = (uint32_t*)0xFFF06028;
    uint8_t core_id = (uint8_t)((*reg_ptr & 0x00FF0000) >> 16);
    uint32_t stack_addr = (uint32_t) getLocalStackAddress(core_id); // compute address according to core offset
    uint32_t local_data_addr = (uint32_t) getLocalDataAddress(core_id);

    /* find stack segment */
    stack_data_segment_index = mmuFindDataSegment((uint32_t)stack_addr,address_translation_table_mmu,address_translation_table_mmu_count);
    if(stack_data_segment_index < 0) asm("  debug");

    /* find global_data segment */
    att_mmu_segment_index = mmuFindDataSegment((uint32_t)local_data_addr,address_translation_table_mmu,address_translation_table_mmu_count);
    if(att_mmu_segment_index < 0) asm(" debug");

    OS_ASSERT_COND(att_mmu_segment_index < NUMBER_OF_MMU_DATA_MATT);
    OS_ASSERT_COND(stack_data_segment_index < NUMBER_OF_MMU_DATA_MATT);
    OS_ASSERT_COND(att_mmu_segment_index >= 0);
    OS_ASSERT_COND(stack_data_segment_index >= 0);
 
    mmu_and_stack_in_diff_segments = (stack_data_segment_index != att_mmu_segment_index);
    /* remap MMU once again, now when we have all static and global variables */
    /* unmap all segments except for segment 0 and possibly segment 2 */
    mmuUnMapAll(mmu_and_stack_in_diff_segments);
 
 
    /* allocate data descriptor 0 for global_data & enable it */
    att_mmu_segment = address_translation_table_mmu[att_mmu_segment_index];
    os_global_data_segments[att_mmu_segment_index] = (uint8_t)MMU_SYSTEM_PID;
    d_matt_counter++;

    // Assumption - the heap is location in the same section as the stack
    att_mmu_segment = address_translation_table_mmu[stack_data_segment_index];
    if (mmu_and_stack_in_diff_segments)
    {
        os_global_data_segments[stack_data_segment_index] = (uint8_t)MMU_SYSTEM_PID;
    }

    /* non-cacheable local memory must be created */
    if(g_heap_nocache_size)
    {
        /* noncacheable heap offset from start of local_data */
        g_heap_nocache_phys  = (uint32_t)att_mmu_segment.physical_address;
        g_heap_nocache_phys += (g_heap_nocache - att_mmu_segment.base_address);

        att_mmu_segment.base_address      = g_heap_nocache;
        att_mmu_segment.physical_address  = (void*)g_heap_nocache_phys;
        att_mmu_segment.physical_size    = g_heap_nocache_size;
        att_mmu_segment.attribute &= ~(MMU_DATA_CACHEABLE_WRITETHROUGH|MMU_DATA_CACHEABLE_WRITEBACK|
                                       MMU_DATA_L2_CACHEABLE_WRITETHROUGH|MMU_DATA_L2_CACHEABLE_WRITEBACK);
        att_mmu_segment.attribute |= (MMU_MATT_ENABLE|MMU_DATA_NONCACHEABLE_WRITETHROUGH|MMU_DATA_L2_NONCACHEABLE);

        if(mmu_and_stack_in_diff_segments)
        {
            // Heap non cacheable should be in STACK_SEGMENT_OTHER_THAN_0 + 1 segment (2n + 1 assumption)
            os_global_data_segments[STACK_SEGMENT_OTHER_THAN_0 + 1] = (uint8_t)(att_mmu_segment.attribute >> 24);
            mmuMapDataSegment(STACK_SEGMENT_OTHER_THAN_0 + 1,&att_mmu_segment);
        }
        else
        {
            os_global_data_segments[d_matt_counter] = (uint8_t)(att_mmu_segment.attribute >> 24);
            mmuMapDataSegment(d_matt_counter,&att_mmu_segment);
            d_matt_counter++;
        }
    }

    /* now map rest of segments */
    for (i=0; i<address_translation_table_mmu_count; i++)
    {
        /* skip local data segment, already mapped */
        if((i == att_mmu_segment_index) || (i == stack_data_segment_index)) continue;

        /* get segment entry */
        att_mmu_segment = address_translation_table_mmu[i];

        /* enable all system data/program segments */
        if((att_mmu_segment.attribute) & (MMU_PROG_DEF_SYSTEM|MMU_DATA_DEF_SYSTEM))
            att_mmu_segment.attribute |= MMU_MATT_ENABLE | (MMU_SYSTEM_PID << 24);

        /* this is PROGRAM */
        if(address_translation_table_mmu[i].other & OVL_OTHER_EXEC)
        {
            os_global_prog_segments[p_matt_counter] = (uint8_t)((att_mmu_segment.attribute & ~MMU_PSDC_ATTR_MASK)>> 24);
            mmuMapProgramSegment(p_matt_counter, &att_mmu_segment);
            p_matt_counter++;
        }
        else
        {
            /* this is DATA */
            if(mmu_and_stack_in_diff_segments)
            {
                /* Assumption - if the stack is not in segment 0, it is in segment STACK_SEGMENT_OTHER_THAN_0 */
                if(d_matt_counter == STACK_SEGMENT_OTHER_THAN_0)
                {
                    d_matt_counter = STACK_SEGMENT_OTHER_THAN_0 + 2; // skip non-cacheable
                }
            }
            os_global_data_segments[d_matt_counter] = (uint8_t)((att_mmu_segment.attribute & ~MMU_PSDC_ATTR_MASK)>> 24);
            mmuMapDataSegment(d_matt_counter, &att_mmu_segment);
            d_matt_counter++;
        }

    }

    os_global_num_of_data_seg = d_matt_counter;
    os_global_num_of_prog_seg = p_matt_counter;
}


/*****************************************************************************/
os_status sc3x00MmuInitPlatform(platform_init_params_t* platform_params)
{
    dsp_plat_mmu_map_t* mmu = (dsp_plat_mmu_map_t*)SC3X00_MMU;
    uint32_t cr = mmu->m_cr;

    if(platform_params->abtbf)
        cr |= M_CR_ABTF;
    else
        cr &= ~M_CR_ABTF;

    if(platform_params->ccee)
        cr |= M_CR_CCEE;
    else
        cr &= ~M_CR_CCEE;

    if(platform_params->cvae)
        cr |= M_CR_CVAE;
    else
        cr &= ~M_CR_CVAE;

    if(platform_params->dnamee)
        cr |= M_CR_DNAMEE;
    else
        cr &= ~M_CR_DNAMEE;
    if(platform_params->wsbee)
    {
        /* Errata (CORESUBS2) - bit not supported in 8154/6 rev1. Check M_PIR[PVN] */
        if ((GET_UINT32(g_dsp_plat_map->mmu.m_pir) & 0x0000FFFF) == 0x00000003)
            OS_ASSERT;
        else
            cr |= M_CR_WSBEE;
    }
    else
        cr &= ~M_CR_WSBEE;

    if(platform_params->dpue)
        cr |= M_CR_DPUE;
    else
        cr &= ~M_CR_DPUE;

    if(platform_params->mpe)
        cr |= M_CR_MPE;
    else
        cr &= ~M_CR_MPE;

    if(platform_params->ate)
        cr |= M_CR_ATE;
    else
        cr &= ~M_CR_ATE;

    if(platform_params->edcee)
        cr |= M_CR_EDCEE;
    else
        cr &= ~M_CR_EDCEE;

    WRITE_SYNCIO_UINT32(mmu->m_cr,cr);
    return OS_SUCCESS;
}
/*****************************************************************************/

/* System initialization after main() */
os_status sysInit()
{
    os_status status = OS_SUCCESS;
    /* Create Entry for Kernel Awareness, if required */
#if defined(HWI_LOGGING) || defined(HWI_PRIORITY_LOGGING) || defined(SWI_LOGGING) || defined(TASK_LOGGING)
    struct att_mmu  att_mmu_entry;
    struct att_mmu* address_translation_table_mmu = _address_translation_table_mmu;
    int address_translation_table_mmu_count = (int)_address_translation_table_mmu_count;
 
    /* If mapped - leave as is. If not, map virtual to physical addressing as the same */
    if (mmuFindDataSegment((uint32_t)_os_base_pointer, address_translation_table_mmu, address_translation_table_mmu_count) == -1)
    {
        att_mmu_entry.base_address     = (void*)_os_base_pointer;
        att_mmu_entry.physical_address = (void*)_os_base_pointer;
        att_mmu_entry.physical_size = (uint32_t)_os_log_end_pointer-(uint32_t)_os_base_pointer;
        att_mmu_entry.attribute     = MMU_DATA_DEF_SYSTEM|
                                      MMU_DATA_DEF_WPERM_SUPER|MMU_DATA_DEF_RPERM_SUPER|
                                      MMU_DATA_CACHEABLE_WRITEBACK|MMU_DATA_PREFETCH_ENABLE|
                                      MMU_DATA_L2_CACHEABLE_WRITEBACK|MMU_MATT_ENABLE | MMU_FLEX_SEGMENT_MODEL;
 
        att_mmu_entry.other = OVL_OTHER_FLEXIBLE_SEGMENT_MODE;
        os_global_data_segments[os_global_num_of_data_seg] = (uint8_t)(att_mmu_entry.attribute >> 24);
        mmuMapDataSegment(os_global_num_of_data_seg,&att_mmu_entry);
        os_global_num_of_data_seg++;
    }
 
#endif

    return status;
}


