/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
 
/******************************************************************************
 $Date: 2014/09/30 13:35:15 $
 $Id: os_mpic_init.c,v 1.20 2014/09/30 13:35:15 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_mpic_init.c,v $
 $Revision: 1.20 $
******************************************************************************/
 
/******************************************************************************
 
 @File          os_mpic.c

 @Description   OS Mpic module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_.h"
#include "os_mpic_.h"
#include "os_mpic.h"
#include "os_list_.h"

#if defined(B4860) || defined(B4420)
#include "b486x_memmap.h"
#include "b486x_mpic.h"
#elif defined(B3421)
#include "b342x_memmap.h"
#include "b342x_mpic.h"
#endif // B4860 || B4420

#ifndef DISABLE_ALL_ERROR_CHECKING
#define MPIC_ERROR_CHECK
#endif//DISABLE_ALL_ERROR_CHECKING

/* Global for fast implementation of MPIC_Intr() */
extern os_mpic_t *g_dsp_mpic_handle;

/******************************************************************************/
void osMpicUnassignedIsr(os_mpic_arg arg)
{
    VAR_UNUSED(arg);
    OS_ASSERT;
}

/*****************************************************************************/
void osMpicErrorIsr(os_mpic_arg arg)
{
    uint32_t    isr_index = MPIC_MULTI_SRC_OFFSET(arg);
    uint32_t    events;

    /* Read event register and clear any umasked bits */
    events = GET_UINT32(g_soc_ccsr_map->mpic_memmap.mpic_global_memmap.global_interrupt_summary.eisr);

    /* Error interrupt handling */
    while (events)
    {
        if (events & EISR_EVENT_MASK)
        {
            OS_ASSERT_COND(g_dsp_mpic_handle->isr_table[isr_index].isr != osMpicUnassignedIsr);
            (MPIC_ISR(g_dsp_mpic_handle->isr_table[isr_index].isr))(g_dsp_mpic_handle->isr_table[isr_index].arg);
        }

        /* Move to check the next bit */
        events <<= 1;
        isr_index++;
    }
}

/*****************************************************************************/
void osMpicMultiSourceIsr(os_mpic_arg arg)
{
    uint16_t    isr_index = MPIC_MULTI_SRC_OFFSET(arg);
    uint16_t    isr_count = MPIC_MULTI_SRC_COUNT(arg);
    bool        found_isr = FALSE;

    while (isr_count)
    {
        if ( (g_dsp_mpic_handle->isr_table[isr_index].isr) != osMpicUnassignedIsr)
        {
            (MPIC_ISR(g_dsp_mpic_handle->isr_table[isr_index].isr))(g_dsp_mpic_handle->isr_table[isr_index].arg);
            found_isr = TRUE;
        }
        isr_index++;
        isr_count--;
        OS_ASSERT_COND(isr_index < (MPIC_NUM_OF_INTR_SOURCES + MPIC_MULTI_SRC_INTERNAL_INTERRUPTS_COUNT + 1));
    }
    if (!found_isr)
    {
           OS_ASSERT;
    }
}

/*******************************************************************************/

