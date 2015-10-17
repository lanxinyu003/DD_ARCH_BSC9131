/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/05/26 13:08:02 $
 $Id: psc9131_aic.c,v 1.8 2014/05/26 13:08:02 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/arch/psc9131_aic.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************

 @File          psc9131_aic.c

 @Description   This file contains the implementation of the AIC 9131 specific
                functions that are used only in initialization.

 @Cautions      None.

*//***************************************************************************/


#include "smartdsp_os_.h"
#include "memmap.h"
#include "psc913x_gpio.h"
#include "psc9131_aic_.h"


static void aicLaneTddUlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t  *aic_adi_lane;
 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane = (aic_adi_lane_t *)aic_lane;
 
    if (osGpioEventReadAndClear(1, 25)) // if UL_OFF event occurred
    {
        if (aic_adi_lane->tdd_ul_off)
        {
            aic_adi_lane->tdd_ul_off(AIC_ADI_LANE3);
        }
    }
}

/**********************************************************************************************/

static void aicLaneTddDlIsr(os_hwi_arg aic_lane)
{
    aic_adi_lane_t  *aic_adi_lane;
 

 
    OS_ASSERT_COND(aic_lane != NULL);
 
    aic_adi_lane = (aic_adi_lane_t *)aic_lane;
 
    if (osGpioEventReadAndClear(1, 24)) // if UL_OFF event occurred
    {
        if (aic_adi_lane->tdd_dl_off)
        {
            aic_adi_lane->tdd_dl_off(AIC_ADI_LANE3);
        }
    }
}


/**********************************************************************************************/

/* The 9131 receives UL_OFF and DL_OFF events from interposer through GPIO57 and GPIO56 pins respectively */

os_status aicArchLteTddUlConfigure(aic_adi_lane_t *aic_adi_lane, aic_lane_interrupt_t *interrupts_table, uint8_t interrupt_index_in_table)
{
    os_status   status;
    uint32_t    reg;
 
    OS_ASSERT_COND(aic_adi_lane->aic_adi_lane_num == AIC_ADI_LANE3);    //Supported for lane 3 only
 
    /* mux GPIO57 */
    READ_UINT32(reg, g_dsp_pa_ccsr_map->device_config.pmuxcr3);
    reg &= 0XFF3FFFFF;
    reg |= 0X00800000;
    WRITE_UINT32(g_dsp_pa_ccsr_map->device_config.pmuxcr3, reg);
 
    // Configure GPIO57
    status = osGpioHwiConfigure(1, 25, 1); // The 3rd parameter makes sure we receive only UL_OFF events (we don't want UL_ON events)
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    status = osHwiMultiplexedCreate(OS_INT_GPIO, interrupts_table[interrupt_index_in_table].interrupt_priority, LEVEL_MODE, aicLaneTddUlIsr, (os_hwi_arg)aic_adi_lane);
    OS_ASSERT_COND(status == OS_SUCCESS);
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].hwi_num   = OS_INT_GPIO;
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].isr       = aicLaneTddUlIsr;   // It's a multiplexed interrupt, that's why the ISR is required to identify it when we'll remove it.

    return status;
}

/**********************************************************************************************/

os_status aicArchLteTddDlConfigure(aic_adi_lane_t *aic_adi_lane, aic_lane_interrupt_t *interrupts_table, uint8_t interrupt_index_in_table)
{
    os_status   status;
    uint32_t    reg;
 
    OS_ASSERT_COND(aic_adi_lane->aic_adi_lane_num == AIC_ADI_LANE3);    //Supported for lane 3 only
 
    /* mux GPIO56 */
    READ_UINT32(reg, g_dsp_pa_ccsr_map->device_config.pmuxcr3);
    reg &= 0XFCFFFFFF;
    reg |= 0X02000000;
    WRITE_UINT32(g_dsp_pa_ccsr_map->device_config.pmuxcr3, reg);
 
    // Configure GPIO56
    status = osGpioHwiConfigure(1, 24, 1); // The 3rd parameter makes sure we receive only DL_OFF events (we don't want DL_ON events)
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    status = osHwiMultiplexedCreate(OS_INT_GPIO, interrupts_table[interrupt_index_in_table].interrupt_priority, LEVEL_MODE, aicLaneTddDlIsr, (os_hwi_arg)aic_adi_lane);
    OS_ASSERT_COND(status == OS_SUCCESS);
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].hwi_num   = OS_INT_GPIO;
    aic_adi_lane->registered_interrupts[osGetCoreID()][interrupt_index_in_table].isr       = aicLaneTddDlIsr;   // It's a multiplexed interrupt, that's why the ISR is required to identify it when we'll remove it.

    return status;
}
