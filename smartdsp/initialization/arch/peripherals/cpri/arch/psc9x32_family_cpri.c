/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/19 17:59:20 $
 $Id: psc9x32_family_cpri.c,v 1.12 2013/02/19 17:59:20 b41640 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/cpri/arch/psc9x32_family_cpri.c,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************

 @File          psc9x32_family_cpri.c

 @Description   This file contains the implementation of the CPRI
                driver.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"
#include "os_init.h"
#include "os_bio_lld_.h"
#include "os_cache.h"
#include "sc3x00_cache_.h"
#include "os_mmu.h"

/* Path relative to SmartDSP\drivers\cpri */
#include "cpri.h"
#include "cpri_.h"
#include "cpri_shared_.h"

/* Path relative to SmartDSP\initialization\arch\peripherals */
#include "cpri_init.h"

void cpriArchInterruptEnable(uint32_t int_type, cpri_num_t cpri_num)
{
    extern psc9x3x_ccsr_t *g_dsp_ccsr_map;
    uint32_t reg;
 
    switch (int_type)
    {
        case CPRI_INT_TRANSMIT_CONTROL:
            reg = (1<<24)<<cpri_num;
            break;
        case CPRI_INT_RECEIVE_CONTROL:
            reg = (1<<16)<<cpri_num;
            break;
        case CPRI_INT_TRANSMIT_TIMING:
            reg = (1<<8)<<cpri_num;
            break;
        case CPRI_INT_RECEIVE_TIMING:
            reg = (1<<0)<<cpri_num;
            break;
        default: OS_ASSERT;

    }
    SET_UINT32(g_dsp_ccsr_map->general_config.gir6.gier[osGetCoreID()], reg);
}

cpri_num_t  cpriPairNumGet(cpri_num_t cpri_num)
{
    switch (cpri_num)
    {
        case CPRI_0:
            return CPRI_1;
            break;
        case CPRI_1:
            return CPRI_0;
            break;
        default:
            OS_ASSERT;
            return CPRI_NOT_USED;
            break;
    }
}

os_status   cpriInitializeExtendedFeatures(cpri_num_t cpri_num, void* extended_features)
{
    VAR_UNUSED(cpri_num);
    VAR_UNUSED(extended_features);
 
    //same functionality as cpriExternalSyncSet - switches signals CP_SYNC1, CP_SYNC2, CP_LOS1, CP_LOS2, CP_TX_INT_B
    cpriExternalSyncSet(cpri_num, CPRI_ACTIVE);
    return OS_SUCCESS;
}

os_status cpriArchStatisticsSet()
{
    uint32_t reg;
 
    READ_UINT32(reg, g_dsp_pa_ccsr_map->serdes_control.srdsb2_pllcr0);
    reg &= 0xCFFFFFFF;
    reg |= 0x18000000;
    WRITE_UINT32(g_dsp_pa_ccsr_map->serdes_control.srdsb2_pllcr0, reg);
    SET_UINT32(g_dsp_ccsr_map->general_config.gcr8, 0x80000000);
 
    return OS_SUCCESS;
}

void cpriExternalSyncSet(cpri_num_t cpri_num, bool sync_mode)
{
    uint32_t reg;
 
    VAR_UNUSED(cpri_num);
    VAR_UNUSED(sync_mode);
 
    READ_UINT32(reg, g_dsp_pa_ccsr_map->device_config.pmuxcr3);
    reg &= 0xFFF3FFFF;
    reg |= 0x00040000;
    WRITE_UINT32(g_dsp_pa_ccsr_map->device_config.pmuxcr3, reg);
 
}

double cpriSerdesDelaysCalculate(cpri_num_t  cpri_num)
{
    double delay;
    uint32_t reg;
 
    VAR_UNUSED(cpri_num);
    READ_UINT32(reg, g_dsp_pa_ccsr_map->serdes_control.pcvtrcpricr0);
    reg &= 0xFFFF;
    delay = (double)reg / (1000000000000/8);
 
    READ_UINT32(reg, g_dsp_pa_ccsr_map->serdes_control.pcvtrcpricr1);
    reg &= 0xFFFF;
    delay += (double)reg / (1000000000000/8);
 
    return delay;
}