/*******************************************************************************/
void     osMpicGetIntrConfigRegs(    uint16_t           grp,
                                     uint16_t           idx,
                                     uint32_t  **vpr,
                                     uint32_t  **dr,
                                     uint32_t  **lr)
{
    mpic_memmap_t       *mpic_memmap = &g_soc_ccsr_map->mpic_memmap;
    uint32_t   *tmp_vpr = NULL;
    uint32_t   *tmp_dr = NULL;
    uint32_t   *tmp_lr = NULL;
    uint32_t msi_grp_offset = 0;
 
 
    if (grp == OS_MPIC_INTR_GROUP_INTERNAL)
    {
        if (idx < MPIC_NUM_OF_LOW_INT_INTRS)
        {
            tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_low.iirl[idx].iivpr);
            tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_low.iirl[idx].iidr);
            tmp_lr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_low.iirl[idx].iilr);
        }
        else
        {
            idx-=MPIC_NUM_OF_LOW_INT_INTRS;
            tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_high.iirh[idx].iivpr);
            tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_high.iirh[idx].iidr);
            tmp_lr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.internal_interrupt_configuration_high.iirh[idx].iilr);
        }
    }
    else if (grp == OS_MPIC_INTR_GROUP_EXTERNAL)
    {
        tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.external_interrupt_configuration.eir[idx].eivpr);
        tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.external_interrupt_configuration.eir[idx].eidr);
        tmp_lr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.external_interrupt_configuration.eir[idx].eilr);
    }
    else if (grp == OS_MPIC_INTR_GROUP_TIMERS)
    {
        if (idx < NUM_OF_TIMERS_PER_GROUP)
        {
            tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_global_memmap.global_timer_a.gtr[idx].gtvpr);
            tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_global_memmap.global_timer_a.gtr[idx].gtdr);
        }
        else
        {
            idx -= NUM_OF_TIMERS_PER_GROUP;
            tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_global_memmap.global_timer_b.gtr[idx].gtvpr);
            tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_global_memmap.global_timer_b.gtr[idx].gtdr);
        }
        /* No level registers for Timers */
    }
    else if (grp == OS_MPIC_INTR_GROUP_MSG)
    {
        if (idx < NUM_OF_MSG_INTR_PER_GROUP)
        {
            if (idx < NUM_OF_LOW_MSG_INTR_PER_GROUP)
            {
                tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_low_a[idx].mivpr);
                tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_low_a[idx].midr);
            /* No level registers for MSG */
            }
            else
            {
                idx -= NUM_OF_LOW_MSG_INTR_PER_GROUP;
                tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_high_a[idx].mivpr);
                tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_high_a[idx].midr);
            /* No level registers for MSG */
            }
        }
        else //Group B
        {
            idx -= NUM_OF_MSG_INTR_PER_GROUP;
            if (idx < NUM_OF_LOW_MSG_INTR_PER_GROUP)
            {
                tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_low_b[idx].mivpr);
                tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_low_b[idx].midr);
            /* No level registers for MSG */
            }
            else
            {
                idx -= NUM_OF_LOW_MSG_INTR_PER_GROUP;
                tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_high_b[idx].mivpr);
                tmp_dr  = (uint32_t *)&(mpic_memmap->mpic_int_src_memmap.msg_interrupt_configuration.mir_high_b[idx].midr);
            /* No level registers for MSG */
            }
        }
    }
    else if (grp == OS_MPIC_INTR_GROUP_SHARED_MSG)
    {
        while(idx >= NUM_OF_MSI_INTR_PER_GROUP)
        {
            idx -= NUM_OF_MSI_INTR_PER_GROUP;
            msi_grp_offset += (sizeof ( mpic_msi_interrupt_registers_memmap_t ) * (NUM_OF_LOW_MSI_INTR_PER_GROUP/4) );
            /* offset to the next group of (msi low msi group size)/(32bit)*/
        }
        if(idx < NUM_OF_LOW_MSI_INTR_PER_GROUP) //low interrupt
        {
            tmp_vpr = (uint32_t *)(&(mpic_memmap->mpic_int_src_memmap.msi_interrupt_configuration.mir_low_a[idx].mivpr)+ (msi_grp_offset));
            tmp_dr  = (uint32_t *)(&(mpic_memmap->mpic_int_src_memmap.msi_interrupt_configuration.mir_low_a[idx].midr)+ (msi_grp_offset));
            /* No level registers for MSG */
        }
        else //high interrupt
        {
            idx -= NUM_OF_LOW_MSI_INTR_PER_GROUP;
            tmp_vpr = (uint32_t *)(&(mpic_memmap->mpic_int_src_memmap.msi_interrupt_configuration.mir_high_a[idx].mivpr)+ (msi_grp_offset));
            tmp_dr  = (uint32_t *)(&(mpic_memmap->mpic_int_src_memmap.msi_interrupt_configuration.mir_high_a[idx].midr)+ (msi_grp_offset));
            /* No level registers for MSG */
        }
     }
    else if (grp == OS_MPIC_INTR_GROUP_IPI)
    {
        tmp_vpr = (uint32_t *)&(mpic_memmap->mpic_global_memmap.gcr.ipivp[idx].ipivpr);
        /* No destination and level registers for IPI */
    }

    if (vpr)
        *vpr = tmp_vpr;
    if (dr)
        *dr  = tmp_dr;
    if (lr)
        *lr  = tmp_lr;
}

