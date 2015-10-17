/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/07/24 17:14:19 $
 $Id: psc9x3x_clocks.c,v 1.7 2014/07/24 17:14:19 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc9x3x_clocks.c,v $
 $Revision: 1.7 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_clocks.c

 @Cautions      None.
 
*//***************************************************************************/
#include "smartdsp_os_.h"
#include "os_init.h"
#include "psc9x3x_clocks.h"
#include "smartdsp_os_device.h"
#include "memmap.h"

/* All the clocks here are in KHz */
static float local_clkin_val;
uint32_t g_clock_in;
uint32_t g_core_clock;
uint32_t g_periph_clock;

/*****************************************************************************/
INLINE uint32_t clock_round(float freq)
{
    // this assignment will round down
    uint32_t clock = (uint32_t)freq;
    if ((freq - clock) >= 0.5)
        clock += 1;
    return clock;
}

/*****************************************************************************/
os_status psc9x3xInitializeClocks(float clk_in)
{
	uint32_t    platform_ratio_from_reg;
	uint32_t    dsp_pll_from_reg;
	
    /* check input */
    if(clk_in==0)
    {
        return OS_FAIL;
    }
 
    /* initialize input clock rate */
    local_clkin_val = clk_in;
    g_clock_in = clock_round(clk_in);
 
    READ_UINT32(platform_ratio_from_reg, g_dsp_pa_ccsr_map->device_config.porpllsr);
    READ_UINT32(dsp_pll_from_reg, g_dsp_pa_ccsr_map->device_config.pordspdevsr);
    platform_ratio_from_reg = ((platform_ratio_from_reg >> 1) & 0X0000001F);
 
    g_periph_clock = clock_round(clk_in * platform_ratio_from_reg);
 
#ifdef PSC9131
    dsp_pll_from_reg >>= 28;
 
    if ((dsp_pll_from_reg == 1) || (dsp_pll_from_reg == 2))
    {
    	g_core_clock = 800;
    }
    else // dsp_pll_from_reg == 3 or 15
    {
    	g_core_clock = 1000;
    }
#elif defined(PSC9132)
    dsp_pll_from_reg >>= 27;
 
    if ((dsp_pll_from_reg == 3) || (dsp_pll_from_reg == 7) || (dsp_pll_from_reg == 8))
    {
    	g_core_clock = 1000;
    }
    else if ((dsp_pll_from_reg == 9) || (dsp_pll_from_reg == 10))
    {
    	g_core_clock = 1200;
    }
    else // dsp_pll_from_reg == 2 or 6 or 11 or 18 or 26
    {
    	g_core_clock = 800;
    }
#endif
 
 
 
    return OS_SUCCESS;
}
