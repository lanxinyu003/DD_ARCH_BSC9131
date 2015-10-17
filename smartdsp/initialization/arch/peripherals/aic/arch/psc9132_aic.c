/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/06/18 13:30:02 $
 $Id: psc9132_aic.c,v 1.3 2014/06/18 13:30:02 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/arch/psc9132_aic.c,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************

 @File          psc9132_aic.c

 @Description   This file contains the implementation of the AIC 9131 specific
                functions that are used only in initialization.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "memmap.h"
#include "psc913x_gpio.h"
#include "psc9132_aic_.h"



/* AIC TDD is supported only in 9132 rev1.1 */

static void aicLane1TddUlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t      *aic_adi_lane;
    uint32_t            aic_dsp_lte_interrupt_status_reg;
 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane                   = (aic_adi_lane_t *)aic_lane;
 
    READ_UINT32(aic_dsp_lte_interrupt_status_reg, aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg);
 
    CLEAR_LEVEL_INTERRUPT(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg, AIC_LANE1_TDD_UL_INT);
 
    if ((aic_dsp_lte_interrupt_status_reg & AIC_LANE1_TDD_UL_STS) == 0) //it is a UL_OFF event (see in TDD_Interposer_Card.pdf)
    {
        if (aic_adi_lane->tdd_ul_off)
        {
            aic_adi_lane->tdd_ul_off(AIC_ADI_LANE1);
        }
    }
}

/*****************************************************************************/
static void aicLane2TddUlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t      *aic_adi_lane;
    uint32_t            aic_dsp_lte_interrupt_status_reg;
 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane                   = (aic_adi_lane_t *)aic_lane;
 
    READ_UINT32(aic_dsp_lte_interrupt_status_reg, aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg);
 
    CLEAR_LEVEL_INTERRUPT(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg, AIC_LANE2_TDD_UL_INT);
 
    if ((aic_dsp_lte_interrupt_status_reg & AIC_LANE2_TDD_UL_STS) == 0) //it is a UL_OFF event (see in TDD_Interposer_Card.pdf)
    {
        if (aic_adi_lane->tdd_ul_off)
        {
            aic_adi_lane->tdd_ul_off(AIC_ADI_LANE2);
        }
    }
}

/*****************************************************************************/
static void aicLane1TddDlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t      *aic_adi_lane;
    uint32_t            aic_dsp_lte_interrupt_status_reg;
 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane                   = (aic_adi_lane_t *)aic_lane;
 
    READ_UINT32(aic_dsp_lte_interrupt_status_reg, aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg);
 
    CLEAR_LEVEL_INTERRUPT(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg, AIC_LANE1_TDD_DL_INT);
 
    if ((aic_dsp_lte_interrupt_status_reg & AIC_LANE1_TDD_DL_STS) == 0) //it is a DL_OFF event (see in TDD_Interposer_Card.pdf)
    {
        if (aic_adi_lane->tdd_dl_off)
        {
            aic_adi_lane->tdd_dl_off(AIC_ADI_LANE1);
        }
    }
}


/*****************************************************************************/
static void aicLane2TddDlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t      *aic_adi_lane;
    uint32_t            aic_dsp_lte_interrupt_status_reg;
 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane                   = (aic_adi_lane_t *)aic_lane;
 
    READ_UINT32(aic_dsp_lte_interrupt_status_reg, aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg);
 
    CLEAR_LEVEL_INTERRUPT(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_status_reg, AIC_LANE2_TDD_DL_INT);
 
    if ((aic_dsp_lte_interrupt_status_reg & AIC_LANE2_TDD_DL_STS) == 0) //it is a DL_OFF event (see in TDD_Interposer_Card.pdf)
    {
        if (aic_adi_lane->tdd_dl_off)
        {
            aic_adi_lane->tdd_dl_off(AIC_ADI_LANE2);
        }
    }
}


/*****************************************************************************/

os_status aicArchLteTddUlConfigure(aic_adi_lane_t *aic_adi_lane, aic_lane_interrupt_t *interrupts_table, uint8_t interrupt_index_in_table)
{
    os_status status;
 
    OS_ASSERT_COND(GET_UINT32(*(uint32_t *)0XFEFE00A4) == 0X86180011);  //Assert GUTS_SVR register (not mapped in our memmap structures) equals 9132 rev1.1 number 0x86180011
    OS_ASSERT_COND(aic_adi_lane->aic_adi_lane_num <= AIC_ADI_LANE2);    //Supported for the 1st 2 lanes only
 
    status = osHwiCreate(OS_INT_AIC_SNIFF_FRAME0 + (uint16_t)aic_adi_lane->aic_adi_lane_num, interrupts_table[interrupt_index_in_table].interrupt_priority,
            LEVEL_MODE, (aic_adi_lane->aic_adi_lane_num == AIC_ADI_LANE1) ? aicLane1TddUlIsr : aicLane2TddUlIsr, (os_hwi_arg)aic_adi_lane);
    OS_ASSERT_COND(status == OS_SUCCESS);
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].hwi_num   = OS_INT_AIC_SNIFF_FRAME0 + (uint16_t)aic_adi_lane->aic_adi_lane_num;
    SET_UINT32(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_ctrl_reg, (AIC_LANE1_TDD_UL_INT << (uint8_t)aic_adi_lane->aic_adi_lane_num));
 
    return status;
}

/**********************************************************************************************/

os_status aicArchLteTddDlConfigure(aic_adi_lane_t *aic_adi_lane, aic_lane_interrupt_t *interrupts_table, uint8_t interrupt_index_in_table)
{
    os_status status;
 
    OS_ASSERT_COND(GET_UINT32(*(uint32_t *)0XFEFE00A4) == 0X86180011);  //Assert GUTS_SVR register (not mapped in our memmap structures) equals 9132 rev1.1 number 0x86180011
    OS_ASSERT_COND(aic_adi_lane->aic_adi_lane_num <= AIC_ADI_LANE2);    //Supported for the 1st 2 lanes only
 
    status = osHwiCreate(OS_INT_AIC_SNIFF_CAPT_COMP0 + (uint16_t)aic_adi_lane->aic_adi_lane_num, interrupts_table[interrupt_index_in_table].interrupt_priority,
            LEVEL_MODE, (aic_adi_lane->aic_adi_lane_num == AIC_ADI_LANE1) ? aicLane1TddDlIsr : aicLane2TddDlIsr, (os_hwi_arg)aic_adi_lane);
    OS_ASSERT_COND(status == OS_SUCCESS);
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].hwi_num   = OS_INT_AIC_SNIFF_CAPT_COMP0 + (uint16_t)aic_adi_lane->aic_adi_lane_num;
    SET_UINT32(aic_adi_lane->aic->aic_memory_map_registers_base->aic_type2.adi_registers.dsp_lte_interrupt_ctrl_reg, (AIC_LANE1_TDD_DL_INT << (uint8_t)aic_adi_lane->aic_adi_lane_num));
 
    return status;
}