/****************************************************************************/
os_status osMpicGetMsiIndexReg(uint32_t mpic_intr_id, uint32_t **p_msiir1 ,uint8_t *msir_num)
{
    uint32_t *msiir1;
    uint32_t msi_grp_offset = 0;
    uint32_t msi_id;
 
    msi_id = MPIC_INTR_TO_IDX(mpic_intr_id);
 
    /* find the msi group */
    while(msi_id >= NUM_OF_MSI_INTR_PER_GROUP)
    {
        msi_id -= NUM_OF_MSI_INTR_PER_GROUP;
        msi_grp_offset+=((sizeof(mpic_global_shared_msi_index_grp_memmap_t))/4);
    }
 
    msiir1 = (uint32_t *)(&(g_soc_ccsr_map->mpic_memmap.mpic_global_memmap.global_shared_msi_index_grp[0].msiir1) + (msi_grp_offset));
 
    *msir_num = (uint8_t)msi_id;
    *p_msiir1 = msiir1;
 
    return OS_SUCCESS;
}

/****************************************************************************/
os_status osMpicGetMsiCoalescingReg(uint32_t msi_id, uint32_t **p_msicr)
{
    uint32_t *msicr;
    uint32_t msi_grp_offset = 0;

    /* find the msi group */
    while(msi_id >= NUM_OF_MSI_INTR_PER_GROUP)
    {
        msi_id -= NUM_OF_MSI_INTR_PER_GROUP;
        msi_grp_offset+=((sizeof(mpic_global_shared_msi_index_grp_memmap_t))/4);
    }
 
    msicr = (uint32_t *)(&(g_soc_ccsr_map->mpic_memmap.mpic_global_memmap.global_shared_msi_index_grp[0].msicr[msi_id]) + (msi_grp_offset));
 
    *p_msicr = msicr;
 
    return OS_SUCCESS;
}

/*******************************************************************************/
void osMpicSetInterruptMask(uint16_t grp, uint16_t idx, bool intr_enable)
{
    uint32_t   *p_vpr;
    uint32_t   vpr;

    osMpicGetIntrConfigRegs(grp, idx, &p_vpr, NULL, NULL);

    OS_ASSERT_COND(p_vpr);
    vpr = GET_UINT32(*p_vpr);

    if (intr_enable)
        WRITE_UINT32(*p_vpr, (volatile uint32_t)(vpr & ~(VPR_MSK)));
    else
        WRITE_UINT32(*p_vpr, (volatile uint32_t)(vpr | VPR_MSK));
 
}

/*****************************************************************************/
static void osMpicSpuriousIsr(os_mpic_arg arg)
{
    VAR_UNUSED(arg);
    OS_ASSERT;
}

/****************************************************************************/

