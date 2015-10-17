/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_tick_init.c,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_tick_init.c,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_tick_init.c

 @Description   sc3x00-specific Tick setup functions.
 

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_tick_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"

extern uint32_t g_tick_parameter;
extern uint32_t g_core_clock;
static uint32_t g_tick_per_second;


//;*************************************************************************
//;
//;     Function:           osTickIntProgram
//:
//:     Inputs:             None
//:
//:     Outputs:            None
//;
//;     Description:        This function starts the hardware timer for
//;                         the OS.  MSC8101 specific implementation.
//;                         PIT and BRG0 are used to provide OS tick.
//;
//;     Cautions:           none
//;
//;*************************************************************************
void osTickIntProgram()
{
    os_status status;
    uint32_t divider_value;

    // Tie the interrupt handler to the interrupt
    status = osHwiCreate(OS_INT_TM0, OS_HWI_PRIORITY0,
                        EDGE_MODE, osTickHwiHandler, 0);
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* value and pre-load value */
    divider_value = g_core_clock*1000000/g_tick_parameter;
    WRITE_UINT32(g_dsp_plat_map->timer.platform_timer[0].tm_tp, divider_value );
    WRITE_UINT32(g_dsp_plat_map->timer.platform_timer[0].tm_tv, divider_value );
    /* Enable in periodic mode */

    WRITE_UINT32(g_dsp_plat_map->timer.platform_timer[0].tm_tc, 0x00000003);

    /* save ticks parameter */
    g_tick_per_second = g_tick_parameter;
 
}


/*****************************************************************************/
void osTickIntStart()
{
}


/*****************************************************************************/
uint32_t osGetTickPerSecond()
{
    return g_tick_per_second;
}
