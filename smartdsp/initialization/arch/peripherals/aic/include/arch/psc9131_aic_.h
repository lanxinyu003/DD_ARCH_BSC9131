/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/05/15 10:24:09 $
 $Id: psc9131_aic_.h,v 1.14 2014/05/15 10:24:09 b33105 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/aic/include/arch/psc9131_aic_.h,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************

 @File          psc9131_aic_.h

 @Description   AIC PSC9131 specific internal header file.

 @Cautions      None.

*//***************************************************************************/

#ifndef __AIC_PSC9131_H_
#define __AIC_PSC9131_H_

#include "memmap.h"
#include "aic_.h"


#define AIC_MAPLE_ANT0_RX_INT_EN    0x00010000
#define AIC_MAPLE_ANT0_TX_INT_EN    0x00040000

#define MICROSECOND     (osGetCoreClock())
#define MILLISECOND     (osGetCoreClock() * 1000)
#define TWO_TTI         (2 * MILLISECOND)

static inline void aicArchAdiLaneIrqToMapleOpen(int mode, aic_adi_channel_num_t channel_num)
{
    if (mode & SIO_READ)
    {
        SET_UINT32(g_dsp_ccsr_map->general_config.maple_ext_req_en, (AIC_MAPLE_ANT0_RX_INT_EN << (uint8_t)channel_num));
    }
    else if (mode & SIO_WRITE)
    {
        SET_UINT32(g_dsp_ccsr_map->general_config.maple_ext_req_en, (AIC_MAPLE_ANT0_TX_INT_EN << (uint8_t)channel_num));
    }
    else
    {
        OS_ASSERT;
    }
}

static inline void aicArchAdiLaneIrqToMapleClose(int mode, aic_adi_channel_num_t channel_num)
{
    if (mode & SIO_READ)
    {
        CLEAR_UINT32(g_dsp_ccsr_map->general_config.maple_ext_req_en, (AIC_MAPLE_ANT0_RX_INT_EN << (uint8_t)channel_num));
    }
    else if (mode & SIO_WRITE)
    {
        CLEAR_UINT32(g_dsp_ccsr_map->general_config.maple_ext_req_en, (AIC_MAPLE_ANT0_TX_INT_EN << (uint8_t)channel_num));
    }
    else
    {
        OS_ASSERT;
    }
}

os_status aicArchLteTddUlConfigure(aic_adi_lane_t*, aic_lane_interrupt_t *, uint8_t);
os_status aicArchLteTddDlConfigure(aic_adi_lane_t*, aic_lane_interrupt_t *, uint8_t);

#endif // __AIC_PSC9131_H_
