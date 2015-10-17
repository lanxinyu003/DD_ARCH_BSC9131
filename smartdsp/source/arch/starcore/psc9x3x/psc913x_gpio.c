/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/05/15 12:46:59 $
 $Id: psc913x_gpio.c,v 1.5 2014/05/15 12:46:59 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/psc9x3x/psc913x_gpio.c,v $
 $Revision: 1.5 $
******************************************************************************/

/******************************************************************************
 
 @File          psc9x3x_gpio.c

 @Description   PSC9X3X GPIO functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"

#include "psc9x3x_hwi.h"
#include "psc913x_gpio.h"
#include "psc9x3x_memmap.h"

/*************************************************************************/
os_status osGpioHwiConfigure(int gpio_num, int port, bool edge)
{

    OS_ASSERT_COND(gpio_num < OS_NUM_OF_GPIO);
    OS_ASSERT_COND(port <= OS_NUM_OF_GPIO_PORTS);
 
    CLEAR_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpdir, 1<<(31-port));
    SET_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpimr, 1<<(31-port));
    if (edge == OS_GPIO_EDGE_HIGH_TO_LOW)
    {
        SET_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpicr, 1<<(31-port));
    }
    else
    {
        CLEAR_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpicr, 1<<(31-port));
    }
 
    return OS_SUCCESS;
}

/*************************************************************************/
os_status osGpioOutputSet(int gpio_num, int port, bool value)
{
 
    OS_ASSERT_COND(gpio_num < OS_NUM_OF_GPIO);
    OS_ASSERT_COND(port <= OS_NUM_OF_GPIO_PORTS);
 
    SET_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpdir, 1<<(31-port));
    CLEAR_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpodr, 1<<(31-port));
    if (value == 1)
    {
        SET_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpdat, 1<<(31-port));
    }
    else if (value == 0)
    {
        CLEAR_UINT32(g_dsp_pa_ccsr_map->gpio[gpio_num].gpdat, 1<<(31-port));
    }
    else
        OS_ASSERT;
 
    return OS_SUCCESS;
}


/*************************************************************************/
bool osGpioDataRead(int gpio_num, int port)
{
    uint32_t reg;
 
    OS_ASSERT_COND(gpio_num < OS_NUM_OF_GPIO);
    OS_ASSERT_COND(port <= OS_NUM_OF_GPIO_PORTS);
 
    READ_UINT32(reg, g_dsp_pa_ccsr_map->gpio[gpio_num].gpdat);
    if ((reg & (1 << (31-port))) == 0)
        return 0;
    else
        return 1;
}

/*************************************************************************/
bool osGpioEventReadAndClear(int gpio_num, int port)
{
    uint32_t reg;
 
    OS_ASSERT_COND(gpio_num < OS_NUM_OF_GPIO);
    OS_ASSERT_COND(port <= OS_NUM_OF_GPIO_PORTS);
 
    READ_UINT32(reg, g_dsp_pa_ccsr_map->gpio[gpio_num].gpier);
    if ((reg & (1 << (31-port))) == 0)
    {
        return 0;
    }
    else
    {
        CLEAR_LEVEL_INTERRUPT(g_dsp_pa_ccsr_map->gpio[gpio_num].gpier, 1<<(31-port));
        return 1;
    }
}

