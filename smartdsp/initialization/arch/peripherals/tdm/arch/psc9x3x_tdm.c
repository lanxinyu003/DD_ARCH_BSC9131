/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/02/18 12:42:51 $
 $Id: psc9x3x_tdm.c,v 1.6 2013/02/18 12:42:51 b41640 Exp $
 $Source: /cvsdata/SmartDSP/initialization/arch/peripherals/tdm/arch/psc9x3x_tdm.c,v $
 $Revision: 1.6 $
******************************************************************************/

/******************************************************************************

 @File          psc9x3x_tdm.c

 @Description   This file contains the implementation of the TDM
                driver.

 @Cautions      None.

*//***************************************************************************/


#include "smartdsp_os_.h"
#include "os_init.h"
#include "tdm_init.h"
#include "psc9x3x_tdm_.h"
#include "tdm_.h"

tdm_t   *tdms[NUM_OF_TDMS];

static void tdmGeneralError(os_hwi_arg arg)
{
    uint32_t        reg;
    uint8_t         tdm_num;
    uint32_t        error;

    VAR_UNUSED(arg);
 
    /*check which TDM caused the error*/
    READ_UINT32(reg ,DEVICE_REGISTERS->general_config.gir2);
    for(tdm_num = 0; tdm_num < NUM_OF_TDMS; tdm_num++)
    {
        /* no interrupt on this TDM */
        if((((TDM_TX_ERROR | TDM_RX_ERROR) << (tdm_num*2)) & reg) == 0) continue;
 
        /* serve TX error */
        if((TDM_TX_ERROR << (tdm_num*2)) & reg)
        {
            READ_UINT32(error ,DEVICE_REGISTERS->tdm[tdm_num].tdmter);
            WRITE_UINT32(DEVICE_REGISTERS->tdm[tdm_num].tdmter, error & TDMTER_ERRORS_ONLY);
            /* call user callback if supplied */
            if (tdms[tdm_num] && tdms[tdm_num]->interrupt_error_callback)
                tdms[tdm_num]->interrupt_error_callback((void *)((TDM_TX_ERROR << (tdm_num*2)) & reg));
        }

        /* serve RX error */
        if((TDM_RX_ERROR << (tdm_num*2)) & reg)
        {
            READ_UINT32(error ,DEVICE_REGISTERS->tdm[tdm_num].tdmrer);
            WRITE_UINT32(DEVICE_REGISTERS->tdm[tdm_num].tdmrer, error & TDMRER_ERRORS_ONLY);
            /* call user callback if supplied */
            if (tdms[tdm_num] && tdms[tdm_num]->interrupt_error_callback)
                tdms[tdm_num]->interrupt_error_callback((void *)((TDM_RX_ERROR << (tdm_num*2)) & reg));
        }
 
        /* clear error interrupt pending */
        WRITE_UINT32(DEVICE_REGISTERS->general_config.gir2,((TDM_TX_ERROR | TDM_RX_ERROR << (tdm_num*2)) & reg));
    }
}

static void tdm_set_pa_pmux()
{
    uint32_t reg;
 
    //General initialization needed for  usage of TDM on PSC9131
    READ_UINT32(reg, g_dsp_pa_ccsr_map->device_config.pmuxcr1);
    reg &= 0xFFFC3FFFF;
    reg |= 0x00028000;
    WRITE_UINT32(g_dsp_pa_ccsr_map->device_config.pmuxcr1, reg);
 
}

void tdm_gen_err_int_create_and_enable(void* tdm, uint8_t tdm_num, os_hwi_priority priority)
{
    uint32_t        reg;
 
    tdms[tdm_num] = (tdm_t*)tdm;
    osHwiCreate((os_hwi_handle)(OS_INT_RX_ERR_INT0 + (tdm_num*6)),
                priority,
                LEVEL_MODE,
                tdmGeneralError,
                0);
    osHwiCreate((os_hwi_handle)(OS_INT_TX_ERR_INT0 + (tdm_num*6)),
                priority,
                LEVEL_MODE,
                tdmGeneralError,
                0);
    READ_UINT32(reg ,DEVICE_REGISTERS->general_config.gier2_mpic);
    reg |= ((TDM_RX_ERROR | TDM_TX_ERROR) << (tdm_num*2));
    WRITE_UINT32(DEVICE_REGISTERS->general_config.gier2_mpic, reg);
 
    //general init needed to work with 9131 TDM
    tdm_set_pa_pmux();
}