os_status osMpicConfig(os_mpic_config_params_t *mpic_params,os_mpic_t **mpic_handle )
{
    os_mpic_t   *p_mpic;
    uint16_t    intr_count_by_group[] = MPIC_INTR_COUNT_BY_GROUP;
    uint32_t    i, first_internal_intr, last_internal_intr;
 
    /*Sanity check*/
    OS_ASSERT_COND(mpic_params != NULL);
 
    /* MPIC handle initialize */
    p_mpic = (void *)osMalloc(sizeof(os_mpic_t), OS_MEM_DDR0_SHARED);
    OS_ASSERT_COND(p_mpic != NULL);
    *mpic_handle = p_mpic;
 
    /* Store parameters */
    p_mpic->guest_id  = mpic_params->guest_id;
 
    /* Set interrupt count and base indices for all interrupt groups
       (Do not move to osMpicInit() - needed also in osMpicConfigInterruptSources) */
    p_mpic->intr_count_by_group[0] = intr_count_by_group[0];
    p_mpic->intr_group_base_idx[0] = 0;
 
    for (i=1; i < MPIC_NUM_OF_INTR_GROUPS; i++)
    {
        p_mpic->intr_count_by_group[i] = intr_count_by_group[i];
        p_mpic->intr_group_base_idx[i] = (uint16_t)(p_mpic->intr_group_base_idx[i-1] + intr_count_by_group[i-1]);
    }
 
    first_internal_intr = p_mpic->intr_group_base_idx[OS_MPIC_INTR_GROUP_INTERNAL];
    last_internal_intr  = (uint32_t)(first_internal_intr + intr_count_by_group[OS_MPIC_INTR_GROUP_INTERNAL] - 1);
 
    return OS_SUCCESS;
}

/****************************************************************************/

/****************************************************************************/
void osMpicClearPendingCoreInterrupts()
{
    uint32_t i, iack;
    for (i=0; i < MPIC_NUM_OF_INTR_SOURCES; i++)
    {
        /**
         * Reading IACK returns the interrupt vector corresponding to the highest priority pending interrupt
         * and it also has the following side effects:
         * The associated field in the corresponding interrupt pending register (IPR) is cleared for edge-sensitive interrupts.
         * The corresponding in-service register (ISR) is updated.
         * The corresponding int output signal from the MPIC is negated.
         */
        iack = (volatile uint32_t)GET_UINT32(g_soc_ccsr_map->mpic_memmap.mpic_global_memmap.private_per_cpu_par.iack);
        WRITE_UINT32(g_soc_ccsr_map->mpic_memmap.mpic_global_memmap.private_per_cpu_par.eoi, EOI_CODE);
    }
    DBAR_SCFG();
    WRITE_UINT32_DBAR_SCFG(g_soc_ccsr_map->mpic_memmap.mpic_core_memmap.core_per_cpu_par[OS_MPIC_CORES_ID_OFFSET + osGetCoreID()].per_cpu_par.ctpr, 0);
}

/****************************************************************************/
os_status osMpicInit()
{
    mpic_memmap_t       *mpic_memmap;
    uint32_t            total_isr_count = MPIC_NUM_OF_INTR_SOURCES + 1;
    volatile uint32_t   iack;
    uint16_t            i, j, multi_src_idx, multi_src_offset, multi_src_vector;
    os_mpic_multi_src_entry_t  multi_src_entries[] = MPIC_MULTI_SRC_INTERNAL_INTERRUPTS;
 
    /*Sanity checks*/
    OS_ASSERT_COND(g_dsp_mpic_handle != NULL);

    mpic_memmap = &g_soc_ccsr_map->mpic_memmap;
 
 
    /* totalIsrCount is initialized to MPIC_NUM_OF_INTR_SOURCES + 1 (spurious).
       We also need to add entries for multi-source interrupt handlers */
    for (i=0; i < __ARRAY_SIZE(multi_src_entries); i++)
        total_isr_count += multi_src_entries[i].num_of_sources;
 
    /* Initialize interrupt service table */
    for (i=0; i < total_isr_count; i++)
    {
        /* Single device per interrupt source */
        g_dsp_mpic_handle->isr_table[i].isr = osMpicUnassignedIsr;
        g_dsp_mpic_handle->isr_table[i].arg = (os_mpic_arg)i;  /* For error reporting */
    }
 
    /* Initialize multi-source interrupt handlers */
    multi_src_offset = (uint16_t)(MPIC_NUM_OF_INTR_SOURCES + 1/* spurious */);

    for (i=0; i < __ARRAY_SIZE(multi_src_entries); i++)
    {
        multi_src_idx    = (uint16_t)(g_dsp_mpic_handle->intr_group_base_idx[OS_MPIC_INTR_GROUP_INTERNAL] + multi_src_entries[i].index);
        multi_src_vector = multi_src_idx;

        g_dsp_mpic_handle->isr_table[multi_src_vector].isr  = osMpicMultiSourceIsr;
        g_dsp_mpic_handle->isr_table[multi_src_vector].arg =
            MPIC_MAKE_MULTI_SRC_HANDLE(multi_src_offset, multi_src_entries[i].num_of_sources);

        multi_src_offset += multi_src_entries[i].num_of_sources;
    }

    /* Set the dedicated ISR to the ORed error interrupt */
    g_dsp_mpic_handle->isr_table[MPIC_ORED_ERROR_VECTOR].isr = osMpicErrorIsr;

    /* Set the spurious interrupt handler in the last entry */
    g_dsp_mpic_handle->isr_table[MPIC_NUM_OF_INTR_SOURCES].isr = osMpicSpuriousIsr;
    g_dsp_mpic_handle->isr_table[MPIC_NUM_OF_INTR_SOURCES].arg = NULL;
 
    /* Initialize g_dsp_mpic_handle registers in master mode only */
    if (g_dsp_mpic_handle->guest_id == osGetMpicMasterCore())
    {
        /**
         * Set spurious interrupt vector.
         * Under certain circumstances, the MPIC has no valid vector to return to a processor core during an interrupt acknowledge cycle.
         * In these cases, the spurious vector from the spurious vector register is returned.
         */
        WRITE_UINT32(mpic_memmap->mpic_global_memmap.gcr.svr, (uint32_t)MPIC_NUM_OF_INTR_SOURCES);
 
        /* Set Mixed mode */
        SET_UINT32(mpic_memmap->mpic_global_memmap.gcr.gcr, GCR_MIXED_MODE);
 
        /* Enable message interrupts */
        WRITE_UINT32(mpic_memmap->mpic_global_memmap.global_msg_a.mer, MER_INIT_MASK);
        WRITE_UINT32(mpic_memmap->mpic_global_memmap.global_msg_b.mer, MER_INIT_MASK);
 
        /* Read IACK repeatedly to clear all pending and in-service registers  for all DSP cores*/
        for (i=0; i <OS_SOC_MAX_NUM_OF_CORES; i++)
        {
            for (j=0; j < MPIC_NUM_OF_INTR_SOURCES; j++)
            {
                /**
                 * Reading IACK returns the interrupt vector corresponding to the highest priority pending interrupt
                 * and it also has the following side effects:
                 * The associated field in the corresponding interrupt pending register (IPR) is cleared for edge-sensitive interrupts.
                 * The corresponding in-service register (ISR) is updated.
                 * The corresponding int output signal from the MPIC is negated.
                 */
                iack = (volatile uint32_t)GET_UINT32(mpic_memmap->mpic_core_memmap.core_per_cpu_par[OS_MPIC_CORES_ID_OFFSET +i].per_cpu_par.iack);
                WRITE_UINT32(mpic_memmap->mpic_core_memmap.core_per_cpu_par[OS_MPIC_CORES_ID_OFFSET +i].per_cpu_par.eoi, EOI_CODE);
            }
        }
    }
 
	/* Set Task Priority threshold to 0 to enable all interrupts */
	for (i = 0; i< OS_SOC_MAX_NUM_OF_CORES; i++)
		WRITE_UINT32(mpic_memmap->mpic_core_memmap.core_per_cpu_par[OS_MPIC_CORES_ID_OFFSET +i].per_cpu_par.ctpr, 0);

    g_dsp_mpic_handle->initialized = TRUE;

    return OS_SUCCESS;
}



